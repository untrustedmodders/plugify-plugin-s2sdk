#include "inventory.hpp"

#if defined (CS2)

#include <algorithm>
#include <array>
#include <bit>
#include <format>
#include <new>
#include <string>
#include <unordered_map>
#include <utility>

#include <cstrike15_gcmessages.pb.h>
#include <econ/econ_item.h>
#include <tier0/memalloc.h>
#include <tier2/fieldpath.h>
#include <entity2/entityclass.h>
#include <entity2/entityidentity.h>
#include <entity2/entitynetwork.h>
#include <networksystem/iflattenedserializers.h>
#include <networksystem/networkvar.h>

#include "game_config.hpp"
#include "player_manager.hpp"
#include "sdk/entity/ccsplayercontroller.h"

namespace {
	using Loadout = std::array<std::array<LoadoutSlot_t, LOADOUT_SLOT_COUNT>, LOADOUT_COUNT>;
	static_assert(sizeof(Loadout) == sizeof(CCSPlayerInventory::m_LoadoutItems));
	constexpr size_t kLoadoutSlots = LOADOUT_COUNT * LOADOUT_SLOT_COUNT;

	using CustomData = CEconItem::CustomDataOptimizedObject_t;

	std::unordered_map<CCSPlayerInventory*, Loadout> s_gameLoadouts;
	std::unordered_map<CCSPlayerInventory*, std::unordered_map<uint64, CEconItem*>> s_createdItems;
	std::unordered_map<CCSPlayerInventory*, GCSDK::CGCClientSharedObjectCache*> s_ownCaches;

	uint64 s_nextItemId = 0x1000000000000000ull;

	IEconItemSchema* GetSchema() {
		return g_ppItemSystem && *g_ppItemSystem ? (*g_ppItemSystem)->GetItemSchemaInterface() : nullptr;
	}

	CEconItemSchema* GetItemSchema() {
		return static_cast<CEconItemSchema*>(GetSchema());
	}

	template<typename Map>
	auto FindInMap(const Map& map, int id) -> std::remove_reference_t<decltype(map.Element(0))> {
		auto index = map.Find(id);
		return index != map.InvalidIndex() ? map.Element(index) : nullptr;
	}

	std::pair<void*, void*> FindVTables(std::string_view name) {
		std::pair<void*, void*> result{};
		auto* server = g_GameConfigManager.GetModuleProvider().GetModule("server");
		for (const auto& info : server->GetVirtualTablesByName(name)) {
			(info.offset == 0 ? result.first : result.second) = info.address;
		}
		return result;
	}

	std::pair<void*, void*> GetItemVTables() {
		static std::pair<void*, void*> vtables = FindVTables("CEconItem");
		return vtables;
	}

	CEconItem::attribute_t* FindItemAttribute(CEconItem* item, const CEconItemAttributeDefinition* definition) {
		CustomData* block = item->m_pCustomDataOptimizedObject;
		uint32 count = block ? block->m_numAttributes : 0;
		for (uint32 i = 0; i < count; ++i) {
			auto attribute = block->GetAttribute(i);
			if (attribute->m_unDefinitionIndex == definition->GetDefinitionIndex())
				return attribute;
		}
		return nullptr;
	}

	CEconItem::attribute_t* AddItemAttribute(CEconItem* item) {
		CustomData* block = item->m_pCustomDataOptimizedObject;
		uint32 count = block ? block->m_numAttributes : 0;
		if (count >= 0xFF)
			return nullptr;

		size_t size = sizeof(CustomData) + (count + 1) * sizeof(CEconItem::attribute_t);
		CustomData* newBlock;
		if (block) {
			newBlock = static_cast<CustomData*>(MemAlloc_ReallocFunc(block, size));
			*newBlock->GetAttribute(count) = {};
		} else {
			newBlock = static_cast<CustomData*>(MemAlloc_CallocFunc(1, size));
			newBlock->m_equipInstanceSlot1 = INVALID_EQUIPPED_SLOT_BITPACKED;
		}
		newBlock->m_numAttributes = count + 1;
		item->m_pCustomDataOptimizedObject = newBlock;
		return newBlock->GetAttribute(count);
	}

	class AttributeReader final : public IEconItemAttributeIterator {
	public:
		explicit AttributeReader(attrib_definition_index_t index) : m_index(index) {}

		std::optional<std::string> value;

		bool OnIterateAttributeValue(const CEconItemAttributeDefinition* definition, attrib_value_t data) override {
			return Take(definition, [&] { return definition->IsStoredAsInteger() ? std::to_string(data) : std::format("{}", std::bit_cast<float>(data)); });
		}
		bool OnIterateAttributeValue(const CEconItemAttributeDefinition* definition, float data) override {
			return Take(definition, [&] { return std::format("{}", data); });
		}
		bool OnIterateAttributeValue(const CEconItemAttributeDefinition* definition, const CAttribute_String& data) override {
			return Take(definition, [&] { return std::string(data.value()); });
		}
		bool OnIterateAttributeValue(const CEconItemAttributeDefinition* definition, const Vector& data) override {
			return Take(definition, [&] { return std::format("{} {} {}", data.x, data.y, data.z); });
		}

	private:
		bool Take(const CEconItemAttributeDefinition* definition, auto&& format) {
			if (definition->GetDefinitionIndex() != m_index)
				return true;
			value = format();
			return false;
		}

		attrib_definition_index_t m_index;
	};

	void DestroyItem(CCSPlayerInventory* inventory, CEconItem* item) {
		if (inventory->m_pSOCache && inventory->m_pSOCache->RemoveObject(*item)) {
			inventory->SODestroyed(inventory->m_OwnerID, item, GCSDK::eSOCacheEvent_Incremental);
			delete item;
		}
	}

	bool CreateOwnCache(CCSPlayerInventory* inventory) {
		static void* vtable = FindVTables("GCSDK::CGCClientSharedObjectCache").first;
		if (!vtable)
			return false;

		static_assert(sizeof(GCSDK::CGCClientSharedObjectCache) == 64);
		auto cache = static_cast<GCSDK::CGCClientSharedObjectCache*>(MemAlloc_CallocFunc(1, sizeof(GCSDK::CGCClientSharedObjectCache)));
		*reinterpret_cast<void**>(cache) = vtable;
		cache->m_owner = inventory->m_OwnerID;

		inventory->SOCacheSubscribed(inventory->m_OwnerID, cache, GCSDK::eSOCacheEvent_Subscribed);
		s_ownCaches[inventory] = cache;
		return true;
	}

	using AttributeStorage = CUtlVector<CEconItemAttribute, int, CNetworkUtlVector_MemoryType<CEconItemAttribute>>;
	using Attributes = decltype(CAttributeList::m_Attributes);

	constexpr const char* kPaintAttributes[] = {"set item texture prefab", "set item texture seed", "set item texture wear"};

	void HideDefaultGloves(CCSPlayerPawn* pawn, bool hide) {
		pawn->SetBodygroupByName("first_or_third_person", hide ? 1 : 0);
	}

	void ClearGloves(CCSPlayerPawn* pawn) {
		HideDefaultGloves(pawn, false);
		auto gloves = pawn->m_EconGloves.Get();
		if (!gloves->m_bInitialized)
			return;

		gloves->m_bInitialized = false;
		gloves->m_iItemID = 0;
		gloves->m_iItemDefinitionIndex = static_cast<CSWeaponDefIndex>(0);
		++pawn->m_nEconGlovesChanged;
	}

	void SetAgentPatches(CCSPlayerPawn* pawn, CCSPlayerInventory* inventory, int team) {
		std::remove_reference_t<decltype(pawn->m_vecPlayerPatchEconIndices.Get())> patches{};
		const IEconItemInterface* agent = inventory ? inventory->GetItemInLoadout(team, LOADOUT_SLOT_CLOTHING_CUSTOMPLAYER) : nullptr;
		auto definition = agent ? agent->GetItemDefinition() : nullptr;
		if (definition && definition->GetDefaultLoadoutSlot() == LOADOUT_SLOT_CLOTHING_CUSTOMPLAYER && definition->GetNumberOfStickerSlots() > 0) {
			for (int i = 0; i < static_cast<int>(std::size(patches)); ++i) {
				patches[i] = agent->GetStickerAttributeBySlotIndexInt(i, k_EStickerAttribute_ID, 0);
			}
		}
		pawn->m_vecPlayerPatchEconIndices = patches;
	}

	bool ResolvePath(CCSPlayerPawn* pawn, Attributes& attributes) {
		if (attributes.IsPathResolved())
			return true;

		bool exact = false;
		CFieldPath path = g_pFlattenedSerializers->OffsetToFieldPath(pawn->m_pEntity->m_pClass->m_flattenedSerializer, pawn, &attributes, &exact);
		if (path.Count() <= 0)
			return false;

		attributes.m_PathToVector = path;
		attributes.m_nNetworkingFlags = exact;
		attributes.m_nResolveFlags |= NETWORK_RESOLVE_PATH_RESOLVED;
		return true;
	}

	void SetGlovesAttributes(CCSPlayerPawn* pawn, const IEconItemInterface* item) {
		static void* vtable = FindVTables("CEconItemAttribute").first;
		static const auto paint = [] {
			std::array<attrib_definition_index_t, std::size(kPaintAttributes)> result{};
			for (size_t i = 0; i < result.size(); ++i) {
				auto attribute = inventory::FindAttribute(kPaintAttributes[i]);
				result[i] = attribute ? attribute->GetDefinitionIndex() : 0;
			}
			return result;
		}();

		auto& attributes = pawn->m_EconGloves->m_NetworkedDynamicAttributes->m_Attributes;
		if (!vtable || !ResolvePath(pawn, attributes))
			return;

		const float values[] = {static_cast<float>(item->GetCustomPaintKitIndex()), static_cast<float>(item->GetCustomPaintKitSeed()), item->GetCustomPaintKitWear()};
		int count = values[0] != 0.0f ? static_cast<int>(std::size(values)) : 0;

		auto& storage = reinterpret_cast<AttributeStorage&>(attributes);
		storage.Purge();

		auto elements = count ? static_cast<CEconItemAttribute*>(MemAlloc_Alloc(count * sizeof(CEconItemAttribute))) : nullptr;
		for (int i = 0; i < count; ++i) {
			auto& element = elements[i];
			*reinterpret_cast<void**>(&element) = vtable;
			new (&element.__m_pChainEntity) CNetworkVarChainer{};
			element.m_iAttributeDefinitionIndex = paint[i];
			element.m_flValue = element.m_flInitialValue = values[i];
			element.m_nRefundableCurrency = 0;
			element.m_bSetBonus = false;

			CFieldPath path = attributes.m_PathToVector;
			path.m_Path[path.m_nCount++] = static_cast<int16>(i);
			element.__m_pChainEntity.SetObject(pawn);
			element.__m_pChainEntity.m_PathIndex = pawn->AddChangeAccessorPath(path);
			element.__m_pChainEntity.m_bNetworkingEnabled = true;
		}
		storage.AssumeMemory(elements, count, count);

		CAttributeList::NetworkVar_m_Attributes::NetworkStateChanged(&attributes);
		pawn->NetworkStateChangedBranch(attributes.m_PathToVector);
	}
} // namespace

CCSPlayerInventory* inventory::Get(int playerSlot) {
	if (!g_pInventoryManager)
		return nullptr;

	auto player = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (!player)
		return nullptr;

	return static_cast<CCSPlayerInventory*>(g_pInventoryManager->GetInventoryForAccount(player->GetSteamId().GetAccountID()));
}

Player* inventory::GetOwner(CCSPlayerInventory* inventory) {
	const GCSDK::SOID_t& owner = inventory->m_OwnerID;
	if (owner.Type() != GCSDK::k_SOID_Type_SteamID)
		return nullptr;

	return g_PlayerManager.ToPlayer(CSteamID(owner.ID()));
}

const CCStrike15ItemDefinition* inventory::GetItemDefinition(uint16 itemDefinition) {
	auto schema = GetSchema();
	return schema ? static_cast<const CCStrike15ItemDefinition*>(schema->GetItemDefinitionInterface(itemDefinition)) : nullptr;
}

const CCStrike15ItemDefinition* inventory::GetItemDefinitionByName(const char* name) {
	auto schema = GetSchema();
	return schema ? static_cast<const CCStrike15ItemDefinition*>(schema->GetItemDefinitionByName(name)) : nullptr;
}

void inventory::SaveGameLoadout(CCSPlayerInventory* inventory) {
	std::copy_n(&inventory->m_LoadoutItems[0][0], kLoadoutSlots, s_gameLoadouts[inventory].front().data());
}

void inventory::SaveGameLoadoutSlot(CCSPlayerInventory* inventory, int team, int loadoutSlot) {
	auto it = s_gameLoadouts.find(inventory);
	if (it != s_gameLoadouts.end()) {
		it->second[team][loadoutSlot] = inventory->m_LoadoutItems[team][loadoutSlot];
	}
}

bool inventory::RestoreGameLoadout(CCSPlayerInventory* inventory) {
	auto it = s_gameLoadouts.find(inventory);
	if (it == s_gameLoadouts.end())
		return false;

	std::copy_n(it->second.front().data(), kLoadoutSlots, &inventory->m_LoadoutItems[0][0]);
	return true;
}

void inventory::DropOwnCache(CCSPlayerInventory* inventory) {
	auto it = s_ownCaches.find(inventory);
	if (it == s_ownCaches.end())
		return;

	auto cache = it->second;
	s_ownCaches.erase(it);

	auto current = std::exchange(inventory->m_pSOCache, cache);
	Forget(inventory);
	inventory->m_pSOCache = current == cache ? nullptr : current;
	delete cache;
}

void inventory::Forget(CCSPlayerInventory* inventory) {
	if (auto it = s_createdItems.find(inventory); it != s_createdItems.end()) {
		for (auto [itemId, item] : it->second) {
			DestroyItem(inventory, item);
		}
		s_createdItems.erase(it);
	}
	s_gameLoadouts.erase(inventory);
}

void inventory::Shutdown() {
	if (g_pInventoryManager) {
		for (const auto& entry : g_pInventoryManager->m_pInventories) {
			auto inventory = static_cast<CCSPlayerInventory*>(entry.pInventory);
			RestoreGameLoadout(inventory);
			if (s_ownCaches.contains(inventory)) {
				DropOwnCache(inventory);
			} else {
				Forget(inventory);
			}
		}
	}

	s_ownCaches.clear();
	s_createdItems.clear();
	s_gameLoadouts.clear();
}

uint64 inventory::CreateItem(CCSPlayerInventory* inventory, uint16 itemDefinition, int quality) {
	auto [vtable, interfaceVTable] = GetItemVTables();
	if (!GetItemDefinition(itemDefinition) || !vtable || !interfaceVTable)
		return 0;

	if (!inventory->m_pSOCache && !CreateOwnCache(inventory))
		return 0;

	auto item = static_cast<CEconItem*>(MemAlloc_CallocFunc(1, sizeof(CEconItem)));
	reinterpret_cast<void**>(item)[0] = vtable;
	reinterpret_cast<void**>(item)[1] = interfaceVTable;
	item->m_iItemSet = 255;

	uint64 itemId = s_nextItemId++;
	item->m_ulID = itemId;
	item->m_ulOriginalID = itemId;
	item->m_unAccountID = static_cast<uint32>(inventory->m_OwnerID.ID());
	item->m_unDefIndex = itemDefinition;
	item->m_nQuality = static_cast<uint16>(quality);
	item->m_unLevel = 1;

	inventory->m_pSOCache->AddObject(item);
	inventory->SOCreated(inventory->m_OwnerID, item, GCSDK::eSOCacheEvent_Incremental);

	s_createdItems[inventory][itemId] = item;
	return itemId;
}

bool inventory::RemoveItem(CCSPlayerInventory* inventory, uint64 itemId) {
	auto item = FindCreatedItem(inventory, itemId);
	if (!item)
		return false;

	s_createdItems[inventory].erase(itemId);

	if (auto game = s_gameLoadouts.find(inventory); game != s_gameLoadouts.end()) {
		for (int team = 0; team < LOADOUT_COUNT; ++team) {
			for (int slot = 0; slot < LOADOUT_SLOT_COUNT; ++slot) {
				if (inventory->m_LoadoutItems[team][slot].m_ItemID == itemId) {
					inventory->m_LoadoutItems[team][slot] = game->second[team][slot];
				}
			}
		}
	}

	DestroyItem(inventory, item);
	return true;
}

CEconItem* inventory::FindCreatedItem(CCSPlayerInventory* inventory, uint64 itemId) {
	auto it = s_createdItems.find(inventory);
	if (it == s_createdItems.end())
		return nullptr;

	auto item = it->second.find(itemId);
	return item != it->second.end() ? item->second : nullptr;
}

void inventory::ValidateCreatedItems(CCSPlayerInventory* inventory) {
	auto it = s_createdItems.find(inventory);
	if (it == s_createdItems.end())
		return;

	std::erase_if(it->second, [inventory](const auto& entry) {
		return !FindItemView(inventory, entry.first);
	});
}

const CEconItemAttributeDefinition* inventory::FindAttribute(std::string_view name) {
	auto schema = GetItemSchema();
	if (!schema)
		return nullptr;

	static CEconItemSchema* s_schema = nullptr;
	static std::unordered_map<std::string, const CEconItemAttributeDefinition*, plg::string_hash, std::equal_to<>> s_definitions;
	if (s_schema != schema) {
		s_schema = schema;
		s_definitions.clear();
		for (const CEconItemAttributeDefinition* definition : schema->m_mapAttributesContainer) {
			if (definition) {
				s_definitions.emplace(definition->GetDefinitionName(), definition);
			}
		}
	}

	auto it = s_definitions.find(name);
	return it != s_definitions.end() ? it->second : nullptr;
}

const CEconItemAttributeDefinition* inventory::GetAttribute(int index) {
	auto schema = GetSchema();
	return schema ? static_cast<const CEconItemAttributeDefinition*>(schema->GetAttributeDefinitionInterface(index)) : nullptr;
}

CEconItemView* inventory::FindItemView(CCSPlayerInventory* inventory, uint64 itemId) {
	auto& views = inventory->m_mapItemIDToItemView;
	auto index = views.Find(itemId);
	return views.IsValidIndex(index) ? views[index] : nullptr;
}

CEconItemView* inventory::GetItemView(int playerSlot, uint64 itemId) {
	auto inventory = Get(playerSlot);
	return inventory ? FindItemView(inventory, itemId) : nullptr;
}

bool inventory::IsLoadoutSlot(int team, int loadoutSlot) {
	return team >= 0 && team < LOADOUT_COUNT && loadoutSlot >= 0 && loadoutSlot < LOADOUT_SLOT_COUNT;
}

CEconItemView* inventory::GetLoadoutItem(int playerSlot, int team, int loadoutSlot) {
	auto inventory = IsLoadoutSlot(team, loadoutSlot) ? Get(playerSlot) : nullptr;
	return inventory ? inventory->GetItemInLoadout(team, loadoutSlot) : nullptr;
}

std::optional<std::string> inventory::GetItemAttribute(const CEconItemView* item, std::string_view name) {
	auto definition = FindAttribute(name);
	if (!definition)
		return std::nullopt;

	AttributeReader reader(definition->GetDefinitionIndex());
	item->IterateAttributes(&reader);
	return std::move(reader.value);
}

bool inventory::SetItemAttribute(CCSPlayerInventory* inventory, CEconItem* item, std::string_view name, std::string_view value) {
	auto definition = FindAttribute(name);
	if (!definition)
		return false;

	auto type = const_cast<ISchemaAttributeType*>(definition->GetAttributeType());
	if (auto attribute = FindItemAttribute(item, definition)) {
		if (!type->BConvertStringToEconAttributeValue(definition, value.data(), &attribute->m_value))
			return false;
	} else {
		attribute_data_union_t data{};
		type->InitializeNewEconAttributeValue(&data);
		if (!type->BConvertStringToEconAttributeValue(definition, value.data(), &data)) {
			type->UnloadEconAttributeValue(&data);
			return false;
		}

		attribute = AddItemAttribute(item);
		if (!attribute) {
			type->UnloadEconAttributeValue(&data);
			return false;
		}
		attribute->m_unDefinitionIndex = definition->GetDefinitionIndex();
		attribute->m_value = data;
	}

	inventory->SOUpdated(inventory->m_OwnerID, item, GCSDK::eSOCacheEvent_Incremental);
	return true;
}

const CPaintKit* inventory::GetPaintKit(int id) {
	auto schema = GetItemSchema();
	return schema ? schema->GetPaintKitDefinitionByMapIndex(id) : nullptr;
}

const CPaintKit* inventory::FindPaintKit(const char* name) {
	auto schema = GetItemSchema();
	return schema ? schema->GetPaintKitDefinitionByName(name) : nullptr;
}

const CStickerKit* inventory::GetStickerKit(int id) {
	auto schema = GetItemSchema();
	return schema ? FindInMap(schema->m_mapStickerKits, id) : nullptr;
}

const CStickerKit* inventory::FindStickerKit(const char* name) {
	auto schema = GetItemSchema();
	if (!schema)
		return nullptr;

	auto& kits = schema->m_dictStickerKits;
	auto index = kits.Find(name);
	return index != kits.InvalidIndex() ? kits[index] : nullptr;
}

const CKeyChain* inventory::GetKeychain(int id) {
	auto schema = GetItemSchema();
	return schema ? FindInMap(schema->m_mapKeychains, id) : nullptr;
}

const CKeyChain* inventory::FindKeychain(const char* name) {
	auto schema = GetItemSchema();
	if (!schema)
		return nullptr;

	FOR_EACH_MAP_FAST(schema->m_mapKeychains, i) {
		CKeyChain* keychain = schema->m_mapKeychains.Element(i);
		if (V_strcmp(keychain->m_strName.Get(), name) == 0)
			return keychain;
	}
	return nullptr;
}

bool inventory::GiveDefaultWearables(CCSPlayerPawn* pawn, CCSPlayerController* controller, CCSPlayerInventory* inventory) {
	auto team = static_cast<CSTeam>(pawn->m_iTeamNum.Get());
	if ((team != CSTeam::T && team != CSTeam::CT) || pawn->IsBot() || (controller && controller->m_bControllingBot)) {
		ClearGloves(pawn);
		return true;
	}

	SetAgentPatches(pawn, inventory, static_cast<int>(team));

	CEconItemView* source = inventory ? inventory->GetItemInLoadout(static_cast<int>(team), LOADOUT_SLOT_CLOTHING_HANDS) : nullptr;
	auto definition = source ? source->GetItemDefinition() : nullptr;
	if (!definition || definition->IsBaseItem()) {
		ClearGloves(pawn);
		return true;
	}

	auto gloves = pawn->m_EconGloves.Get();
	if (gloves->m_bInitialized != source->m_bInitialized.Get() || gloves->m_iItemID != source->m_iItemID.Get() || gloves->m_iItemDefinitionIndex != source->m_iItemDefinitionIndex.Get()) {
		++pawn->m_nEconGlovesChanged;
	}

	gloves->m_iItemDefinitionIndex = source->m_iItemDefinitionIndex.Get();
	gloves->m_iEntityQuality = source->m_iEntityQuality.Get();
	gloves->m_iEntityLevel = source->m_iEntityLevel.Get();
	gloves->m_iItemID = source->m_iItemID.Get();
	gloves->m_iItemIDHigh = source->m_iItemIDHigh.Get();
	gloves->m_iItemIDLow = source->m_iItemIDLow.Get();
	gloves->m_iAccountID = source->m_iAccountID.Get();
	gloves->m_iInventoryPosition = source->m_iInventoryPosition.Get();
	gloves->m_bInitialized = source->m_bInitialized.Get();
	SetGlovesAttributes(pawn, source);
	HideDefaultGloves(pawn, true);
	return true;
}

#endif

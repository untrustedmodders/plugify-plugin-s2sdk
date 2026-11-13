#include "inventory.hpp"

#if defined (CS2)

#include <array>
#include <bit>
#include <charconv>
#include <cstring>
#include <format>
#include <string>
#include <unordered_map>

#include <cstrike15_gcmessages.pb.h> // CAttribute_String
#include <econ/econ_item.h>
#include <tier0/keyvalues.h>
#include <tier0/memalloc.h>

#include "game_config.hpp"
#include "player_manager.hpp"

namespace {
	using Loadout = std::array<std::array<LoadoutSlot_t, LOADOUT_SLOT_COUNT>, LOADOUT_COUNT>;
	static_assert(sizeof(Loadout) == sizeof(CCSPlayerInventory::m_LoadoutItems));

	using CustomData = CEconItem::CustomDataOptimizedObject_t;

	std::unordered_map<CCSPlayerInventory*, Loadout> s_gameLoadouts;
	std::unordered_map<CCSPlayerInventory*, std::unordered_map<uint64, CEconItem*>> s_createdItems;

	// way below GC item ids and k_ulDefaultEquippedDefinitionID
	uint64 s_nextItemId = 0x1000000000000000ull;

	IEconItemSchema* GetSchema() {
		return g_ppItemSystem && *g_ppItemSystem ? (*g_ppItemSystem)->GetItemSchemaInterface() : nullptr;
	}

	// game allocates CEconItem bare: two vtables, rest zeroed, m_iItemSet = 255
	// freed by its virtual destructor through tier0
	std::pair<void*, void*> GetItemVTables() {
		static std::pair<void*, void*> vtables = [] {
			std::pair<void*, void*> result{};
			auto* server = g_GameConfigManager.GetModuleProvider().GetModule("server");
			for (const auto& info : server->FindVtablePartial("CEconItem")) {
				if (info.demangled_name == "CEconItem") {
					(info.offset == 0 ? result.first : result.second) = info.address;
				}
			}
			return result;
		}();
		return vtables;
	}

	struct SchemaSection {
		std::unordered_map<int, KeyValues*> entries;
		std::unordered_map<std::string, int, plg::string_hash, std::equal_to<>> names;
	};

	// paint kits, sticker kits and keychains have no schema interface, read them from raw items_game
	// one section there is split into many blocks with the same name
	const SchemaSection* GetSchemaSection(std::string_view section) {
		auto schema = GetSchema();
		KeyValues* root = schema ? schema->GetRawDefinition() : nullptr;
		if (!root)
			return nullptr;

		static KeyValues* s_root = nullptr;
		static std::unordered_map<std::string, SchemaSection, plg::string_hash, std::equal_to<>> s_sections;
		if (s_root != root) {
			s_root = root;
			s_sections.clear();
		}

		if (auto it = s_sections.find(section); it != s_sections.end())
			return &it->second;

		auto& result = s_sections[std::string(section)];
		for (KeyValues* block = root->GetFirstTrueSubKey(); block; block = block->GetNextTrueSubKey()) {
			if (section != block->GetName())
				continue;

			for (KeyValues* entry = block->GetFirstTrueSubKey(); entry; entry = entry->GetNextTrueSubKey()) {
				std::string_view key = entry->GetName();
				int id;
				if (std::from_chars(key.data(), key.data() + key.size(), id).ec != std::errc{})
					continue;

				result.entries.emplace(id, entry);
				if (const char* name = entry->GetString("name", nullptr)) {
					result.names.emplace(name, id);
				}
			}
		}
		return &result;
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

	// attributes sit right after the block header, so realloc it through tier0, game frees it there
	CEconItem::attribute_t* AddItemAttribute(CEconItem* item) {
		CustomData* block = item->m_pCustomDataOptimizedObject;
		uint32 count = block ? block->m_numAttributes : 0;
		if (count >= 0xFF)
			return nullptr;

		size_t size = sizeof(CustomData) + (count + 1) * sizeof(CEconItem::attribute_t);
		auto newBlock = static_cast<CustomData*>(MemAlloc_Alloc(size));
		std::memset(static_cast<void*>(newBlock), 0, size);
		if (block) {
			std::memcpy(static_cast<void*>(newBlock), block, sizeof(CustomData) + count * sizeof(CEconItem::attribute_t));
			MemAlloc_Free(block);
		} else {
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
		// false stops the iteration
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
	std::memcpy(&s_gameLoadouts[inventory], inventory->m_LoadoutItems, sizeof(Loadout));
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

	std::memcpy(inventory->m_LoadoutItems, &it->second, sizeof(Loadout));
	return true;
}

void inventory::Forget(CCSPlayerInventory* inventory) {
	// SO cache outlives the inventory on map change, take our items out of it
	if (auto it = s_createdItems.find(inventory); it != s_createdItems.end()) {
		for (auto [itemId, item] : it->second) {
			DestroyItem(inventory, item);
		}
		s_createdItems.erase(it);
	}
	s_gameLoadouts.erase(inventory);
}

uint64 inventory::CreateItem(CCSPlayerInventory* inventory, uint16 itemDefinition, int quality) {
	auto [vtable, interfaceVTable] = GetItemVTables();
	if (!GetItemDefinition(itemDefinition) || !inventory->m_pSOCache || !vtable || !interfaceVTable)
		return 0;

	auto item = static_cast<CEconItem*>(MemAlloc_Alloc(sizeof(CEconItem)));
	std::memset(static_cast<void*>(item), 0, sizeof(CEconItem));
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

	// AddObject doesn't notify listeners, GC calls SOCreated itself
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

	// slot with a removed item gives the base item, put back what the game equipped
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

// schema has no lookup by name, index names once per schema
const CEconItemAttributeDefinition* inventory::FindAttribute(std::string_view name) {
	auto schema = GetSchema();
	if (!schema)
		return nullptr;

	static IEconItemSchema* s_schema = nullptr;
	static std::unordered_map<std::string, int, plg::string_hash, std::equal_to<>> s_indices;
	if (s_schema != schema) {
		s_schema = schema;
		s_indices.clear();
		for (int i = 0; i <= 0xFFFF; ++i) {
			if (auto definition = schema->GetAttributeDefinitionInterface(i)) {
				s_indices.emplace(definition->GetDefinitionName(), i);
			}
		}
	}

	auto it = s_indices.find(name);
	return it != s_indices.end() ? static_cast<const CEconItemAttributeDefinition*>(schema->GetAttributeDefinitionInterface(it->second)) : nullptr;
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

	// item view asks its inventory item, or its own attribute lists if there is none
	AttributeReader reader(definition->GetDefinitionIndex());
	reinterpret_cast<const IEconItemInterface*>(item)->IterateAttributes(&reader);
	return std::move(reader.value);
}

// attribute type inits and parses the value, same as CS:GO CEconItem::SetDynamicAttributeValue
bool inventory::SetItemAttribute(CCSPlayerInventory* inventory, CEconItem* item, std::string_view name, const char* value) {
	auto definition = FindAttribute(name);
	if (!definition)
		return false;

	auto type = const_cast<ISchemaAttributeType*>(definition->GetAttributeType());
	if (auto attribute = FindItemAttribute(item, definition)) {
		if (!type->BConvertStringToEconAttributeValue(definition, value, &attribute->m_value))
			return false;
	} else {
		attribute_data_union_t data{};
		type->InitializeNewEconAttributeValue(&data);
		if (!type->BConvertStringToEconAttributeValue(definition, value, &data)) {
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

const char* inventory::GetSchemaValue(std::string_view section, int id, const char* key) {
	auto entries = GetSchemaSection(section);
	if (!entries)
		return nullptr;

	auto it = entries->entries.find(id);
	return it != entries->entries.end() ? it->second->GetString(key, nullptr) : nullptr;
}

int inventory::FindSchemaEntry(std::string_view section, std::string_view name) {
	auto entries = GetSchemaSection(section);
	if (!entries)
		return -1;

	auto it = entries->names.find(name);
	return it != entries->names.end() ? it->second : -1;
}

#endif

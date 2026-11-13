#include <core/inventory.hpp>
#include <core/player_manager.hpp>
#include <core/sdk/entity/ccsweaponbase.h>
#include <core/sdk/entity/services.h>
#include <core/sdk/helpers.hpp>
#include <cstrike15/cstrike15_item_schema.h>
#include <econ/econ_item_interface.h>

PLUGIFY_WARN_PUSH()

PLUGIFY_LINKAGE()

#if defined (CS2)

namespace {
	const IEconItemInterface* AsInterface(void* itemView) {
		return static_cast<const IEconItemInterface*>(itemView);
	}

	plg::string ToString(const char* value) {
		return value ? plg::string(value) : plg::string();
	}
} // namespace

/**
 * @brief Returns the name of an item definition, e.g. "weapon_ak47" or "weapon_knife_karambit".
 *
 * @param itemDefinition The item definition index.
 * @return The definition name, or an empty string.
 */
extern "C" PLUGIN_API plg::string GetItemDefinitionName(uint16_t itemDefinition) {
	auto definition = inventory::GetItemDefinition(itemDefinition);
	return definition ? ToString(definition->m_pszDefinitionName) : plg::string();
}

/**
 * @brief Finds an item definition by its name, e.g. "weapon_ak47", "studded_brokenfang_gloves" or "customplayer_tm_leet_varianta".
 *
 * @param name The definition name.
 * @return The item definition index, or 0.
 */
extern "C" PLUGIN_API uint16_t GetItemDefinitionByName(const plg::string& name) {
	auto definition = inventory::GetItemDefinitionByName(name.c_str());
	return definition ? definition->GetDefinitionIndex() : 0;
}

/**
 * @brief Returns the item class of an item definition, e.g. "weapon_knife" for every knife.
 *
 * @param itemDefinition The item definition index.
 * @return The item class, or an empty string.
 */
extern "C" PLUGIN_API plg::string GetItemDefinitionClassName(uint16_t itemDefinition) {
	auto definition = inventory::GetItemDefinition(itemDefinition);
	return definition ? ToString(definition->GetItemClassname()) : plg::string();
}

/**
 * @brief Returns the loadout slot that an item definition takes for a team.
 *
 * @param itemDefinition The item definition index.
 * @param team The team.
 * @return The loadout position, or Invalid.
 */
extern "C" PLUGIN_API loadout_slot_t GetItemDefinitionLoadoutSlot(uint16_t itemDefinition, CSTeam team) {
	auto definition = inventory::GetItemDefinition(itemDefinition);
	int index = static_cast<int>(team);
	if (!definition || index < 0 || index >= LOADOUT_COUNT)
		return LOADOUT_SLOT_INVALID;

	return static_cast<loadout_slot_t>(definition->m_iLoadoutSlots[index]);
}

/**
 * @brief Returns the only team that can use an item definition.
 *
 * @param itemDefinition The item definition index.
 * @return The team, or CSTeam::None when both teams or neither can use it.
 */
extern "C" PLUGIN_API CSTeam GetItemDefinitionTeam(uint16_t itemDefinition) {
	auto definition = inventory::GetItemDefinition(itemDefinition);
	return static_cast<CSTeam>(definition ? definition->GetUsedByTeam() : 0);
}

/**
 * @brief Returns how many stickers (or patches on an agent) an item definition takes.
 *
 * @param itemDefinition The item definition index.
 * @return The number of sticker slots.
 */
extern "C" PLUGIN_API int GetItemDefinitionStickerSlots(uint16_t itemDefinition) {
	auto definition = inventory::GetItemDefinition(itemDefinition);
	return definition ? definition->GetNumberOfStickerSlots() : 0;
}

/**
 * @brief Returns how many keychains an item definition takes.
 *
 * @param itemDefinition The item definition index.
 * @return The number of keychain slots.
 */
extern "C" PLUGIN_API int GetItemDefinitionKeychainSlots(uint16_t itemDefinition) {
	auto definition = inventory::GetItemDefinition(itemDefinition);
	return definition ? definition->GetNumberOfKeychainSlots() : 0;
}

/**
 * @brief Returns the inventory image of an item definition.
 *
 * @param itemDefinition The item definition index.
 * @return The image path, or an empty string.
 */
extern "C" PLUGIN_API plg::string GetItemDefinitionImage(uint16_t itemDefinition) {
	auto definition = inventory::GetItemDefinition(itemDefinition);
	return definition ? ToString(definition->GetInventoryImage(0)) : plg::string();
}

/**
 * @brief Returns the model of an item definition.
 *
 * @param itemDefinition The item definition index.
 * @param world True for the world model, false for the one the player holds.
 * @return The model path, or an empty string.
 */
extern "C" PLUGIN_API plg::string GetItemDefinitionModel(uint16_t itemDefinition, bool world) {
	auto definition = inventory::GetItemDefinition(itemDefinition);
	if (!definition)
		return {};

	return ToString(world ? definition->GetWorldDisplayModel() : definition->GetBasePlayerDisplayModel());
}

/**
 * @brief Returns the index of an attribute definition, e.g. 6 for "set item texture prefab".
 *
 * @param attribute The attribute definition name.
 * @return The attribute definition index, or -1.
 */
extern "C" PLUGIN_API int GetAttributeDefinitionIndex(const plg::string& attribute) {
	auto definition = inventory::FindAttribute(attribute);
	return definition ? definition->GetDefinitionIndex() : -1;
}

/**
 * @brief Returns the name of an attribute definition.
 *
 * @param attributeIndex The attribute definition index.
 * @return The attribute definition name, or an empty string.
 */
extern "C" PLUGIN_API plg::string GetAttributeDefinitionName(int attributeIndex) {
	auto definition = inventory::GetAttribute(attributeIndex);
	return definition ? ToString(definition->GetDefinitionName()) : plg::string();
}

/**
 * @brief Returns the schema name of a paint kit, e.g. "cu_medieval_dragon_awp".
 *
 * @param paintKit The paint kit index.
 * @return The name, or an empty string.
 */
extern "C" PLUGIN_API plg::string GetPaintKitName(int paintKit) {
	return ToString(inventory::GetSchemaValue("paint_kits", paintKit, "name"));
}

/**
 * @brief Returns the localization token of a paint kit's name, e.g. "#PaintKit_cu_medieval_dragon_awp_Tag" (Dragon Lore).
 *
 * The server has no translations; the client shows the text for a token sent to it.
 *
 * @param paintKit The paint kit index.
 * @return The token, or an empty string.
 */
extern "C" PLUGIN_API plg::string GetPaintKitLocName(int paintKit) {
	return ToString(inventory::GetSchemaValue("paint_kits", paintKit, "description_tag"));
}

/**
 * @brief Finds a paint kit by its schema name.
 *
 * @param name The schema name, e.g. "cu_medieval_dragon_awp".
 * @return The paint kit index, or -1.
 */
extern "C" PLUGIN_API int FindPaintKit(const plg::string& name) {
	return inventory::FindSchemaEntry("paint_kits", name);
}

/**
 * @brief Returns the schema name of a sticker kit, e.g. "kat2014_ibuypower_holo".
 *
 * @param stickerKit The sticker kit index.
 * @return The name, or an empty string.
 */
extern "C" PLUGIN_API plg::string GetStickerKitName(int stickerKit) {
	return ToString(inventory::GetSchemaValue("sticker_kits", stickerKit, "name"));
}

/**
 * @brief Returns the localization token of a sticker kit's name, e.g. "#StickerKit_kat2014_ibuypower_holo".
 *
 * The server has no translations; the client shows the text for a token sent to it.
 *
 * @param stickerKit The sticker kit index.
 * @return The token, or an empty string.
 */
extern "C" PLUGIN_API plg::string GetStickerKitLocName(int stickerKit) {
	return ToString(inventory::GetSchemaValue("sticker_kits", stickerKit, "item_name"));
}

/**
 * @brief Finds a sticker kit by its schema name.
 *
 * @param name The schema name, e.g. "kat2014_ibuypower_holo".
 * @return The sticker kit index, or -1.
 */
extern "C" PLUGIN_API int FindStickerKit(const plg::string& name) {
	return inventory::FindSchemaEntry("sticker_kits", name);
}

/**
 * @brief Returns the schema name of a keychain, e.g. "kc_missinglink_ava".
 *
 * @param keychain The keychain index.
 * @return The name, or an empty string.
 */
extern "C" PLUGIN_API plg::string GetKeychainName(int keychain) {
	return ToString(inventory::GetSchemaValue("keychain_definitions", keychain, "name"));
}

/**
 * @brief Returns the localization token of a keychain's name, e.g. "#keychain_kc_missinglink_ava".
 *
 * The server has no translations; the client shows the text for a token sent to it.
 *
 * @param keychain The keychain index.
 * @return The token, or an empty string.
 */
extern "C" PLUGIN_API plg::string GetKeychainLocName(int keychain) {
	return ToString(inventory::GetSchemaValue("keychain_definitions", keychain, "loc_name"));
}

/**
 * @brief Finds a keychain by its schema name.
 *
 * @param name The schema name, e.g. "kc_missinglink_ava".
 * @return The keychain index, or -1.
 */
extern "C" PLUGIN_API int FindKeychain(const plg::string& name) {
	return inventory::FindSchemaEntry("keychain_definitions", name);
}

/**
 * @brief Returns the item view (CEconItemView) of an item in the client's inventory.
 *
 * Valid while the item stays in the inventory.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemId The item ID.
 * @return A pointer to the item view, or nullptr.
 */
extern "C" PLUGIN_API void* GetClientItemView(int playerSlot, uint64_t itemId) {
	return inventory::GetItemView(playerSlot, itemId);
}

/**
 * @brief Returns the item view (CEconItemView) equipped in a loadout slot.
 *
 * An empty slot gives the default item of the slot. Valid while the inventory is loaded.
 *
 * @param playerSlot The index of the player's slot.
 * @param team The loadout team; music kits and flair use CSTeam::None.
 * @param loadoutSlot The loadout position.
 * @return A pointer to the item view, or nullptr.
 */
extern "C" PLUGIN_API void* GetClientLoadoutItemView(int playerSlot, CSTeam team, loadout_slot_t loadoutSlot) {
	return inventory::GetLoadoutItem(playerSlot, static_cast<int>(team), loadoutSlot);
}

/**
 * @brief Returns the item view (CEconItemView) of a weapon or another econ entity.
 *
 * Valid while the entity exists.
 *
 * @param entityHandle The handle of the entity.
 * @return A pointer to the item view, or nullptr.
 */
extern "C" PLUGIN_API void* GetEntityItemView(int entityHandle) {
	auto entity = helpers::GetEntity<CEconEntity>(entityHandle);
	if (!entity || !helpers::IsDerivedFrom(entity, "CEconEntity"))
		return nullptr;

	return &entity->m_AttributeManager->m_Item.Get();
}

/**
 * @brief Returns the item definition index of an item view.
 *
 * @param itemView A pointer to the item view.
 * @return The item definition index.
 */
extern "C" PLUGIN_API uint16_t GetItemViewDefinition(void* itemView) {
	return static_cast<uint16_t>(static_cast<CEconItemView*>(itemView)->m_iItemDefinitionIndex.Get());
}

/**
 * @brief Returns the item ID of an item view, 0 for a default item.
 *
 * @param itemView A pointer to the item view.
 * @return The item ID.
 */
extern "C" PLUGIN_API uint64_t GetItemViewId(void* itemView) {
	return AsInterface(itemView)->GetItemID();
}

/**
 * @brief Returns the Steam account ID of the owner of an item view.
 *
 * @param itemView A pointer to the item view.
 * @return The account ID.
 */
extern "C" PLUGIN_API uint32_t GetItemViewAccountId(void* itemView) {
	return AsInterface(itemView)->GetAccountID();
}

/**
 * @brief Returns the quality of an item view.
 *
 * @param itemView A pointer to the item view.
 * @return The quality.
 */
extern "C" PLUGIN_API EEconItemQuality GetItemViewQuality(void* itemView) {
	return static_cast<EEconItemQuality>(AsInterface(itemView)->GetQuality());
}

/**
 * @brief Returns the rarity of an item view.
 *
 * @param itemView A pointer to the item view.
 * @return The rarity.
 */
extern "C" PLUGIN_API int GetItemViewRarity(void* itemView) {
	return AsInterface(itemView)->GetRarity();
}

/**
 * @brief Returns how the item of an item view was obtained (eEconItemOrigin).
 *
 * @param itemView A pointer to the item view.
 * @return The origin.
 */
extern "C" PLUGIN_API eEconItemOrigin GetItemViewOrigin(void* itemView) {
	return AsInterface(itemView)->GetOrigin();
}

/**
 * @brief Returns whether the item of an item view can be traded.
 *
 * @param itemView A pointer to the item view.
 * @return True if it is tradable.
 */
extern "C" PLUGIN_API bool IsItemViewTradable(void* itemView) {
	return AsInterface(itemView)->IsTradable();
}

/**
 * @brief Returns whether the item of an item view can be sold on the market.
 *
 * @param itemView A pointer to the item view.
 * @return True if it is marketable.
 */
extern "C" PLUGIN_API bool IsItemViewMarketable(void* itemView) {
	return AsInterface(itemView)->IsMarketable();
}

/**
 * @brief Returns the paint kit of an item view.
 *
 * @param itemView A pointer to the item view.
 * @return The paint kit index, 0 without a paint.
 */
extern "C" PLUGIN_API int GetItemViewPaintKit(void* itemView) {
	return AsInterface(itemView)->GetCustomPaintKitIndex();
}

/**
 * @brief Returns the paint seed of an item view.
 *
 * @param itemView A pointer to the item view.
 * @return The seed.
 */
extern "C" PLUGIN_API int GetItemViewPaintKitSeed(void* itemView) {
	return AsInterface(itemView)->GetCustomPaintKitSeed();
}

/**
 * @brief Returns the paint wear of an item view.
 *
 * @param itemView A pointer to the item view.
 * @return The wear, 0 without a paint.
 */
extern "C" PLUGIN_API float GetItemViewPaintKitWear(void* itemView) {
	return AsInterface(itemView)->GetCustomPaintKitWear(0.0f);
}

/**
 * @brief Returns an attribute of a sticker on an item view.
 *
 * @param itemView A pointer to the item view.
 * @param stickerSlot The sticker slot.
 * @param attribute The sticker attribute.
 * @return The value, 0 if the slot is empty.
 */
extern "C" PLUGIN_API double GetItemViewStickerAttribute(void* itemView, int stickerSlot, EStickerAttributeType attribute) {
	if (attribute == k_EStickerAttribute_ID || attribute == k_EStickerAttribute_Schema)
		return AsInterface(itemView)->GetStickerAttributeBySlotIndexInt(stickerSlot, attribute, 0);

	return AsInterface(itemView)->GetStickerAttributeBySlotIndexFloat(stickerSlot, attribute, 0.0f);
}

/**
 * @brief Returns an attribute of a keychain on an item view.
 *
 * @param itemView A pointer to the item view.
 * @param keychainSlot The keychain slot.
 * @param attribute The keychain attribute.
 * @return The value, 0 if the slot is empty.
 */
extern "C" PLUGIN_API double GetItemViewKeychainAttribute(void* itemView, int keychainSlot, EStickerAttributeType attribute) {
	if (attribute == k_EStickerAttribute_ID || attribute == k_EStickerAttribute_Schema)
		return AsInterface(itemView)->GetKeychainAttributeBySlotIndexInt(keychainSlot, attribute, 0);

	return AsInterface(itemView)->GetKeychainAttributeBySlotIndexFloat(keychainSlot, attribute, 0.0f);
}

/**
 * @brief Returns any attribute of an item view as text, e.g. "kill eater" (StatTrak), "custom name attr"
 * or "sticker slot 0 id".
 *
 * @param itemView A pointer to the item view.
 * @param attribute The attribute definition name.
 * @return The value, or an empty string if the item has no such attribute.
 */
extern "C" PLUGIN_API plg::string GetItemViewAttribute(void* itemView, const plg::string& attribute) {
	auto value = inventory::GetItemAttribute(static_cast<const CEconItemView*>(itemView), attribute);
	return value ? plg::string(*value) : plg::string();
}

/**
 * @brief Gives the client a weapon made from an item of their inventory, with its paint, stickers and name.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemId The item ID.
 * @return The weapon's entity handle, or INVALID_EHANDLE_INDEX.
 */
extern "C" PLUGIN_API int GiveClientItem(int playerSlot, uint64_t itemId) {
	auto item = inventory::GetItemView(playerSlot, itemId);
	if (!item)
		return INVALID_EHANDLE_INDEX;

	auto definition = inventory::GetItemDefinition(static_cast<uint16_t>(item->m_iItemDefinitionIndex.Get()));
	auto services = helpers::GetService2<CCSPlayer_ItemServices>(playerSlot, &CPlayerPawn::m_pItemServices);
	if (!definition || !services)
		return INVALID_EHANDLE_INDEX;

	auto weapon = addresses::CCSPlayer_ItemServices_GiveNamedItem(services, definition->m_pszDefinitionName, 0, item, false, nullptr);
	return weapon ? weapon->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

#endif

PLUGIFY_WARN_POP()

#include <core/core_config.hpp>
#include <core/inventory.hpp>
#include <core/sdk/entity/ccsweaponbase.h>
#include <cstrike15/cstrike15_item_schema.h>

PLUGIFY_WARN_PUSH()

PLUGIFY_LINKAGE()

#if defined (CS2)

namespace {
	// GetItemInLoadout takes an item only into its slot group (or its own slot), otherwise gives the base item
	bool FitsLoadoutSlot(const CCStrike15ItemDefinition* definition, int loadoutSlot) {
		int itemSlot = definition->GetDefaultLoadoutSlot();
		for (auto [first, last] : {std::pair{LOADOUT_SLOT_SECONDARY1, LOADOUT_SLOT_SECONDARY4}, {LOADOUT_SLOT_SMG0, LOADOUT_SLOT_SMG4}, {LOADOUT_SLOT_RIFLE0, LOADOUT_SLOT_RIFLE4}}) {
			if (itemSlot >= first && itemSlot <= last)
				return loadoutSlot >= first && loadoutSlot <= last;
		}
		return itemSlot == loadoutSlot;
	}

	// stops at the first one that fails
	bool SetCreatedItemAttributes(int playerSlot, uint64_t itemId, std::initializer_list<std::pair<std::string, std::string>> attributes) {
		auto inv = inventory::Get(playerSlot);
		if (!inv)
			return false;

		auto item = inventory::FindCreatedItem(inv, itemId);
		if (!item)
			return false;

		for (const auto& [name, value] : attributes) {
			if (!inventory::SetItemAttribute(inv, item, name, value.c_str()))
				return false;
		}
		return true;
	}
} // namespace

/**
 * @brief Returns the client's game inventory (CCSPlayerInventory).
 *
 * Available once OnClientInventoryLoaded fires; bots have no inventory.
 *
 * @param playerSlot The index of the player's slot.
 * @return A pointer to the inventory, or nullptr if it is not loaded.
 */
extern "C" PLUGIN_API void* GetClientInventory(int playerSlot) {
	return inventory::Get(playerSlot);
}

/**
 * @brief Returns the item ID equipped in a loadout slot.
 *
 * An empty slot gives the default item of the slot, whose ID is 0.
 *
 * @param playerSlot The index of the player's slot.
 * @param team The loadout team; music kits and flair use CSTeam::None.
 * @param loadoutSlot The loadout position.
 * @return The item ID, or 0.
 */
extern "C" PLUGIN_API uint64_t GetClientLoadoutItemId(int playerSlot, CSTeam team, loadout_slot_t loadoutSlot) {
	auto item = inventory::GetLoadoutItem(playerSlot, static_cast<int>(team), loadoutSlot);
	if (!item)
		return 0;

	return item->m_iItemID.Get();
}

/**
 * @brief Returns the item definition index equipped in a loadout slot.
 *
 * An empty slot gives the default item of the slot.
 *
 * @param playerSlot The index of the player's slot.
 * @param team The loadout team; music kits and flair use CSTeam::None.
 * @param loadoutSlot The loadout position.
 * @return The item definition index, or 0.
 */
extern "C" PLUGIN_API uint16_t GetClientLoadoutItemDefinition(int playerSlot, CSTeam team, loadout_slot_t loadoutSlot) {
	auto item = inventory::GetLoadoutItem(playerSlot, static_cast<int>(team), loadoutSlot);
	if (!item)
		return 0;

	return static_cast<uint16_t>(item->m_iItemDefinitionIndex.Get());
}

/**
 * @brief Equips an item of the client's inventory in a loadout slot.
 *
 * The game reads the loadout when it gives items, so already given weapons keep their look.
 * The inventory update event is sent afterwards, as after an equip from the game coordinator.
 * The game rewrites the loadout when the inventory is loaded again or the player changes it.
 *
 * @param playerSlot The index of the player's slot.
 * @param team The loadout team; music kits and flair use CSTeam::None.
 * @param loadoutSlot The loadout position.
 * @param itemId The item ID, it must be in the client's inventory and belong to the slot or its group.
 * @return True if the item was equipped.
 */
extern "C" PLUGIN_API bool SetClientLoadoutItem(int playerSlot, CSTeam team, loadout_slot_t loadoutSlot, uint64_t itemId) {
	if (!inventory::IsLoadoutSlot(static_cast<int>(team), loadoutSlot))
		return false;

	auto inv = inventory::Get(playerSlot);
	if (!inv)
		return false;

	auto item = inventory::FindItemView(inv, itemId);
	if (!item)
		return false;

	auto definition = static_cast<item_definition_index_t>(item->m_iItemDefinitionIndex.Get());
	auto itemDefinition = inventory::GetItemDefinition(definition);
	if (!itemDefinition || !FitsLoadoutSlot(itemDefinition, loadoutSlot))
		return false;

	inv->SetItemInLoadout(static_cast<int>(team), loadoutSlot, itemId, definition);
	inv->SendInventoryUpdateEvent();
	return true;
}

/**
 * @brief Equips a stock item definition in a loadout slot without an owned item, e.g. picks USP-S over P2000.
 *
 * The definition must belong to the slot or its group. Works like SetClientLoadoutItem otherwise.
 *
 * @param playerSlot The index of the player's slot.
 * @param team The loadout team; music kits and flair use CSTeam::None.
 * @param loadoutSlot The loadout position.
 * @param itemDefinition The item definition index.
 * @return True if the definition was equipped.
 */
extern "C" PLUGIN_API bool SetClientLoadoutDefinition(int playerSlot, CSTeam team, loadout_slot_t loadoutSlot, uint16_t itemDefinition) {
	if (!inventory::IsLoadoutSlot(static_cast<int>(team), loadoutSlot))
		return false;

	auto inv = inventory::Get(playerSlot);
	if (!inv)
		return false;

	// without an item game takes only stock definitions
	auto definition = inventory::GetItemDefinition(itemDefinition);
	if (!definition || itemDefinition > 256 || !definition->IsBaseItem() || !FitsLoadoutSlot(definition, loadoutSlot))
		return false;

	inv->SetItemInLoadout(static_cast<int>(team), loadoutSlot, 0, itemDefinition);
	inv->SendInventoryUpdateEvent();
	return true;
}

/**
 * @brief Finds the loadout slot that gives an item definition to a team, e.g. which rifle slot holds the AK-47.
 *
 * Checks the slots the definition can take: its own slot, or every slot of its group for pistols, mid-tier weapons and rifles.
 * Equip into the returned slot to keep one item per weapon: with two items of one weapon the game picks one at random on every give.
 *
 * @param playerSlot The index of the player's slot.
 * @param team The loadout team.
 * @param itemDefinition The item definition index.
 * @return The slot whose equipped or stock item has the definition, or LoadoutSlot::Invalid.
 */
extern "C" PLUGIN_API loadout_slot_t FindClientLoadoutSlot(int playerSlot, CSTeam team, uint16_t itemDefinition) {
	auto definition = inventory::GetItemDefinition(itemDefinition);
	if (!definition)
		return LOADOUT_SLOT_INVALID;

	for (int slot = 0; slot < LOADOUT_SLOT_COUNT; ++slot) {
		if (!FitsLoadoutSlot(definition, slot))
			continue;

		auto item = inventory::GetLoadoutItem(playerSlot, static_cast<int>(team), slot);
		if (item && static_cast<uint16_t>(item->m_iItemDefinitionIndex.Get()) == itemDefinition)
			return static_cast<loadout_slot_t>(slot);
	}
	return LOADOUT_SLOT_INVALID;
}

/**
 * @brief Undoes the loadout changes made through s2sdk and puts back what the game equipped.
 *
 * @param playerSlot The index of the player's slot.
 * @return True if the loadout was restored.
 */
extern "C" PLUGIN_API bool RestoreClientLoadout(int playerSlot) {
	auto inv = inventory::Get(playerSlot);
	if (!inv)
		return false;

	if (!inventory::RestoreGameLoadout(inv))
		return false;

	inv->SendInventoryUpdateEvent();
	return true;
}

/**
 * @brief Returns the IDs of the items in the client's inventory.
 *
 * @param playerSlot The index of the player's slot.
 * @return The item IDs, empty if the inventory is not loaded.
 */
extern "C" PLUGIN_API plg::vector<uint64_t> GetClientInventoryItems(int playerSlot) {
	plg::vector<uint64_t> result;

	auto inv = inventory::Get(playerSlot);
	if (!inv)
		return result;

	result.reserve(static_cast<size_t>(inv->m_vecInventoryItems.Count()));
	for (CEconItemView* item : inv->m_vecInventoryItems) {
		if (item) {
			result.push_back(item->m_iItemID.Get());
		}
	}
	return result;
}

/**
 * @brief Creates an item that the client does not own and puts it into the client's inventory.
 *
 * The item lives until the client leaves, the map changes or the game coordinator sends the inventory again,
 * so create items again in OnClientInventoryLoaded. Equip it with SetClientLoadoutItem.
 * Refused while the "FollowCS2ServerGuidelines" option is enabled: Valve's server rules forbid
 * giving players items they do not own. Attributes can be set only on created items.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemDefinition The item definition index.
 * @param quality The item quality.
 * @return The new item ID, or 0 on failure.
 */
extern "C" PLUGIN_API uint64_t CreateClientItem(int playerSlot, uint16_t itemDefinition, EEconItemQuality quality) {
	if (g_pCoreConfig->FollowCS2ServerGuidelines) {
		plg::print(LS_WARNING, "Cannot create items with \"FollowCS2ServerGuidelines\" option enabled.\n");
		return 0;
	}

	auto inv = inventory::Get(playerSlot);
	if (!inv || quality < 0 || quality > 15)
		return 0;

	return inventory::CreateItem(inv, itemDefinition, quality);
}

/**
 * @brief Removes an item made by CreateClientItem from the client's inventory.
 *
 * Loadout slots that held the item get back what the game equipped there.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemId The item ID.
 * @return True if the item was removed.
 */
extern "C" PLUGIN_API bool RemoveClientItem(int playerSlot, uint64_t itemId) {
	auto inv = inventory::Get(playerSlot);
	if (!inv)
		return false;

	return inventory::RemoveItem(inv, itemId);
}

/**
 * @brief Sets an attribute of an item made by CreateClientItem, e.g. "set item texture prefab" (paint kit),
 * "set item texture seed", "set item texture wear", "kill eater" or "custom name attr".
 *
 * The value is text that the attribute's type parses, as in the item schema: "38", "0.01" or a name.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemId The item ID.
 * @param attribute The attribute definition name.
 * @param value The value.
 * @return True if the attribute was set.
 */
extern "C" PLUGIN_API bool SetClientItemAttribute(int playerSlot, uint64_t itemId, const plg::string& attribute, const plg::string& value) {
	return SetCreatedItemAttributes(playerSlot, itemId, {{std::string(attribute), std::string(value)}});
}

/**
 * @brief Sets the skin of an item made by CreateClientItem.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemId The item ID.
 * @param paintKit The paint kit index, e.g. 259 for Howl.
 * @param seed The pattern seed, 0 to 1000.
 * @param wear The wear, 0.0 (factory new) to 1.0.
 * @return True if the skin was set.
 */
extern "C" PLUGIN_API bool SetClientItemPaint(int playerSlot, uint64_t itemId, int paintKit, int seed, float wear) {
	return SetCreatedItemAttributes(playerSlot, itemId, {
		{"set item texture prefab", std::to_string(paintKit)},
		{"set item texture seed", std::to_string(seed)},
		{"set item texture wear", std::format("{}", wear)},
	});
}

/**
 * @brief Puts a sticker on an item made by CreateClientItem.
 *
 * The client draws stickers of a created item after a full update, see ForceClientFullUpdate.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemId The item ID.
 * @param stickerSlot The sticker slot, from 0 to GetItemDefinitionStickerSlots - 1.
 * @param stickerId The sticker kit index, e.g. 60 for iBUYPOWER (Holo) | Katowice 2014.
 * @param wear The sticker wear, 0.0 (new) to 1.0 (scraped off).
 * @return True if the sticker was set.
 */
extern "C" PLUGIN_API bool SetClientItemSticker(int playerSlot, uint64_t itemId, int stickerSlot, int stickerId, float wear) {
	if (stickerSlot < 0)
		return false;

	return SetCreatedItemAttributes(playerSlot, itemId, {
		{std::format("sticker slot {} id", stickerSlot), std::to_string(stickerId)},
		{std::format("sticker slot {} wear", stickerSlot), std::format("{}", wear)},
	});
}

/**
 * @brief Puts a keychain (charm) on an item made by CreateClientItem.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemId The item ID.
 * @param keychainSlot The keychain slot, from 0 to GetItemDefinitionKeychainSlots - 1.
 * @param keychainId The keychain definition index.
 * @param seed The keychain pattern seed.
 * @return True if the keychain was set.
 */
extern "C" PLUGIN_API bool SetClientItemKeychain(int playerSlot, uint64_t itemId, int keychainSlot, int keychainId, int seed) {
	if (keychainSlot < 0)
		return false;

	return SetCreatedItemAttributes(playerSlot, itemId, {
		{std::format("keychain slot {} id", keychainSlot), std::to_string(keychainId)},
		{std::format("keychain slot {} seed", keychainSlot), std::to_string(seed)},
	});
}

/**
 * @brief Makes an item made by CreateClientItem a StatTrak item with the given kill count.
 *
 * The game does not count kills for created items, the plugin counts them and calls this again.
 * Create the item with quality 9 to show the StatTrak name.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemId The item ID.
 * @param kills The kill count.
 * @return True if the counter was set.
 */
extern "C" PLUGIN_API bool SetClientItemStatTrak(int playerSlot, uint64_t itemId, int kills) {
	return SetCreatedItemAttributes(playerSlot, itemId, {
		{"kill eater score type", "0"},
		{"kill eater", std::to_string(kills)},
	});
}

/**
 * @brief Sets the name tag of an item made by CreateClientItem.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemId The item ID.
 * @param name The name.
 * @return True if the name was set.
 */
extern "C" PLUGIN_API bool SetClientItemName(int playerSlot, uint64_t itemId, const plg::string& name) {
	return SetCreatedItemAttributes(playerSlot, itemId, {{"custom name attr", std::string(name)}});
}

/**
 * @brief Returns an attribute of an item in the client's inventory as text, e.g. "kill eater" after a plugin counted kills.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemId The item ID.
 * @param attribute The attribute definition name.
 * @return The value, or an empty string if the item has no such attribute.
 */
extern "C" PLUGIN_API plg::string GetClientItemAttribute(int playerSlot, uint64_t itemId, const plg::string& attribute) {
	auto item = inventory::GetItemView(playerSlot, itemId);
	if (!item)
		return {};

	auto value = inventory::GetItemAttribute(item, attribute);
	return value ? plg::string(*value) : plg::string();
}

#endif

PLUGIFY_WARN_POP()

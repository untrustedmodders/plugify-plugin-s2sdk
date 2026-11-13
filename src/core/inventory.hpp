#pragma once

#if defined (CS2)

#include <cstrike15/cstrike15_item_inventory.h>

#include <optional>
#include <string>
#include <string_view>

class Player;
class CEconItem;
class CEconItemView;
class CEconItemAttributeDefinition;
class CCStrike15ItemDefinition;

namespace inventory {
	// only humans, nullptr until GC sends it
	CCSPlayerInventory* Get(int playerSlot);
	Player* GetOwner(CCSPlayerInventory* inventory);
	const CCStrike15ItemDefinition* GetItemDefinition(uint16 itemDefinition);
	const CCStrike15ItemDefinition* GetItemDefinitionByName(const char* name);

	// loadout written by the game, kept to undo our changes
	void SaveGameLoadout(CCSPlayerInventory* inventory);
	void SaveGameLoadoutSlot(CCSPlayerInventory* inventory, int team, int loadoutSlot);
	bool RestoreGameLoadout(CCSPlayerInventory* inventory);
	void Forget(CCSPlayerInventory* inventory);

	// created items live in SO cache, it outlives the inventory on map change, Forget removes them
	uint64 CreateItem(CCSPlayerInventory* inventory, uint16 itemDefinition, int quality);
	bool RemoveItem(CCSPlayerInventory* inventory, uint64 itemId);
	CEconItem* FindCreatedItem(CCSPlayerInventory* inventory, uint64 itemId);
	// drop items the game removed from the cache, e.g. after full update from GC
	void ValidateCreatedItems(CCSPlayerInventory* inventory);

	// values as text, attribute type parses and prints them
	bool SetItemAttribute(CCSPlayerInventory* inventory, CEconItem* item, std::string_view name, const char* value);
	std::optional<std::string> GetItemAttribute(const CEconItemView* item, std::string_view name);

	// items_game sections "paint_kits", "sticker_kits", "keychain_definitions"
	const char* GetSchemaValue(std::string_view section, int id, const char* key);
	int FindSchemaEntry(std::string_view section, std::string_view name);

	CEconItemView* FindItemView(CCSPlayerInventory* inventory, uint64 itemId);
	CEconItemView* GetItemView(int playerSlot, uint64 itemId);
	bool IsLoadoutSlot(int team, int loadoutSlot);
	// what the game gives for the slot: equipped item or slot default
	CEconItemView* GetLoadoutItem(int playerSlot, int team, int loadoutSlot);
	const CEconItemAttributeDefinition* FindAttribute(std::string_view name);
	const CEconItemAttributeDefinition* GetAttribute(int index);
} // namespace inventory

#endif

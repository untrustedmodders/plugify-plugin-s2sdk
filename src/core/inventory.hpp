#pragma once

#if defined (CS2)

#include <cstrike15/cstrike15_item_inventory.h>

#include <optional>
#include <string>
#include <string_view>

class Player;
class CCSPlayerPawn;
class CCSPlayerController;
class CEconItem;
class CEconItemView;
class CEconItemAttributeDefinition;
class CCStrike15ItemDefinition;

namespace inventory {
	CCSPlayerInventory* Get(int playerSlot);
	Player* GetOwner(CCSPlayerInventory* inventory);
	const CCStrike15ItemDefinition* GetItemDefinition(uint16 itemDefinition);
	const CCStrike15ItemDefinition* GetItemDefinitionByName(const char* name);

	void SaveGameLoadout(CCSPlayerInventory* inventory);
	void SaveGameLoadoutSlot(CCSPlayerInventory* inventory, int team, int loadoutSlot);
	bool RestoreGameLoadout(CCSPlayerInventory* inventory);
	void Forget(CCSPlayerInventory* inventory);

	void DropOwnCache(CCSPlayerInventory* inventory);

	uint64 CreateItem(CCSPlayerInventory* inventory, uint16 itemDefinition, int quality);
	bool RemoveItem(CCSPlayerInventory* inventory, uint64 itemId);
	CEconItem* FindCreatedItem(CCSPlayerInventory* inventory, uint64 itemId);
	void ValidateCreatedItems(CCSPlayerInventory* inventory);

	bool SetItemAttribute(CCSPlayerInventory* inventory, CEconItem* item, std::string_view name, std::string_view value);
	std::optional<std::string> GetItemAttribute(const CEconItemView* item, std::string_view name);

	const CPaintKit* GetPaintKit(int id);
	const CPaintKit* FindPaintKit(const char* name);
	const CStickerKit* GetStickerKit(int id);
	const CStickerKit* FindStickerKit(const char* name);
	const CKeyChain* GetKeychain(int id);
	const CKeyChain* FindKeychain(const char* name);

	bool GiveDefaultWearables(CCSPlayerPawn* pawn, CCSPlayerController* controller, CCSPlayerInventory* inventory);

	CEconItemView* FindItemView(CCSPlayerInventory* inventory, uint64 itemId);
	CEconItemView* GetItemView(int playerSlot, uint64 itemId);
	bool IsLoadoutSlot(int team, int loadoutSlot);
	CEconItemView* GetLoadoutItem(int playerSlot, int team, int loadoutSlot);
	const CEconItemAttributeDefinition* FindAttribute(std::string_view name);
	const CEconItemAttributeDefinition* GetAttribute(int index);
} // namespace inventory

#endif

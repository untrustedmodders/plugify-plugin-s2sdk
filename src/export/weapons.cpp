#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/entity/ccsweaponbase.h>
#include <core/sdk/helpers.hpp>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

/**
 * @brief Retrieves the weapon VData for a given weapon name.
 *
 * @param name The name of the weapon.
 * @return A pointer to the `CCSWeaponBaseVData` if found; otherwise, nullptr.
 */
extern "C" PLUGIN_API CCSWeaponBaseVData* GetWeaponVDataFromKey(const plg::string& name) {
	return addresses::GetCSWeaponDataFromKey(-1, name.c_str());
}

/**
 * @brief Retrieves the weapon VData for a given weapon.
 *
 * @param entityHandle The handle of the entity from which to retrieve the weapon VData.
 * @return A pointer to the `CCSWeaponBaseVData` if the entity handle is valid and
 *         represents a player weapon; otherwise, nullptr.
 */
extern "C" PLUGIN_API CCSWeaponBaseVData* GetWeaponVData(int entityHandle) {
	auto* weapon = helpers::GetEntity<CBasePlayerWeapon>(entityHandle);
	if (!weapon) return {};
	return weapon->GetWeaponVData();
}

/**
 * @brief Retrieves the weapon type of a given entity.
 *
 * @param entityHandle The handle of the entity (weapon).
 * @return CSWeaponType The type of the weapon, or WEAPONTYPE_UNKNOWN if the entity is invalid.
 */
extern "C" PLUGIN_API CSWeaponType GetWeaponType(int entityHandle) {
	auto* weapon = helpers::GetEntity<CBasePlayerWeapon>(entityHandle);
	if (!weapon) return {};
	return weapon->GetWeaponVData()->m_WeaponType;
}

/**
 * @brief Retrieves the weapon category of a given entity.
 *
 * @param entityHandle The handle of the entity (weapon).
 * @return CSWeaponCategory The category of the weapon, or WEAPONCATEGORY_OTHER if the entity is invalid.
 */
extern "C" PLUGIN_API CSWeaponCategory GetWeaponCategory(int entityHandle) {
	auto* weapon = helpers::GetEntity<CBasePlayerWeapon>(entityHandle);
	if (!weapon) return {};
	return weapon->GetWeaponVData()->m_WeaponCategory;
}

/**
 * @brief Retrieves the gear slot of a given weapon entity.
 *
 * @param entityHandle The handle of the entity (weapon).
 * @return gear_slot_t The gear slot of the weapon, or GEAR_SLOT_INVALID if the entity is invalid.
 */
extern "C" PLUGIN_API gear_slot_t GetWeaponGearSlot(int entityHandle) {
	auto* weapon = helpers::GetEntity<CBasePlayerWeapon>(entityHandle);
	if (!weapon) return {};
	return weapon->GetWeaponVData()->m_GearSlot;
}

/**
 * @brief Retrieves the weapon definition index for a given entity handle.
 *
 * @param entityHandle The handle of the entity from which to retrieve the weapon def index.
 * @return The weapon definition index as a `uint16_t`, or 0 if the entity handle is invalid.
 */
extern "C" PLUGIN_API CSWeaponDefIndex GetWeaponItemDefinition(int entityHandle) {
	auto* weapon = helpers::GetEntity<CBasePlayerWeapon>(entityHandle);
	if (!weapon) return {};
	return weapon->m_AttributeManager->m_Item->m_iItemDefinitionIndex;
}

/**
 * @brief Retrieves the item definition index associated with a given item name.
 *
 * @param name The name of the item.
 * @return The item definition index if found; otherwise, 0 or an invalid value.
 */
extern "C" PLUGIN_API CSWeaponDefIndex GetWeaponItemDefinitionByName(const plg::string& name) {
	auto itemSchema = reinterpret_cast<CEconItemSchema*>(g_pSource2Server->GetEconItemSystem());
	if (!itemSchema) return {};
	return static_cast<CSWeaponDefIndex>(addresses::GetItemDefinitionByName(itemSchema, name.c_str()));
}

PLUGIFY_WARN_POP()
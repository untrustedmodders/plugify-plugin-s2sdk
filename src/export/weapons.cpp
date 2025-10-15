#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/entity/ccsweaponbase.h>
#include <plugin_export.h>

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

/*extern "C" PLUGIN_API CCSWeaponBaseVData* GetItemDefinitionByName(const plg::string& name) {
	return addresses::GetItemDefinitionByName(name.c_str());
}*/

/**
 * @brief Retrieves the weapon VData for a given weapon.
 *
 * @param entityHandle The handle of the entity from which to retrieve the weapon VData.
 * @return A pointer to the `CCSWeaponBaseVData` if the entity handle is valid and
 *         represents a player weapon; otherwise, nullptr.
 */
extern "C" PLUGIN_API CCSWeaponBaseVData* GetWeaponVData(int entityHandle) {
	CBasePlayerWeapon* weapon = static_cast<CBasePlayerWeapon*>(g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) entityHandle)));
	if (!weapon) {
		plg::print(LS_WARNING, "Cannot execute 'GetWeaponVData' on invalid entity handle: {}\n", entityHandle);
		return nullptr;
	}

	return weapon->GetWeaponVData();
}

/**
 * @brief Retrieves the weapon type of a given entity.
 *
 * @param entityHandle The handle of the entity (weapon).
 * @return CSWeaponType The type of the weapon, or WEAPONTYPE_UNKNOWN if the entity is invalid.
 */
extern "C" PLUGIN_API CSWeaponType GetWeaponType(int entityHandle) {
	CBasePlayerWeapon* weapon = static_cast<CBasePlayerWeapon*>(g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) entityHandle)));
	if (!weapon) {
		plg::print(LS_WARNING, "Cannot execute 'GetWeaponType' on invalid entity handle: {}\n", entityHandle);
		return CSWeaponType::WEAPONTYPE_UNKNOWN;
	}

	return weapon->GetWeaponVData()->m_WeaponType;
}

/**
 * @brief Retrieves the weapon category of a given entity.
 *
 * @param entityHandle The handle of the entity (weapon).
 * @return CSWeaponCategory The category of the weapon, or WEAPONCATEGORY_OTHER if the entity is invalid.
 */
extern "C" PLUGIN_API CSWeaponCategory GetWeaponCategory(int entityHandle) {
	CBasePlayerWeapon* weapon = static_cast<CBasePlayerWeapon*>(g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) entityHandle)));
	if (!weapon) {
		plg::print(LS_WARNING, "Cannot execute 'GetWeaponCategory' on invalid entity handle: {}\n", entityHandle);
		return CSWeaponCategory::WEAPONCATEGORY_OTHER;
	}

	return weapon->GetWeaponVData()->m_WeaponCategory;
}

/**
 * @brief Retrieves the gear slot of a given weapon entity.
 *
 * @param entityHandle The handle of the entity (weapon).
 * @return gear_slot_t The gear slot of the weapon, or GEAR_SLOT_INVALID if the entity is invalid.
 */
extern "C" PLUGIN_API gear_slot_t GetWeaponGearSlot(int entityHandle) {
	CBasePlayerWeapon* weapon = static_cast<CBasePlayerWeapon*>(g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) entityHandle)));
	if (!weapon) {
		plg::print(LS_WARNING, "Cannot execute 'GetWeaponGearSlot' on invalid entity handle: {}\n", entityHandle);
		return gear_slot_t::GEAR_SLOT_INVALID;
	}

	return weapon->GetWeaponVData()->m_GearSlot;
}

/**
 * @brief Retrieves the weapon definition index for a given entity handle.
 *
 * @param entityHandle The handle of the entity from which to retrieve the weapon def index.
 * @return The weapon definition index as a `uint16_t`, or 0 if the entity handle is invalid.
 */
extern "C" PLUGIN_API uint16_t GetWeaponItemDefinition(int entityHandle) {
	CBasePlayerWeapon* weapon = static_cast<CBasePlayerWeapon*>(g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) entityHandle)));
	if (!weapon) {
		plg::print(LS_WARNING, "Cannot execute 'GetWeaponItemDefinition' on invalid entity handle: {}\n", entityHandle);
		return 0;
	}

	return weapon->m_AttributeManager().m_Item().m_iItemDefinitionIndex;
}

PLUGIFY_WARN_POP()
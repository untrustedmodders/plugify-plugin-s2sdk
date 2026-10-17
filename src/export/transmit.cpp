#include <iservernetworkable.h>
#include <playerslot.h>
#include <core/sdk/utils.hpp>
#include <core/sdk/helpers.hpp>
#include <core/transmit_manager.hpp>

PLUGIFY_WARN_PUSH()

PLUGIFY_LINKAGE()

// ============================================================================
// CCheckTransmitInfo - BitVec Entity Transmit Methods
// ============================================================================

/**
 * @brief Sets a bit in the TransmitEntity bitvec, marking an entity as transmittable.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The handle of the entity to mark as transmittable.
 */
extern "C" PLUGIN_API void SetTransmitInfoEntity(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pTransmitEntity) {
		return;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	info->m_pTransmitEntity->Set(entity->entindex());
}

/**
 * @brief Clears a bit in the TransmitEntity bitvec, marking an entity as not transmittable.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The handle of the entity to mark as not transmittable.
 */
extern "C" PLUGIN_API void ClearTransmitInfoEntity(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pTransmitEntity) {
		return;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	info->m_pTransmitEntity->Clear(entity->entindex());
}

/**
 * @brief Checks if a bit is set in the TransmitEntity bitvec.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The handle of the entity to check.
 * @return True if the entity is marked as transmittable, false otherwise.
 */
extern "C" PLUGIN_API bool IsTransmitInfoEntitySet(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pTransmitEntity) {
		return false;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return false;
	return info->m_pTransmitEntity->IsBitSet(entity->entindex());
}

/**
 * @brief Sets all bits in the TransmitEntity bitvec, marking all entities as transmittable.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void SetTransmitInfoEntityAll(CCheckTransmitInfo* info) {
	if (!info || !info->m_pTransmitEntity) {
		return;
	}
	info->m_pTransmitEntity->SetAll();
}

/**
 * @brief Clears all bits in the TransmitEntity bitvec, marking all entities as not transmittable.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void ClearTransmitInfoEntityAll(CCheckTransmitInfo* info) {
	if (!info || !info->m_pTransmitEntity) {
		return;
	}
	info->m_pTransmitEntity->ClearAll();
}

// ============================================================================
// CCheckTransmitInfo - BitVec NonPlayer Transmit Methods
// ============================================================================

/**
 * @brief Sets a bit in the TransmitNonPlayers bitvec, marking a non-player entity as transmittable.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The index of the non-player entity to mark as transmittable.
 */
extern "C" PLUGIN_API void SetTransmitInfoNonPlayer(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pNonTransmitEntity) {
		return;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	info->m_pNonTransmitEntity->Set(entity->entindex());
}

/**
 * @brief Clears a bit in the TransmitNonPlayers bitvec, marking a non-player entity as not transmittable.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The index of the non-player entity to mark as not transmittable.
 */
extern "C" PLUGIN_API void ClearTransmitInfoNonPlayer(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pNonTransmitEntity) {
		return;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	info->m_pNonTransmitEntity->Clear(entity->entindex());
}

/**
 * @brief Checks if a bit is set in the TransmitNonPlayers bitvec.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The index of the non-player entity to check.
 * @return True if the entity is marked as transmittable, false otherwise.
 */
extern "C" PLUGIN_API bool IsTransmitInfoNonPlayerSet(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pNonTransmitEntity) {
		return false;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return false;
	return info->m_pNonTransmitEntity->IsBitSet(entity->entindex());
}

/**
 * @brief Sets all bits in the TransmitNonPlayers bitvec, marking all non-player entities as transmittable.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void SetTransmitInfoNonPlayerAll(CCheckTransmitInfo* info) {
	if (!info || !info->m_pNonTransmitEntity) {
		return;
	}
	info->m_pNonTransmitEntity->SetAll();
}

/**
 * @brief Clears all bits in the TransmitNonPlayers bitvec, marking all non-player entities as not transmittable.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void ClearTransmitInfoNonPlayerAll(CCheckTransmitInfo* info) {
	if (!info || !info->m_pNonTransmitEntity) {
		return;
	}
	info->m_pNonTransmitEntity->ClearAll();
}

// ============================================================================
// CCheckTransmitInfo - BitVec OutOfPVS Methods
// ============================================================================

/**
 * @brief Sets a bit in the TransmitOutOfPVS bitvec, marking an entity to always transmit.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The handle of the entity to mark as always transmittable.
 */
extern "C" PLUGIN_API void SetTransmitInfoOutOfPVS(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pTransmitOutOfPVS) {
		return;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	info->m_pTransmitOutOfPVS->Set(entity->entindex());
}

/**
 * @brief Clears a bit in the TransmitOutOfPVS bitvec, unmarking an entity from always transmit.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The handle of the entity to unmark from always transmit.
 */
extern "C" PLUGIN_API void ClearTransmitInfoOutOfPVS(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pTransmitOutOfPVS) {
		return;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	info->m_pTransmitOutOfPVS->Clear(entity->entindex());
}

/**
 * @brief Checks if a bit is set in the TransmitOutOfPVS bitvec.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The handle of the entity to check.
 * @return True if the entity is marked to always transmit, false otherwise.
 */
extern "C" PLUGIN_API bool IsTransmitInfoOutOfPVSSet(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pTransmitOutOfPVS) {
		return false;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return false;
	return info->m_pTransmitOutOfPVS->IsBitSet(entity->entindex());
}

/**
 * @brief Sets all bits in the TransmitOutOfPVS bitvec, marking all entities to always transmit.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void SetTransmitInfoOutOfPVSAll(CCheckTransmitInfo* info) {
	if (!info || !info->m_pTransmitOutOfPVS) {
		return;
	}
	info->m_pTransmitOutOfPVS->SetAll();
}

/**
 * @brief Clears all bits in the TransmitOutOfPVS bitvec, unmarking all entities from always transmit.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void ClearTransmitInfoOutOfPVSAll(CCheckTransmitInfo* info) {
	if (!info || !info->m_pTransmitOutOfPVS) {
		return;
	}
	info->m_pTransmitOutOfPVS->ClearAll();
}

// ============================================================================
// CCheckTransmitInfo - BitVec Always Methods
// ============================================================================

/**
 * @brief Sets a bit in the TransmitAlways bitvec, marking an entity to always transmit.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The handle of the entity to mark as always transmittable.
 */
extern "C" PLUGIN_API void SetTransmitInfoAlways(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pTransmitAlways) {
		return;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	info->m_pTransmitAlways->Set(entity->entindex());
}

/**
 * @brief Clears a bit in the TransmitAlways bitvec, unmarking an entity from always transmit.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The handle of the entity to unmark from always transmit.
 */
extern "C" PLUGIN_API void ClearTransmitInfoAlways(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pTransmitAlways) {
		return;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	info->m_pTransmitAlways->Clear(entity->entindex());
}

/**
 * @brief Checks if a bit is set in the TransmitAlways bitvec.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The handle of the entity to check.
 * @return True if the entity is marked to always transmit, false otherwise.
 */
extern "C" PLUGIN_API bool IsTransmitInfoAlwaysSet(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pTransmitAlways) {
		return false;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return false;
	return info->m_pTransmitAlways->IsBitSet(entity->entindex());
}

/**
 * @brief Sets all bits in the TransmitAlways bitvec, marking all entities to always transmit.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void SetTransmitInfoAlwaysAll(CCheckTransmitInfo* info) {
	if (!info || !info->m_pTransmitAlways) {
		return;
	}
	info->m_pTransmitAlways->SetAll();
}

/**
 * @brief Clears all bits in the TransmitAlways bitvec, unmarking all entities from always transmit.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void ClearTransmitInfoAlwaysAll(CCheckTransmitInfo* info) {
	if (!info || !info->m_pTransmitAlways) {
		return;
	}
	info->m_pTransmitAlways->ClearAll();
}

// ============================================================================
// CCheckTransmitInfo - TargetSlots Vector Methods
// ============================================================================

/**
 * @brief Gets the count of spawn groups loaded by the client.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @return The number of loaded spawn groups, or 0 if the info pointer is null.
 */
extern "C" PLUGIN_API int GetTransmitInfoTargetSpawnGroupsCount(CCheckTransmitInfo* info) {
	if (!info) {
		return 0;
	}
	return info->m_LoadedSpawnGroups.Count();
}

/**
 * @brief Gets a spawn group handle at a specific index in the loaded spawn groups vector.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param index The index in the loaded spawn groups vector.
 * @return The spawn group handle, or -1 if the index is invalid or info is null.
 */
extern "C" PLUGIN_API int GetTransmitInfoTargetSpawnGroup(CCheckTransmitInfo* info, int index) {
	if (!info || index < 0 || index >= info->m_LoadedSpawnGroups.Count()) {
		return INVALID_PLAYER_SLOT_INDEX;
	}
	return info->m_LoadedSpawnGroups[index];
}

/**
 * @brief Adds a spawn group handle to the loaded spawn groups vector.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param spawnGroup The spawn group handle to add.
 */
extern "C" PLUGIN_API void AddTransmitInfoTargetSpawnGroup(CCheckTransmitInfo* info, int spawnGroup) {
	if (!info) {
		return;
	}
	info->m_LoadedSpawnGroups.AddToTail(spawnGroup);
}

/**
 * @brief Removes a spawn group handle from the loaded spawn groups vector.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param index Index within the loaded spawn groups vector to remove.
 */
extern "C" PLUGIN_API void RemoveTransmitInfoTargetSpawnGroup(CCheckTransmitInfo* info, int index) {
	if (!info) {
		return;
	}
	info->m_LoadedSpawnGroups.FastRemove(index);
}

/**
 * @brief Gets the loaded spawn groups vector.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @return The spawn group handles array.
 */
extern "C" PLUGIN_API plg::vector<int> GetTransmitInfoTargetSpawnGroupsAll(CCheckTransmitInfo* info) {
	if (!info) {
		return {};
	}
	return plg::vector<int>(info->m_LoadedSpawnGroups.begin(), info->m_LoadedSpawnGroups.end());
}

/**
 * @brief Clears all spawn groups from the loaded spawn groups vector.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void RemoveTransmitInfoTargetSpawnGroupsAll(CCheckTransmitInfo* info) {
	if (!info) {
		return;
	}
	info->m_LoadedSpawnGroups.RemoveAll();
}

// ============================================================================
// CCheckTransmitInfo - PlayerSlot Methods
// ============================================================================

/**
 * @brief Gets the player slot value from the CCheckTransmitInfo.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @return The player slot value, or -1 if info is null.
 */
extern "C" PLUGIN_API int GetTransmitInfoPlayerSlot(CCheckTransmitInfo* info) {
	if (!info) {
		return INVALID_PLAYER_SLOT_INDEX;
	}
	return info->m_nPlayerSlot;
}

/**
 * @brief Sets the player slot value in the CCheckTransmitInfo.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param playerSlot The player slot value to set.
 */
extern "C" PLUGIN_API void SetTransmitInfoPlayerSlot(CCheckTransmitInfo* info, int playerSlot) {
	if (!info) {
		return;
	}
	info->m_nPlayerSlot = playerSlot;
}

// ============================================================================
// CCheckTransmitInfo - FullUpdate Methods
// ============================================================================

/**
 * @brief Gets the full update flag from the CCheckTransmitInfo.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @return True if full update is enabled, false otherwise.
 */
extern "C" PLUGIN_API bool GetTransmitInfoFullUpdate(CCheckTransmitInfo* info) {
	if (!info) {
		return false;
	}
	return info->m_bFullUpdate;
}

/**
 * @brief Sets the full update flag in the CCheckTransmitInfo.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param fullUpdate The full update flag value to set.
 */
extern "C" PLUGIN_API void SetTransmitInfoFullUpdate(CCheckTransmitInfo* info, bool fullUpdate) {
	if (!info) {
		return;
	}
	info->m_bFullUpdate = fullUpdate;
}

// ============================================================================
// Transmit Manager Methods
// ============================================================================

/**
 * @brief Hides entities from a player's transmit list.
 *
 * @param playerSlot The player slot to hide entities from.
 * @param entHandles Entity handles to hide.
 */
extern "C" PLUGIN_API void HideTransmitEntities(int playerSlot, const plg::vector<int>& entHandles) {
	g_TransmitManager.HideEntities(playerSlot, entHandles);
}

/**
 * @brief Shows previously hidden entities to a player.
 *
 * @param playerSlot The player slot to show entities to.
 * @param entHandles Entity handles to show.
 */
extern "C" PLUGIN_API void ShowTransmitEntities(int playerSlot, const plg::vector<int>& entHandles) {
	g_TransmitManager.ShowEntities(playerSlot, entHandles);
}

/**
 * @brief Gets all hidden entity handles for a player.
 *
 * @param playerSlot The player slot to query.
 * @return Array of hidden entity handles.
 */
extern "C" PLUGIN_API plg::vector<int> GetHiddenTransmitEntities(int playerSlot) {
	return g_TransmitManager.GetHiddenEntities(playerSlot);
}

/**
 * @brief Hides a single entity from a player's transmit list.
 *
 * @param playerSlot The player slot to hide the entity from.
 * @param entityHandle Entity handle to hide.
 */
extern "C" PLUGIN_API void HideTransmitEntity(int playerSlot, int entityHandle) {
	const int32_t handle = entityHandle;
	g_TransmitManager.HideEntities(playerSlot, std::span<const int32_t>(&handle, 1));
}

/**
 * @brief Shows a previously hidden entity to a player.
 *
 * @param playerSlot The player slot to show the entity to.
 * @param entityHandle Entity handle to show.
 */
extern "C" PLUGIN_API void ShowTransmitEntity(int playerSlot, int entityHandle) {
	const int32_t handle = entityHandle;
	g_TransmitManager.ShowEntities(playerSlot, std::span<const int32_t>(&handle, 1));
}

/**
 * @brief Hides an entity from all players except the owner.
 *
 * @param playerSlot The owner player slot who will still see the entity.
 * @param entityHandle Entity handle to hide from other players.
 */
extern "C" PLUGIN_API void HideTransmitEntityFromOtherPlayers(int playerSlot, int entityHandle) {
	g_TransmitManager.HideEntityFromOtherPlayers(playerSlot, entityHandle);
}

/**
 * @brief Shows a previously hidden entity to all players except the owner.
 *
 * @param playerSlot The owner player slot who was excluded from hiding.
 * @param entityHandle Entity handle to show to other players.
 */
extern "C" PLUGIN_API void ShowTransmitEntityToOtherPlayers(int playerSlot, int entityHandle) {
	g_TransmitManager.ShowEntityToOtherPlayers(playerSlot, entityHandle);
}

PLUGIFY_WARN_POP()
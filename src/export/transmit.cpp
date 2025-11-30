#include <iservernetworkable.h>
#include <playerslot.h>
#include <core/sdk/utils.hpp>
#include <core/sdk/helpers.hpp>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

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
	if (!info || !info->m_pTransmitNonPlayers) {
		return;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	info->m_pTransmitNonPlayers->Set(entity->entindex());
}

/**
 * @brief Clears a bit in the TransmitNonPlayers bitvec, marking a non-player entity as not transmittable.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The index of the non-player entity to mark as not transmittable.
 */
extern "C" PLUGIN_API void ClearTransmitInfoNonPlayer(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pTransmitNonPlayers) {
		return;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	info->m_pTransmitNonPlayers->Clear(entity->entindex());
}

/**
 * @brief Checks if a bit is set in the TransmitNonPlayers bitvec.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param entityHandle The index of the non-player entity to check.
 * @return True if the entity is marked as transmittable, false otherwise.
 */
extern "C" PLUGIN_API bool IsTransmitInfoNonPlayerSet(CCheckTransmitInfo* info, int entityHandle) {
	if (!info || !info->m_pTransmitNonPlayers) {
		return false;
	}
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return false;
	return info->m_pTransmitNonPlayers->IsBitSet(entity->entindex());
}

/**
 * @brief Sets all bits in the TransmitNonPlayers bitvec, marking all non-player entities as transmittable.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void SetTransmitInfoNonPlayerAll(CCheckTransmitInfo* info) {
	if (!info || !info->m_pTransmitNonPlayers) {
		return;
	}
	info->m_pTransmitNonPlayers->SetAll();
}

/**
 * @brief Clears all bits in the TransmitNonPlayers bitvec, marking all non-player entities as not transmittable.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void ClearTransmitInfoNonPlayerAll(CCheckTransmitInfo* info) {
	if (!info || !info->m_pTransmitNonPlayers) {
		return;
	}
	info->m_pTransmitNonPlayers->ClearAll();
}

// ============================================================================
// CCheckTransmitInfo - BitVec TransmitAlways Methods
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
 * @brief Gets the count of target player slots.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @return The number of target player slots, or 0 if the info pointer is null.
 */
extern "C" PLUGIN_API int GetTransmitInfoTargetSlotsCount(CCheckTransmitInfo* info) {
	if (!info) {
		return 0;
	}
	return info->m_vecTargetSlots.Count();
}

/**
 * @brief Gets a player slot value at a specific index in the target slots vector.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param index The index in the target slots vector.
 * @return The player slot value, or -1 if the index is invalid or info is null.
 */
extern "C" PLUGIN_API int GetTransmitInfoTargetSlot(CCheckTransmitInfo* info, int index) {
	if (!info || index < 0 || index >= info->m_vecTargetSlots.Count()) {
		return INVALID_PLAYER_SLOT_INDEX;
	}
	return info->m_vecTargetSlots[index].Get();
}

/**
 * @brief Adds a player slot to the target slots vector.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param playerSlot The player slot value to add.
 */
extern "C" PLUGIN_API void AddTransmitInfoTargetSlot(CCheckTransmitInfo* info, int playerSlot) {
	if (!info) {
		return;
	}
	info->m_vecTargetSlots.AddToTail(playerSlot);
}

/**
 * @brief Removes a player slot from the target slots vector.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @param index Index within the target slots vector to remove.
 */
extern "C" PLUGIN_API void RemoveTransmitInfoTargetSlot(CCheckTransmitInfo* info, int index) {
	if (!info) {
		return;
	}
	info->m_vecTargetSlots.FastRemove(index);
}

/**
 * @brief Gets the target slots vector.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 * @return The player slots array.
 */
extern "C" PLUGIN_API plg::vector<int> GetTransmitInfoTargetSlotsAll(CCheckTransmitInfo* info) {
	if (!info) {
		return {};
	}
	return plg::vector<int>(info->m_vecTargetSlots.begin(), info->m_vecTargetSlots.end());
}

/**
 * @brief Clears all target player slots from the vector.
 *
 * @param info Pointer to the CCheckTransmitInfo structure.
 */
extern "C" PLUGIN_API void RemoveTransmitInfoTargetSlotsAll(CCheckTransmitInfo* info) {
	if (!info) {
		return;
	}
	info->m_vecTargetSlots.RemoveAll();
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

PLUGIFY_WARN_POP()
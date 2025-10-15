#include <core/player_manager.hpp>
#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/entity/cbaseplayercontroller.h>
#include <core/sdk/entity/cplayercontroller.h>
#include <core/sdk/utils.h>
#include <plg/plugin.hpp>
#include <plugin_export.h>

#include <core/sdk/entity/cplayerpawn.h>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

/**
 * @brief Retrieves the player slot from a given entity pointer.
 *
 * This function checks if the provided entity pointer is valid and returns
 * the corresponding player slot. If the entity pointer is invalid, it returns -1.
 *
 * @param entity A pointer to the entity (CBaseEntity*).
 * @return The player slot if valid, otherwise -1.
 */
extern "C" PLUGIN_API int EntPointerToPlayerSlot(CBaseEntity* entity) {
	if (!g_pGameEntitySystem->IsEntityPtr(entity)) {
		return -1;
	}

	return utils::GetEntityPlayerSlot(entity);
}

/**
 * @brief Returns a pointer to the entity instance by player slot index.
 *
 * This function retrieves a pointer to the entity instance associated with the specified player slot index.
 * If the client list is unavailable or the slot index is invalid, it returns nullptr.
 *
 * @param playerSlot Index of the player slot.
 * @return Pointer to the entity instance, or nullptr if the slot is invalid.
 */
extern "C" PLUGIN_API CEntityInstance* PlayerSlotToEntPointer(int playerSlot) {
	CUtlClientVector* clients = utils::GetClientList();
	if (!clients || !(0 <= playerSlot && playerSlot < clients->Count())) {
		return nullptr;
	}

	return g_pGameEntitySystem->GetEntityInstance(clients->Element(playerSlot)->GetEntityIndex());
}

/**
 * @brief Returns the entity handle associated with a player slot index.
 *
 * This function retrieves the entity handle corresponding to the specified
 * player slot index. If the client list is unavailable, the slot index is invalid,
 * or the entity cannot be resolved, it returns INVALID_EHANDLE_INDEX.
 *
 * @param playerSlot Index of the player slot.
 * @return The index of the entity, or -1 if the handle is invalid.
 */
extern "C" PLUGIN_API int PlayerSlotToEntHandle(int playerSlot) {
	CUtlClientVector* clients = utils::GetClientList();
	if (!clients || !(0 <= playerSlot && playerSlot < clients->Count())) {
		return INVALID_EHANDLE_INDEX;
	}

	CBaseEntity* entity = static_cast<CBaseEntity*>(g_pGameEntitySystem->GetEntityInstance(clients->Element(playerSlot)->GetEntityIndex()));
	if (!entity) {
		return INVALID_EHANDLE_INDEX;
	}

	return entity->GetRefEHandle().ToInt();
}

/**
 * @brief Retrieves the client object from a given player slot.
 *
 * This function fetches the client object associated with the specified index.
 * If the client list is unavailable, or the index is invalid, it returns nullptr.
 *
 * @param playerSlot The index of the player's slot (0-based).
 * @return A pointer to the client object if found, otherwise nullptr.
 */
extern "C" PLUGIN_API void* PlayerSlotToClientPtr(int playerSlot) {
	CUtlClientVector* clients = utils::GetClientList();
	if (!clients || !(0 <= playerSlot && playerSlot < clients->Count())) {
		return nullptr;
	}
	
	return clients->Element(playerSlot);
}

/**
 * @brief Retrieves the index of a given client object.
 *
 * This function finds the index of a specified client object within the client list.
 * If the client is not found or the list is unavailable, it returns -1.
 *
 * @param client A pointer to the client object (CServerSideClient*).
 * @return The player slot if found, otherwise -1.
 */
extern "C" PLUGIN_API int ClientPtrToPlayerSlot(CServerSideClient* client) {
	CUtlClientVector* clients = utils::GetClientList();
	if (!clients) {
		return -1;
	}
	
	if (clients->Find(client) != INVALID_PLAYER_SLOT) {
		return client->GetPlayerSlot();
	}

	return -1;
}

/**
 * @brief Returns the entity index for a given player slot.
 *
 * This function retrieves the entity index associated with the specified player slot.
 * If the client list is unavailable or the slot index is invalid, it returns 0.
 *
 * @param playerSlot The index of the player's slot.
 * @return The entity index if valid, otherwise 0.
 */
extern "C" PLUGIN_API int PlayerSlotToClientIndex(int playerSlot) {
	CUtlClientVector* clients = utils::GetClientList();
	if (!clients || !(0 <= playerSlot && playerSlot < clients->Count())) {
		return 0;
	}

	return clients->Element(playerSlot)->GetEntityIndex();
}

/**
 * @brief Retrieves the player slot from a given client index.
 *
 * This function converts a client index to the corresponding player slot.
 * If the controller is not found, it returns -1.
 *
 * @param clientIndex The index of the client.
 * @return The player slot if valid, otherwise -1.
 */
extern "C" PLUGIN_API int ClientIndexToPlayerSlot(int clientIndex) {
	auto controller = g_PlayerManager.ToPlayer(CEntityIndex(clientIndex));
	if (!controller) {
		return -1;
	}

	return controller->GetPlayerSlot();
}

/**
 * @brief Retrieves the player slot from a given player service.
 *
 * @param service The service pointer. Like CCSPlayer_ItemServices, CCSPlayer_WeaponServices ect.
 * @return The player slot if valid, otherwise -1.
 */
extern "C" PLUGIN_API int PlayerServicesToPlayerSlot(CPlayerPawnComponent* service) {
	auto pawn = service->GetPawn();
	if (!pawn) {
		return -1;
	}

	auto controller = pawn->GetController();
	if (!controller) {
		return -1;
	}

	return controller->GetPlayerSlot();
}

/**
 * @brief Retrieves a client's authentication string (SteamID).
 *
 * @param playerSlot The index of the player's slot whose authentication string is being retrieved.
 * @return The authentication string.
 */
extern "C" PLUGIN_API plg::string GetClientAuthId(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return {};
	}

	return pPlayer->GetSteamId().Render();
}

/**
 * @brief Returns the client's Steam account ID, a unique number identifying a given Steam account.
 *
 * @param playerSlot The index of the player's slot.
 * @return uint32_t The client's steam account ID.
 */
extern "C" PLUGIN_API uint32_t GetClientAccountId(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return 0;
	}

	return pPlayer->GetSteamId().GetAccountID();
}

/**
 * @brief Returns the client's SteamID64 — a unique 64-bit identifier of a Steam account.
 *
 * @param playerSlot The index of the player's slot.
 * @return uint64_t The client's SteamID64.
 */
extern "C" PLUGIN_API uint64_t GetClientSteamID64(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return 0;
	}

	return pPlayer->GetSteamId().ConvertToUint64();
}

/**
 * @brief Retrieves a client's IP address.
 *
 * @param playerSlot The index of the player's slot.
 * @return The client's IP address.
 */
extern "C" PLUGIN_API plg::string GetClientIp(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return {};
	}

	return pPlayer->GetIpAddress();
}

/**
 * @brief Retrieves a client's language.
 *
 * @param playerSlot The index of the player's slot.
 * @return The client's language.
 */
extern "C" PLUGIN_API plg::string GetClientLanguage(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return {};
	}

	return pPlayer->GetLanguage();
}

/**
 * @brief Retrieves a client's operating system.
 *
 * @param playerSlot The index of the player's slot.
 * @return The client's operating system.
 */
extern "C" PLUGIN_API plg::string GetClientOS(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return {};
	}

	return pPlayer->GetOperatingSystem();
}

/**
 * @brief Returns the client's name.
 *
 * @param playerSlot The index of the player's slot.
 * @return The client's name.
 */
extern "C" PLUGIN_API plg::string GetClientName(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return {};
	}

	return pPlayer->GetName();
}

/**
 * @brief Returns the client's connection time in seconds.
 *
 * @param playerSlot The index of the player's slot.
 * @return float Connection time in seconds.
 */
extern "C" PLUGIN_API float GetClientTime(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return -1.0f;
	}

	return pPlayer->GetTimeConnected();
}

/**
 * @brief Returns the client's current latency (RTT).
 *
 * @param playerSlot The index of the player's slot.
 * @return float Latency value.
 */
extern "C" PLUGIN_API float GetClientLatency(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return 0.0f;
	}

	return pPlayer->GetLatency();
}

/**
 * @brief Returns the client's access flags.
 *
 * @param playerSlot The index of the player's slot.
 * @return uint64 Access flags as a bitmask.
 */
extern "C" PLUGIN_API uint64 GetUserFlagBits(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return false;
	}

	return 0;//pPlayer->GetAdminFlags();
}

/**
 * @brief Sets the access flags on a client using a bitmask.
 *
 * @param playerSlot The index of the player's slot.
 * @param flags Bitmask representing the flags to be set.
 */
extern "C" PLUGIN_API void SetUserFlagBits(int playerSlot, uint64 flags) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return;
	}

	//pPlayer->SetAdminFlags(flags);
}

/**
 * @brief Adds access flags to a client.
 *
 * If the client is not an admin, they will be given a temporary AdminId.
 *
 * @param playerSlot The index of the player's slot.
 * @param flags Bitmask representing the flags to be added.
 */
extern "C" PLUGIN_API void AddUserFlags(int playerSlot, uint64 flags) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return;
	}

	//pPlayer->SetAdminFlags(pPlayer->GetAdminFlags() | flags);
}

/**
 * @brief Removes access flags from a client.
 *
 * If the client is not an admin, this has no effect.
 *
 * @param playerSlot The index of the player's slot.
 * @param flags Bitmask representing the flags to be removed.
 */
extern "C" PLUGIN_API void RemoveUserFlags(int playerSlot, uint64 flags) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return;
	}

	//pPlayer->SetAdminFlags(pPlayer->GetAdminFlags() & ~flags);
}

/**
 * @brief Checks if a certain player has been authenticated.
 *
 * @param playerSlot The index of the player's slot.
 * @return true if the player is authenticated, false otherwise.
 */
extern "C" PLUGIN_API bool IsClientAuthorized(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return false;
	}

	return pPlayer->IsAuthenticated();
}

/**
 * @brief Checks if a certain player is connected.
 *
 * @param playerSlot The index of the player's slot.
 * @return true if the player is connected, false otherwise.
 */
extern "C" PLUGIN_API bool IsClientConnected(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return false;
	}

	return pPlayer->IsConnected();
}

/**
 * @brief Checks if a certain player has entered the game.
 *
 * @param playerSlot The index of the player's slot.
 * @return true if the player is in the game, false otherwise.
 */
extern "C" PLUGIN_API bool IsClientInGame(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return false;
	}

	return pPlayer->IsInGame();
}

/**
 * @brief Checks if a certain player is the SourceTV bot.
 *
 * @param playerSlot The index of the player's slot.
 * @return true if the client is the SourceTV bot, false otherwise.
 */
extern "C" PLUGIN_API bool IsClientSourceTV(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return false;
	}

	return pPlayer->IsSourceTV();
}

/**
 * @brief Checks if the client is alive or dead.
 *
 * @param playerSlot The index of the player's slot.
 * @return true if the client is alive, false if dead.
 */
extern "C" PLUGIN_API bool IsClientAlive(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return false;
	}

	return pPlayer->IsAlive();
}

/**
 * @brief Checks if a certain player is a fake client.
 *
 * @param playerSlot The index of the player's slot.
 * @return true if the client is a fake client, false otherwise.
 */
extern "C" PLUGIN_API bool IsFakeClient(int playerSlot) {
	auto pPlayer = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (pPlayer == nullptr) {
		return false;
	}

	return pPlayer->IsFakeClient();
}

/////

/**
 * @brief Retrieves a client's team index.
 *
 * @param playerSlot The index of the player's slot.
 * @return The team index of the client.
 */
extern "C" PLUGIN_API int GetClientTeam(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetEntityHealth' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientTeam' on invalid player pawn: {}\n", playerSlot);
		return 0;
	}

	return pawn->m_iTeamNum;
}

/**
 * @brief Sets a client's team index.
 *
 * @param playerSlot The index of the player's slot.
 * @param team The team index to set.
 */
extern "C" PLUGIN_API void SetClientTeam(int playerSlot, int team) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientTeam' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientTeam' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	pawn->m_iTeamNum = team;
	controller->m_iTeamNum = team;
}

/**
 * @brief Returns the client's health.
 *
 * @param playerSlot The index of the player's slot.
 * @return The health value of the client.
 */
extern "C" PLUGIN_API int GetClientHealth(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientHealth' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientHealth' on invalid player pawn: {}\n", playerSlot);
		return 0;
	}

	return pawn->m_iHealth;
}

/**
 * @brief Sets the client's health.
 *
 * @param playerSlot The index of the player's slot.
 * @param health The health value to set.
 */
extern "C" PLUGIN_API void SetClientHealth(int playerSlot, int health) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientHealth' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientHealth' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	pawn->m_iHealth = health;
	controller->m_iHealth = health;
}

/**
 * @brief Returns the client's max health.
 *
 * @param playerSlot The index of the player's slot.
 * @return The max health value of the client.
 */
extern "C" PLUGIN_API int GetClientMaxHealth(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientMaxHealth' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientMaxHealth' on invalid player pawn: {}\n", playerSlot);
		return 0;
	}

	return pawn->m_iMaxHealth;
}

/**
 * @brief Sets the client's max health.
 *
 * @param playerSlot The index of the player's slot.
 * @param maxHealth The max health value to set.
 */
extern "C" PLUGIN_API void SetClientMaxHealth(int playerSlot, int maxHealth) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientMaxHealth' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientMaxHealth' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	pawn->m_iMaxHealth = maxHealth;
	controller->m_iMaxHealth = maxHealth;
}

/**
 * @brief Returns the client's speed value.
 *
 * @param playerSlot The index of the player's slot.
 * @return The speed value of the client.
 */
extern "C" PLUGIN_API float GetClientSpeed(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientSpeed' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientSpeed' on invalid player pawn: {}\n", playerSlot);
		return 0;
	}

	return pawn->m_flVelocityModifier;
}

/**
 * @brief Sets the client's speed value.
 *
 * @param playerSlot The index of the player's slot.
 * @param speed The speed value to set.
 */
extern "C" PLUGIN_API void SetClientSpeed(int playerSlot, float speed) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientSpeed' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientSpeed' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	pawn->m_flVelocityModifier = speed;
}

/**
 * @brief Returns the client's gravity value.
 *
 * @param playerSlot The index of the player's slot.
 * @return The gravity value of the client.
 */
extern "C" PLUGIN_API float GetClientGravity(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientGravity' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientGravity' on invalid player pawn: {}\n", playerSlot);
		return 0;
	}

	return pawn->m_flGravityScale;
}

/**
 * @brief Sets the client's gravity value.
 *
 * @param playerSlot The index of the player's slot.
 * @param gravity The gravity value to set.
 */
extern "C" PLUGIN_API void SetClientGravity(int playerSlot, float gravity) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientGravity' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientGravity' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	pawn->m_flGravityScale = gravity;
	pawn->m_bGravityDisabled = true;
	pawn->m_bGravityActuallyDisabled = true;
}

/**
 * @brief Returns the client's armor value.
 *
 * @param playerSlot The index of the player's slot.
 * @return The armor value of the client.
 */
extern "C" PLUGIN_API int GetClientArmor(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientArmor' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientArmor' on invalid player pawn: {}\n", playerSlot);
		return 0;
	}

	return pawn->m_ArmorValue;
}

/**
 * @brief Sets the client's armor value.
 *
 * @param playerSlot The index of the player's slot.
 * @param armor The armor value to set.
 */
extern "C" PLUGIN_API void SetClientArmor(int playerSlot, int armor) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientArmor' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientArmor' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	pawn->m_ArmorValue = armor;
}

/**
 * @brief Retrieves the client's origin vector.
 *
 * @param playerSlot The index of the player's slot.
 * @return A Vector where the client's origin will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientAbsOrigin(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientAbsOrigin' on invalid player slot: {}\n", playerSlot);
		return {};
	}
	
	auto pNode = controller->m_CBodyComponent->m_pSceneNode();
	if (!pNode) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientAbsOrigin' on invalid player node: {}\n", playerSlot);
		return {};
	}
	
	const Vector& vec = pNode->m_vecAbsOrigin;
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Sets the client's origin vector.
 *
 * @param playerSlot The index of the player's slot.
 * @param origin The new origin vector to set.
 */
extern "C" PLUGIN_API void SetClientAbsOrigin(int playerSlot, const plg::vec3& origin) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientAbsOrigin' on invalid player slot: {}\n", playerSlot);
		return;
	}
	
	auto pNode = controller->m_CBodyComponent->m_pSceneNode();
	if (!pNode) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientAbsAngles' on invalid player node: {}\n", playerSlot);
		return;
	}
	
	pNode->m_vecAbsOrigin = *reinterpret_cast<const Vector*>(&origin);
}

/**
 * @brief Retrieves the client's position angle.
 *
 * @param playerSlot The index of the player's slot.
 * @return A QAngle where the client's position angle will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientAbsAngles(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientAbsAngles' on invalid player slot: {}\n", playerSlot);
		return {};
	}
	
	auto pNode = controller->m_CBodyComponent->m_pSceneNode();
	if (!pNode) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientAbsAngles' on invalid player node: {}\n", playerSlot);
		return {};
	}
	
	const QAngle& ang = pNode->m_angRotation;
	return *reinterpret_cast<const plg::vec3*>(&ang);
}

/**
 * @brief Sets the client's absolute rotation angles.
 *
 * @param playerSlot The index of the player's slot.
 * @param angles The new rotation angles to set.
 */
extern "C" PLUGIN_API void SetClientAbsAngles(int playerSlot, const plg::vec3& angles) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientAbsAngles' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pNode = controller->m_CBodyComponent->m_pSceneNode();
	if (!pNode) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientAbsAngles' on invalid player node: {}\n", playerSlot);
		return;
	}
	
	pNode->m_angRotation = *reinterpret_cast<const QAngle*>(&angles);
}

/**
 * @brief Retrieves the absolute velocity of a client.
 *
 * This function gets the absolute velocity of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The handle of the client whose absolute velocity is to be retrieved.
 * @return A vector where the absolute velocity will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientAbsVelocity(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientAbsVelocity' on invalid player slot: {}\n", playerSlot);
		return {};
	}

	const Vector& vec = controller->m_vecAbsVelocity;
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Sets the absolute velocity of a client.
 *
 * This function updates the absolute velocity of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The handle of the client whose absolute velocity is to be set.
 * @param velocity The new absolute velocity to set for the client.
 */
extern "C" PLUGIN_API void SetClientAbsVelocity(int playerSlot, const Vector& velocity) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientAbsVelocity' on invalid player slot: {}\n", playerSlot);
		return;
	}

	controller->m_vecAbsVelocity = velocity;
}

/**
 * @brief Retrieves the client's eye angle.
 *
 * @param playerSlot The index of the player's slot.
 * @return A QAngle where the client's eye angle will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientEyeAngles(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientEyeAngles' on invalid player slot: {}\n", playerSlot);
		return {};
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientEyeAngles' on invalid player pawn: {}\n", playerSlot);
		return {};
	}

	const QAngle& ang = pawn->m_angEyeAngles;
	return *reinterpret_cast<const plg::vec3*>(&ang);
}

/**
 * @brief Sets the client's eye angles.
 *
 * @param playerSlot The index of the player's slot.
 * @param angles The new eye angles to set.
 */
extern "C" PLUGIN_API void SetClientEyeAngles(int playerSlot, const plg::vec3& angles) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientEyeAngles' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientEyeAngles' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	pawn->m_angEyeAngles = *reinterpret_cast<const QAngle*>(&angles);
}

/**
 * @brief Retrieves the pawn entity pointer associated with a client.
 *
 * @param playerSlot The index of the player's slot.
 * @return A pointer to the client's pawn entity, or nullptr if the client or controller is invalid.
 */
extern "C" PLUGIN_API void* GetClientPawn(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientPawn' on invalid player slot: {}\n", playerSlot);
		return nullptr;
	}

	return controller->GetCurrentPawn();
}

/**
 * @brief Processes the target string to determine if one user can target another.
 *
 * @param caller The index of the player's slot making the target request.
 * @param target The target string specifying the player or players to be targeted.
 * @return A vector where the result of the targeting operation will be stored.
 */
extern "C" PLUGIN_API plg::vector<int> ProcessTargetString(int caller, const plg::string& target) {
	plg::vector<int> output;
	g_PlayerManager.TargetPlayerString(caller, target, output);
	return output;
}

/**
 * @brief Changes a client's team.
 *
 * @param playerSlot The index of the player's slot.
 * @param team The team index to assign the client to.
 */
extern "C" PLUGIN_API void ChangeClientTeam(int playerSlot, int team) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'ChangeClientTeam' on invalid player slot: {}\n", playerSlot);
		return;
	}

	controller->ChangeTeam(team);
}

/**
 * @brief Switches the player's team.
 *
 * @param playerSlot The index of the player's slot.
 * @param team The team index to switch the client to.
 */
extern "C" PLUGIN_API void SwitchClientTeam(int playerSlot, int team) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SwitchClientTeam' on invalid player slot: {}\n", playerSlot);
		return;
	}

	controller->UpdateTeam(team);
}

/**
 * @brief Respawns a player.
 *
 * @param playerSlot The index of the player's slot to respawn.
 */
extern "C" PLUGIN_API void RespawnClient(int playerSlot) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'RespawnClient' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'RespawnClient' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	if (pawn->IsAlive()) {
		// TODO: Fix players spawning under spawn positions
		pawn->Respawn();
	} else {
		static_cast<CPlayerController*>(controller)->Respawn();
	}
}

/**
 * @brief Forces a player to commit suicide.
 *
 * @param playerSlot The index of the player's slot.
 * @param explode If true, the client will explode upon death.
 * @param force If true, the suicide will be forced.
 */
extern "C" PLUGIN_API void ForcePlayerSuicide(int playerSlot, bool explode, bool force) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'ForcePlayerSuicide' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'ForcePlayerSuicide' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	pawn->CommitSuicide(explode, force);
}

/**
 * @brief Disconnects a client from the server as soon as the next frame starts.
 *
 * @param playerSlot The index of the player's slot to be kicked.
 */
extern "C" PLUGIN_API void KickClient(int playerSlot) {
	g_pEngineServer->DisconnectClient(playerSlot, NETWORK_DISCONNECT_KICKED);
}

/**
 * @brief Bans a client for a specified duration.
 *
 * @param playerSlot The index of the player's slot to be banned.
 * @param duration Duration of the ban in seconds.
 * @param kick If true, the client will be kicked immediately after being banned.
 */
extern "C" PLUGIN_API void BanClient(int playerSlot, float duration, bool kick) {
	g_pEngineServer->BanClient(CPlayerSlot(playerSlot), duration, kick);
}

/**
 * @brief Bans an identity (either an IP address or a Steam authentication string).
 *
 * @param steamId The Steam ID to ban.
 * @param duration Duration of the ban in seconds.
 * @param kick If true, the client will be kicked immediately after being banned.
 */
extern "C" PLUGIN_API void BanIdentity(uint64_t steamId, float duration, bool kick) {
	g_pEngineServer->BanClient(CSteamID(static_cast<uint64>(steamId)), duration, kick);
}

/**
 * @brief Retrieves the model name of a client.
 *
 * This function gets the model name of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param pplayerSlot The index of the player's slot.
 * @return A string where the model name will be stored.
 */
extern "C" PLUGIN_API plg::string GetClientModel(int playerSlot) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientModel' on invalid player slot: {}\n", playerSlot);
		return {};
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientModel' on invalid player pawn: {}\n", playerSlot);
		return {};
	}

	return pawn->GetModelName();
}

/**
 * @brief Sets the model name of a client.
 *
 * This function updates the model name of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot.
 * @param model The new model name to set for the client.
 */
extern "C" PLUGIN_API void SetClientModel(int playerSlot, const plg::string& model) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientModel' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientModel' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	plg::print(LS_WARNING, "Try Set {}\n", model);
	pawn->SetModel(model.c_str());
	plg::print(LS_WARNING, "Set {}\n", model);
}

/**
 * @brief Retrieves the handle of the client's currently active weapon.
 *
 * @param playerSlot The index of the player's slot.
 * @return The entity handle of the active weapon, or INVALID_EHANDLE_INDEX if the client is invalid or has no active weapon.
 */
extern "C" PLUGIN_API int GetClientActiveWeapon(int playerSlot) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientActiveWeapon' on invalid player slot: {}\n", playerSlot);
		return INVALID_EHANDLE_INDEX;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientActiveWeapon' on invalid player pawn: {}\n", playerSlot);
		return INVALID_EHANDLE_INDEX;
	}

	CCSPlayer_WeaponServices* weaponServices = pawn->m_pWeaponServices;
	if (!weaponServices) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientActiveWeapon' on m_pWeaponServices: {}\n", playerSlot);
		return INVALID_EHANDLE_INDEX;
	}

	return weaponServices->m_hActiveWeapon().ToInt();
}

/**
 * @brief Retrieves a list of weapon handles owned by the client.
 *
 * @param playerSlot The index of the player's slot.
 * @return A vector of entity handles for the client's weapons, or an empty vector if the client is invalid or has no weapons.
 */
extern "C" PLUGIN_API plg::vector<int> GetClientWeapons(int playerSlot) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientWeapons' on invalid player slot: {}\n", playerSlot);
		return {};
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientActiveWeapon' on invalid player pawn: {}\n", playerSlot);
		return {};
	}

	CCSPlayer_WeaponServices* weaponServices = pawn->m_pWeaponServices;
	if (!weaponServices) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientWeapons' on m_pWeaponServices: {}\n", playerSlot);
		return {};
	}

	CUtlVector<CHandle<CBasePlayerWeapon>>* weapons = weaponServices->m_hMyWeapons;
	if (!weapons) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientWeapons' on m_hMyWeapons: {}\n", playerSlot);
		return {};
	}

	plg::vector<int> handles;
	handles.reserve(static_cast<size_t>(weapons->Count()));

	FOR_EACH_VEC(*weapons, i) {
		handles.emplace_back((*weapons)[i].ToInt());
	}

	return handles;
}

/**
 * @brief Removes all weapons from a client, with an option to remove the suit as well.
 *
 * @param playerSlot The index of the player's slot.
 * @param removeSuit A boolean indicating whether to also remove the client's suit.
 */
extern "C" PLUGIN_API void StripWeapons(int playerSlot, bool removeSuit) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'StripWeapons' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'StripWeapons' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	CCSPlayer_ItemServices* itemServices = pawn->m_pItemServices;
	if (!itemServices) {
		plg::print(LS_WARNING, "Cannot execute 'StripWeapons' on m_pItemServices: {}\n", playerSlot);
		return;
	}

	itemServices->StripPlayerWeapons(removeSuit);
}

/**
 * @brief Forces a player to drop their weapon.
 *
 * @param playerSlot The index of the player's slot.
 * @param weaponHandle The handle of weapon to drop.
 * @param target Target direction.
 * @param velocity Velocity to toss weapon or zero to just drop weapon.
 */
extern "C" PLUGIN_API void DropWeapon(int playerSlot, int weaponHandle, const plg::vec3& target, const plg::vec3& velocity) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'DropWeapon' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pWeapon = static_cast<CBasePlayerWeapon*>(g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) weaponHandle)));
	if (!pWeapon) {
		plg::print(LS_WARNING, "Cannot execute 'DropWeapon' on invalid weapon handle: {}\n", weaponHandle);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'DropWeapon' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	CCSPlayer_WeaponServices* weaponServices = pawn->m_pWeaponServices;
	if (!weaponServices) {
		plg::print(LS_WARNING, "Cannot execute 'DropWeapon' on m_pWeaponServices: {}\n", playerSlot);
		return;
	}

	weaponServices->Drop(pWeapon, reinterpret_cast<Vector*>(const_cast<plg::vec3*>(&target)), reinterpret_cast<Vector*>(const_cast<plg::vec3*>(&velocity)));
}

/**
 * @brief Bumps a player's weapon.
 *
 * @param playerSlot The index of the player's slot.
 * @param weaponHandle The handle of weapon to bump.
 */
extern "C" PLUGIN_API void BumpWeapon(int playerSlot, int weaponHandle) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SwitchWeapon' on invalid player slot: {}\n", playerSlot);
		return;
	}

	CBasePlayerWeapon* pWeapon = static_cast<CBasePlayerWeapon*>(g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) weaponHandle)));
	if (!pWeapon) {
		plg::print(LS_WARNING, "Cannot execute 'SwitchWeapon' on invalid weapon handle: {}\n", weaponHandle);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'SwitchWeapon' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	CCSPlayer_WeaponServices* weaponServices = pawn->m_pWeaponServices;
	if (!weaponServices) {
		plg::print(LS_WARNING, "Cannot execute 'SwitchWeapon' on m_pWeaponServices: {}\n", playerSlot);
		return;
	}

	weaponServices->Bump(pWeapon);
}

/**
 * @brief Switches a player's weapon.
 *
 * @param playerSlot The index of the player's slot.
 * @param weaponHandle The handle of weapon to switch.
 */
extern "C" PLUGIN_API void SwitchWeapon(int playerSlot, int weaponHandle) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SwitchWeapon' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pWeapon = static_cast<CBasePlayerWeapon*>(g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) weaponHandle)));
	if (!pWeapon) {
		plg::print(LS_WARNING, "Cannot execute 'SwitchWeapon' on invalid weapon handle: {}\n", weaponHandle);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'SwitchWeapon' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	CCSPlayer_WeaponServices* weaponServices = pawn->m_pWeaponServices;
	if (!weaponServices) {
		plg::print(LS_WARNING, "Cannot execute 'SwitchWeapon' on m_pWeaponServices: {}\n", playerSlot);
		return;
	}

	weaponServices->Switch(pWeapon);
}

/**
 * @brief Removes a player's weapon.
 *
 * @param playerSlot The index of the player's slot.
 * @param weaponHandle The handle of weapon to remove.
 */
extern "C" PLUGIN_API void RemoveWeapon(int playerSlot, int weaponHandle) {
	auto controller = utils::GetController(playerSlot);
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'RemovePlayerItem' on invalid player slot: {}\n", playerSlot);
		return;
	}

	auto pWeapon = static_cast<CBasePlayerWeapon*>(g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) weaponHandle)));
	if (!pWeapon) {
		plg::print(LS_WARNING, "Cannot execute 'RemovePlayerItem' on invalid weapon handle: {}\n", weaponHandle);
		return;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'RemovePlayerItem' on invalid player pawn: {}\n", playerSlot);
		return;
	}

	CCSPlayer_WeaponServices* weaponServices = pawn->m_pWeaponServices;
	if (!weaponServices) {
		plg::print(LS_WARNING, "Cannot execute 'RemoveWeapon' on m_pWeaponServices: {}\n", playerSlot);
		return;
	}

	weaponServices->RemoveItem(pWeapon);
}

/**
 * @brief Gives a named item (e.g., weapon) to a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemName The name of the item to give.
 * @return The entity handle of the created item, or INVALID_EHANDLE_INDEX if the client or item is invalid.
 */
extern "C" PLUGIN_API int GiveNamedItem(int playerSlot, const plg::string& itemName) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GiveNamedItem' on invalid player slot: {}\n", playerSlot);
		return INVALID_EHANDLE_INDEX;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GiveNamedItem' on invalid player pawn: {}\n", playerSlot);
		return INVALID_EHANDLE_INDEX;
	}

	CCSPlayer_ItemServices* itemServices = controller->GetCurrentPawn()->m_pItemServices;
	if (!itemServices) {
		plg::print(LS_WARNING, "Cannot execute 'GiveNamedItem' on m_pItemServices: {}\n", playerSlot);
		return INVALID_EHANDLE_INDEX;
	}

	CBaseEntity* pWeapon = itemServices->GiveNamedItem(itemName.c_str());
	if (!pWeapon) {
		plg::print(LS_WARNING, "Cannot execute 'GiveNamedItem' on m_pItemServices: {}\n", itemName);
		return INVALID_EHANDLE_INDEX;
	}

	return pWeapon->GetRefEHandle().ToInt();
}

/**
 * @brief Retrieves the state of a specific button for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param buttonIndex The index of the button (0-2).
 * @return uint64_t The state of the specified button, or 0 if the client or button index is invalid.
 */
extern "C" PLUGIN_API uint64_t GetClientButtons(int playerSlot, int buttonIndex) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientButtons' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	if (buttonIndex > 2 || buttonIndex < 0) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientButtons' on invalid button index: {}\n", buttonIndex);
		return 0;
	}

	auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
	if (!pawn) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientButtons' on invalid player pawn: {}\n", playerSlot);
		return INVALID_EHANDLE_INDEX;
	}

	CPlayer_MovementServices* movementServices = pawn->m_pMovementServices;
	if (!movementServices) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientButtons' on m_pMovementServices: {}\n", playerSlot);
		return 0;
	}

	return movementServices->m_nButtons().m_pButtonStates[buttonIndex];
}

/**
 * @brief Retrieves the amount of money a client has.
 *
 * @param playerSlot The index of the player's slot.
 * @return The amount of money the client has, or 0 if the player slot is invalid.
 */
extern "C" PLUGIN_API int GetClientMoney(int playerSlot) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientMoney' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	CCSPlayerController_InGameMoneyServices* inGameMoneyServices = controller->m_pInGameMoneyServices;
	if (!inGameMoneyServices) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientMoney' on m_pInGameMoneyServices: {}\n", playerSlot);
		return 0;
	}

	return inGameMoneyServices->m_iAccount;
}

/**
 * @brief Sets the amount of money for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param money The amount of money to set.
 */
extern "C" PLUGIN_API void SetClientMoney(int playerSlot, int money) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientMoney' on invalid player slot: {}\n", playerSlot);
		return;
	}

	CCSPlayerController_InGameMoneyServices* inGameMoneyServices = controller->m_pInGameMoneyServices;
	if (!inGameMoneyServices) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientMoney' on m_pInGameMoneyServices: {}\n", playerSlot);
		return;
	}

	inGameMoneyServices->m_iAccount = money;
}

/**
 * @brief Retrieves the number of kills for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @return The number of kills the client has, or 0 if the player slot is invalid.
 */
extern "C" PLUGIN_API int GetClientKills(int playerSlot) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientKills' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	CCSPlayerController_ActionTrackingServices* actionTrackingServices = controller->m_pActionTrackingServices;
	if (!actionTrackingServices) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientKills' on m_pActionTrackingServices: {}\n", playerSlot);
		return 0;
	}

	return actionTrackingServices->m_matchStats().m_iKills;
}

/**
 * @brief Sets the number of kills for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param kills The number of kills to set.
 */
extern "C" PLUGIN_API void SetClientKills(int playerSlot, int kills) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientKills' on invalid player slot: {}\n", playerSlot);
		return;
	}

	CCSPlayerController_ActionTrackingServices* actionTrackingServices = controller->m_pActionTrackingServices;
	if (!actionTrackingServices) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientKills' on m_pActionTrackingServices: {}\n", playerSlot);
		return;
	}

	actionTrackingServices->m_matchStats().m_iKills = kills;
}

/**
 * @brief Retrieves the number of deaths for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @return The number of deaths the client has, or 0 if the player slot is invalid.
 */
extern "C" PLUGIN_API int GetClientDeaths(int playerSlot) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientDeaths' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	CCSPlayerController_ActionTrackingServices* actionTrackingServices = controller->m_pActionTrackingServices;
	if (!actionTrackingServices) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientDeaths' on m_pActionTrackingServices: {}\n", playerSlot);
		return 0;
	}

	return actionTrackingServices->m_matchStats().m_iDeaths;
}

/**
 * @brief Sets the number of deaths for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param deaths The number of deaths to set.
 */
extern "C" PLUGIN_API void SetClientDeaths(int playerSlot, int deaths) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientDeaths' on invalid player slot: {}\n", playerSlot);
		return;
	}

	CCSPlayerController_ActionTrackingServices* actionTrackingServices = controller->m_pActionTrackingServices;
	if (!actionTrackingServices) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientDeaths' on m_pActionTrackingServices: {}\n", playerSlot);
		return;
	}

	actionTrackingServices->m_matchStats().m_iDeaths = deaths;
}

/**
 * @brief Retrieves the number of assists for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @return The number of assists the client has, or 0 if the player slot is invalid.
 */
extern "C" PLUGIN_API int GetClientAssists(int playerSlot) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientAssists' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	CCSPlayerController_ActionTrackingServices* actionTrackingServices = controller->m_pActionTrackingServices;
	if (!actionTrackingServices) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientAssists' on m_pActionTrackingServices: {}\n", playerSlot);
		return 0;
	}

	return actionTrackingServices->m_matchStats().m_iAssists;
}

/**
 * @brief Sets the number of assists for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param assists The number of assists to set.
 */
extern "C" PLUGIN_API void SetClientAssists(int playerSlot, int assists) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientAssists' on invalid player slot: {}\n", playerSlot);
		return;
	}

	CCSPlayerController_ActionTrackingServices* actionTrackingServices = controller->m_pActionTrackingServices;
	if (!actionTrackingServices) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientAssists' on m_pActionTrackingServices: {}\n", playerSlot);
		return;
	}

	actionTrackingServices->m_matchStats().m_iAssists = assists;
}

/**
 * @brief Retrieves the total damage dealt by a client.
 *
 * @param playerSlot The index of the player's slot.
 * @return The total damage dealt by the client, or 0 if the player slot is invalid.
 */
extern "C" PLUGIN_API int GetClientDamage(int playerSlot) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientDamage' on invalid player slot: {}\n", playerSlot);
		return 0;
	}

	CCSPlayerController_ActionTrackingServices* actionTrackingServices = controller->m_pActionTrackingServices;
	if (!actionTrackingServices) {
		plg::print(LS_WARNING, "Cannot execute 'GetClientDamage' on m_pActionTrackingServices: {}\n", playerSlot);
		return 0;
	}

	return actionTrackingServices->m_matchStats().m_iDamage;
}

/**
 * @brief Sets the total damage dealt by a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param damage The amount of damage to set.
 */
extern "C" PLUGIN_API void SetClientDamage(int playerSlot, int damage) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientDamage' on invalid player slot: {}\n", playerSlot);
		return;
	}

	CCSPlayerController_ActionTrackingServices* actionTrackingServices = controller->m_pActionTrackingServices;
	if (!actionTrackingServices) {
		plg::print(LS_WARNING, "Cannot execute 'SetClientDamage' on m_pActionTrackingServices: {}\n", playerSlot);
		return;
	}

	actionTrackingServices->m_matchStats().m_iDamage = damage;
}

/**
 * @brief Teleports a client to a specified location and orientation.
 *
 * This function teleports the specified client to the given absolute position,
 * with an optional new orientation and velocity. If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot.
 * @param origin A pointer to a Vector representing the new absolute position. Can be nullptr.
 * @param angles A pointer to a QAngle representing the new orientation. Can be nullptr.
 * @param velocity A pointer to a Vector representing the new velocity. Can be nullptr.
 */
extern "C" PLUGIN_API void TeleportClient(int playerSlot, const Vector* origin, const QAngle* angles, const Vector* velocity) {
	auto controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
	if (!controller) {
		plg::print(LS_WARNING, "Cannot execute 'TeleportClient' on invalid player slot: {}\n", playerSlot);
		return;
	}

	controller->GetPlayerPawn()->Teleport(origin, angles, velocity);
}

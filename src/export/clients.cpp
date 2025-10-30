#include <core/player_manager.hpp>
#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/entity/cbaseplayercontroller.h>
#include <core/sdk/entity/cplayercontroller.h>
#include <core/sdk/helpers.hpp>

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
 * @brief Retrieves the movement type of an client.
 *
 * This function returns the movement type of the specified client.
 * If the client is invalid, it returns 0.
 *
 * @param playerSlot The index of the player's slot whose movement type is to be retrieved.
 * @return The movement type of the client, or 0 if the client is invalid.
 */
extern "C" PLUGIN_API MoveType_t GetClientMoveType(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->m_MoveType;
}

/**
 * @brief Sets the movement type of an client.
 *
 * This function updates the movement type of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose movement type is to be set.
 * @param moveType The new movement type to set for the client.
 */
extern "C" PLUGIN_API void SetClientMoveType(int playerSlot, MoveType_t moveType) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetMoveType(moveType);
}

/**
 * @brief Retrieves the gravity scale of an client.
 *
 * This function returns the gravity scale of the specified client.
 * If the client is invalid, it returns 0.0f.
 *
 * @param playerSlot The index of the player's slot whose gravity scale is to be retrieved.
 * @return The gravity scale of the client, or 0.0f if the client is invalid.
 */
extern "C" PLUGIN_API float GetClientGravity(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->m_flGravityScale;
}

/**
 * @brief Sets the gravity scale of an client.
 *
 * This function updates the gravity scale of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose gravity scale is to be set.
 * @param gravity The new gravity scale to set for the client.
 */
extern "C" PLUGIN_API void SetClientGravity(int playerSlot, float gravity) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetGravity(gravity);
}

/**
 * @brief Retrieves the flags of an client.
 *
 * This function returns the flags of the specified client.
 * If the client is invalid, it returns 0.
 *
 * @param playerSlot The index of the player's slot whose flags are to be retrieved.
 * @return The flags of the client, or 0 if the client is invalid.
 */
extern "C" PLUGIN_API int GetClientFlags(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return *pawn->m_fFlags;
}

/**
 * @brief Sets the flags of an client.
 *
 * This function updates the flags of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose flags are to be set.
 * @param flags The new flags to set for the client.
 */
extern "C" PLUGIN_API void SetClientFlags(int playerSlot, int flags) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->m_fFlags = static_cast<uint32>(flags);
}

/**
 * @brief Retrieves the render color of an client.
 *
 * This function gets the render color of the specified client.
 * If the client is invalid, it returns 0.
 *
 * @param playerSlot The index of the player's slot whose render color is to be retrieved.
 * @return The raw color value of the client's render color, or 0 if the client is invalid.
 */
extern "C" PLUGIN_API int GetClientRenderColor(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->m_clrRender->GetRawColor();
}

/**
 * @brief Sets the render color of an client.
 *
 * This function updates the render color of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose render color is to be set.
 * @param color The new raw color value to set for the client's render color.
 */
extern "C" PLUGIN_API void SetClientRenderColor(int playerSlot, int color) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->m_clrRender = *reinterpret_cast<Color*>(&color);
}

/**
 * @brief Retrieves the render mode of an client.
 *
 * This function gets the render mode of the specified client.
 * If the client is invalid, it returns 0.
 *
 * @param playerSlot The index of the player's slot whose render mode is to be retrieved.
 * @return The render mode of the client, or 0 if the client is invalid.
 */
extern "C" PLUGIN_API uint8_t GetClientRenderMode(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->m_nRenderMode;
}

/**
 * @brief Sets the render mode of an client.
 *
 * This function updates the render mode of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose render mode is to be set.
 * @param renderMode The new render mode to set for the client.
 */
extern "C" PLUGIN_API void SetClientRenderMode(int playerSlot, uint8_t renderMode) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->m_nRenderMode = renderMode;
}

/**
 * @brief Retrieves the mass of an client.
 *
 * This function returns the current mass of the specified client.
 * If the client is invalid, it returns 0.
 *
 * @param playerSlot The index of the player's slot whose mass is to be retrieved.
 * @return The mass of the client, or 0 if the client is invalid.
 */
extern "C" PLUGIN_API int GetClientMass(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->GetMass();
}

/**
 * @brief Sets the mass of an client.
 *
 * This function updates the mass of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose mass is to be set.
 * @param mass The new mass value to set for the client.
 */
extern "C" PLUGIN_API void SetClientMass(int playerSlot, int mass) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetMass(mass);
}

/**
 * @brief Retrieves the friction of an client.
 *
 * This function returns the current friction of the specified client.
 * If the client is invalid, it returns 0.
 *
 * @param playerSlot The index of the player's slot whose friction is to be retrieved.
 * @return The friction of the client, or 0 if the client is invalid.
 */
extern "C" PLUGIN_API float GetClientFriction(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->m_flFriction;
}

/**
 * @brief Sets the friction of an client.
 *
 * This function updates the friction of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose friction is to be set.
 * @param friction The new friction value to set for the client.
 */
extern "C" PLUGIN_API void SetClientFriction(int playerSlot, float friction) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetFriction(friction);
}

/**
 * @brief Sets the friction of an client.
 *
 * This function updates the friction of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose friction is to be set.
 * @param duration Takes duration, value for a temporary override.
 * @param friction The new friction value to set for the client.
 */
extern "C" PLUGIN_API void OverrideClientFriction(int playerSlot, float duration, float friction) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->OverrideFriction(duration, friction);
}

/**
 * @brief Retrieves the health of an client.
 *
 * This function returns the current health of the specified client.
 * If the client is invalid, it returns 0.
 *
 * @param playerSlot The index of the player's slot whose health is to be retrieved.
 * @return The health of the client, or 0 if the client is invalid.
 */
extern "C" PLUGIN_API int GetClientHealth(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->GetHealth();
}

/**
 * @brief Sets the health of an client.
 *
 * This function updates the health of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose health is to be set.
 * @param health The new health value to set for the client.
 */
extern "C" PLUGIN_API void SetClientHealth(int playerSlot, int health) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetHealth(health);
}

/**
 * @brief Retrieves the max health of an client.
 *
 * This function returns the current max health of the specified client.
 * If the client is invalid, it returns 0.
 *
 * @param playerSlot The index of the player's slot whose max health is to be retrieved.
 * @return The max health of the client, or 0 if the client is invalid.
 */
extern "C" PLUGIN_API int GetClientMaxHealth(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->GetMaxHealth();
}

/**
 * @brief Sets the max health of an client.
 *
 * This function updates the max health of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose max health is to be set.
 * @param maxHealth The new max health value to set for the client.
 */
extern "C" PLUGIN_API void SetClientMaxHealth(int playerSlot, int maxHealth) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetMaxHealth(maxHealth);
}

/**
 * @brief Retrieves the team number of an client.
 *
 * This function returns the team number of the specified client.
 * If the client is invalid, it returns 0.
 *
 * @param playerSlot The index of the player's slot whose team number is to be retrieved.
 * @return The team number of the client, or 0 if the client is invalid.
 */
extern "C" PLUGIN_API int GetClientTeam(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!controller) return {};
	return controller->GetTeam();
}

/**
 * @brief Sets the team number of an client.
 *
 * This function updates the team number of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose team number is to be set.
 * @param team The new team number to set for the client.
 */
extern "C" PLUGIN_API void SetClientTeam(int playerSlot, int team) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!controller) return;
	controller->SetTeam(team);
}

/**
 * @brief Retrieves the absolute origin of an client.
 *
 * This function gets the absolute position of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose absolute origin is to be retrieved.
 * @return A vector where the absolute origin will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientAbsOrigin(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	const Vector& vec = pawn->GetAbsOrigin();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Sets the absolute origin of an client.
 *
 * This function updates the absolute position of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose absolute origin is to be set.
 * @param origin The new absolute origin to set for the client.
 */
extern "C" PLUGIN_API void SetClientAbsOrigin(int playerSlot, const plg::vec3& origin) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetAbsOrigin(*reinterpret_cast<const Vector*>(&origin));
}

/**
 * @brief Retrieves the absolute scale of an client.
 *
 * This function gets the absolute position of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose absolute scale is to be retrieved.
 * @return A vector where the absolute scale will be stored.
 */
extern "C" PLUGIN_API float GetClientAbsScale(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->GetAbsScale();
}

/**
 * @brief Sets the absolute scale of an client.
 *
 * This function updates the absolute position of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose absolute scale is to be set.
 * @param scale The new absolute scale to set for the client.
 */
extern "C" PLUGIN_API void SetClientAbsScale(int playerSlot, float scale) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetAbsScale(scale);
}

/**
 * @brief Retrieves the angular rotation of an client.
 *
 * This function gets the angular rotation of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose angular rotation is to be retrieved.
 * @return A QAngle where the angular rotation will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientAbsAngles(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	const QAngle& ang = pawn->GetAngles();
	return *reinterpret_cast<const plg::vec3*>(&ang);
}

/**
 * @brief Sets the angular rotation of an client.
 *
 * This function updates the angular rotation of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose angular rotation is to be set.
 * @param angle The new angular rotation to set for the client.
 */
extern "C" PLUGIN_API void SetClientAbsAngles(int playerSlot, const QAngle& angle) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetAbsAngles(angle.x, angle.y, angle.z);
}

/**
 * @brief Retrieves the local origin of an client.
 *
 * This function gets the local position of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose local origin is to be retrieved.
 * @return A vector where the local origin will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientLocalOrigin(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	const Vector& vec = pawn->GetLocalOrigin();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Sets the local origin of an client.
 *
 * This function updates the local position of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose local origin is to be set.
 * @param origin The new local origin to set for the client.
 */
extern "C" PLUGIN_API void SetClientLocalOrigin(int playerSlot, const plg::vec3& origin) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetLocalOrigin(*reinterpret_cast<const Vector*>(&origin));
}

/**
 * @brief Retrieves the local scale of an client.
 *
 * This function gets the local position of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose local scale is to be retrieved.
 * @return A vector where the local scale will be stored.
 */
extern "C" PLUGIN_API float GetClientLocalScale(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->GetLocalScale();
}

/**
 * @brief Sets the local scale of an client.
 *
 * This function updates the local position of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose local scale is to be set.
 * @param scale The new local scale to set for the client.
 */
extern "C" PLUGIN_API void SetClientLocalScale(int playerSlot, float scale) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetLocalScale(scale);
}

/**
 * @brief Retrieves the angular rotation of an client.
 *
 * This function gets the angular rotation of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose angular rotation is to be retrieved.
 * @return A QAngle where the angular rotation will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientLocalAngles(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	const QAngle& ang = pawn->GetAngles();
	return *reinterpret_cast<const plg::vec3*>(&ang);
}

/**
 * @brief Sets the angular rotation of an client.
 *
 * This function updates the angular rotation of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose angular rotation is to be set.
 * @param angle The new angular rotation to set for the client.
 */
extern "C" PLUGIN_API void SetClientLocalAngles(int playerSlot, const QAngle& angle) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetLocalAngles(angle.x, angle.y, angle.z);
}

/**
 * @brief Retrieves the absolute velocity of an client.
 *
 * This function gets the absolute velocity of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose absolute velocity is to be retrieved.
 * @return A vector where the absolute velocity will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientAbsVelocity(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	const Vector& vec = pawn->GetVelocity();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Sets the absolute velocity of an client.
 *
 * This function updates the absolute velocity of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose absolute velocity is to be set.
 * @param velocity The new absolute velocity to set for the client.
 */
extern "C" PLUGIN_API void SetClientAbsVelocity(int playerSlot, const plg::vec3& velocity) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetVelocity(*reinterpret_cast<const Vector*>(&velocity));
}

/**
 * @brief Retrieves the base velocity of an client.
 *
 * This function gets the base velocity of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose base velocity is to be retrieved.
 * @return A vector where the base velocity will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientBaseVelocity(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	const Vector& vec = pawn->GetBaseVelocity();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Retrieves the local angular velocity of an client.
 *
 * This function gets the local angular velocity of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose local angular velocity is to be retrieved.
 * @return A vector where the local angular velocity will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientLocalAngVelocity(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	const QAngle& ang = pawn->GetLocalAngularVelocity();
	return *reinterpret_cast<const plg::vec3*>(&ang);
}

/**
 * @brief Retrieves the angular velocity of an client.
 *
 * This function gets the angular velocity of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose angular velocity is to be retrieved.
 * @return A vector where the angular velocity will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientAngVelocity(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	const Vector& vec = pawn->GetAngularVelocity();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}
/**
 * @brief Sets the angular velocity of an client.
 *
 * This function updates the angular velocity of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose angular velocity is to be set.
 * @param velocity The new angular velocity to set for the client.
 */
extern "C" PLUGIN_API void SetClientAngVelocity(int playerSlot, const plg::vec3& velocity) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetAngularVelocity(velocity.x, velocity.y, velocity.z);
}

/**
 * @brief Retrieves the local velocity of an client.
 *
 * This function gets the local velocity of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose local velocity is to be retrieved.
 * @return A vector where the local velocity will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetClientLocalVelocity(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	const Vector& vec = pawn->GetLocalVelocity();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Returns the input Vector transformed from client to world space.
 *
 * Transforms a point from the client's local coordinate space to world coordinate space.
 *
 * @param playerSlot The index of the player's slot
 * @param point Point in client local space to transform
 * @return The point transformed to world space coordinates
 */
extern "C" PLUGIN_API plg::vec3 TransformPointClientToWorld(int playerSlot, const plg::vec3& point) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const Vector& pos = pawn->TransformPointEntityToWorld(*reinterpret_cast<const Vector*>(&point));
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Returns the input Vector transformed from world to client space.
 *
 * Transforms a point from world coordinate space to the client's local coordinate space.
 *
 * @param playerSlot The index of the player's slot
 * @param point Point in world space to transform
 * @return The point transformed to client local space coordinates
 */
extern "C" PLUGIN_API plg::vec3 TransformPointWorldToClient(int playerSlot, const plg::vec3& point) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const Vector& pos = pawn->TransformPointWorldToEntity(*reinterpret_cast<const Vector*>(&point));
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get vector to eye position - absolute coords.
 *
 * Returns the position of the client's eyes in world space coordinates.
 *
 * @param playerSlot The index of the player's slot
 * @return Eye position in absolute/world coordinates
 */
extern "C" PLUGIN_API plg::vec3 GetClientEyePosition(int playerSlot) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const Vector& pos = pawn->EyePosition();
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get the qangles that this client is looking at.
 *
 * Returns the eye angles (pitch, yaw, roll) representing the direction
 * the client is looking.
 *
 * @param playerSlot The index of the player's slot
 * @return Eye angles as a vector (pitch, yaw, roll)
 */
extern "C" PLUGIN_API plg::vec3 GetClientEyeAngles(int playerSlot) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const QAngle& ang = pawn->EyeAngles();
    return *reinterpret_cast<const plg::vec3*>(&ang);
}

/**
 * @brief Sets the forward velocity of an client.
 *
 * This function updates the forward velocity of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose forward velocity is to be set.
 * @param forward The new forward velocity to set for the client.
 */
extern "C" PLUGIN_API void SetClientForwardVector(int playerSlot, const plg::vec3& forward) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetForwardVector(*reinterpret_cast<const Vector*>(&forward));
}

/**
 * @brief Get the forward vector of the client.
 *
 * @param playerSlot The index of the player's slot to query
 * @return Forward-facing direction vector of the client
 */
extern "C" PLUGIN_API plg::vec3 GetClientForwardVector(int playerSlot) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const Vector& pos = pawn->GetForwardVector();
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get the left vector of the client.
 *
 * @param playerSlot The index of the player's slot to query
 * @return Left-facing direction vector of the client (aligned with the y axis)
 */
extern "C" PLUGIN_API plg::vec3 GetClientLeftVector(int playerSlot) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const Vector& pos = pawn->GetLeftVector();
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get the right vector of the client.
 *
 * @warning This produces a left-handed coordinate system. Use GetLeftVector instead
 *          (which is aligned with the y axis of the client).
 *
 * @param playerSlot The index of the player's slot to query
 * @return Right-facing direction vector of the client
 */
extern "C" PLUGIN_API plg::vec3 GetClientRightVector(int playerSlot) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const Vector& pos = pawn->GetRightVector();
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get the up vector of the client.
 *
 * @param playerSlot The index of the player's slot to query
 * @return Up-facing direction vector of the client
 */
extern "C" PLUGIN_API plg::vec3 GetClientUpVector(int playerSlot) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const Vector& pos = pawn->GetUpVector();
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get the client-to-world transformation matrix.
 *
 * Returns the complete transformation matrix that converts coordinates from
 * client local space to world space.
 *
 * @param playerSlot The index of the player's slot to query
 * @return 4x4 transformation matrix representing client's position, rotation, and scale in world space
 */
extern "C" PLUGIN_API plg::mat4x4 GetClientTransform(int playerSlot) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const matrix3x4_t& mat = pawn->m_CBodyComponent->m_pSceneNode->EntityToWorldTransform();
    return *reinterpret_cast<const plg::mat4x4*>(&mat);
}

/**
 * @brief Retrieves the model name of an client.
 *
 * This function gets the model name of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose model name is to be retrieved.
 * @return A string where the model name will be stored.
 */
extern "C" PLUGIN_API plg::string GetClientModel(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->GetModelName();
}

/**
 * @brief Sets the model name of an client.
 *
 * This function updates the model name of the specified client.
 * If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot whose model name is to be set.
 * @param model The new model name to set for the client.
 */
extern "C" PLUGIN_API void SetClientModel(int playerSlot, const plg::string& model) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->SetModel(model.c_str());
}

/**
 * @brief Retrieves the water level of an client.
 *
 * This function returns the water level of the specified client.
 * If the client is invalid, it returns 0.0f.
 *
 * @param playerSlot The index of the player's slot whose water level is to be retrieved.
 * @return The water level of the client, or 0.0f if the client is invalid.
 */
extern "C" PLUGIN_API float GetClientWaterLevel(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->m_flWaterLevel;
}

/**
 * @brief Retrieves the ground client of an client.
 *
 * This function returns the handle of the ground client for the specified client.
 * If the client is invalid, it returns -1.
 *
 * @param playerSlot The index of the player's slot whose ground client is to be retrieved.
 * @return The handle of the ground client, or INVALID_EHANDLE_INDEX if the client is invalid.
 */
extern "C" PLUGIN_API int GetClientGroundEntity(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};

	CBaseEntity* ground = *pawn->m_hGroundEntity;
	if (!ground) {
		return INVALID_EHANDLE_INDEX;
	}

	return ground->GetRefEHandle().ToInt();
}

/**
 * @brief Retrieves the effects of an client.
 *
 * This function returns the effect flags of the specified client.
 * If the client is invalid, it returns 0.
 *
 * @param playerSlot The index of the player's slot whose effects are to be retrieved.
 * @return The effect flags of the client, or 0 if the client is invalid.
 */
extern "C" PLUGIN_API int GetClientEffects(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->m_fEffects;
}

/**
 * @brief Adds the render effect flag to an client.
 *
 * @param playerSlot The index of the player's slot to modify
 * @param effects Render effect flags to add
 */
extern "C" PLUGIN_API void AddClientEffects(int playerSlot, int effects) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return;
    pawn->AddEffects(effects);
}

/**
 * @brief Removes the render effect flag from an client.
 *
 * @param playerSlot The index of the player's slot to modify
 * @param effects Render effect flags to remove
 */
extern "C" PLUGIN_API void RemoveClientEffects(int playerSlot, int effects) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return;
    pawn->RemoveEffects(effects);
}

/**
 * @brief Get a vector containing max bounds, centered on object.
 *
 * @param playerSlot The index of the player's slot to query
 * @return Vector containing the maximum bounds of the client's bounding box
 */
extern "C" PLUGIN_API plg::vec3 GetClientBoundingMaxs(int playerSlot) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const Vector& vec = pawn->GetBoundingMaxs();
    return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Get a vector containing min bounds, centered on object.
 *
 * @param playerSlot The index of the player's slot to query
 * @return Vector containing the minimum bounds of the client's bounding box
 */
extern "C" PLUGIN_API plg::vec3 GetClientBoundingMins(int playerSlot) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const Vector& vec = pawn->GetBoundingMins();
    return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Get vector to center of object - absolute coords.
 *
 * @param playerSlot The index of the player's slot to query
 * @return Vector pointing to the center of the client in absolute/world coordinates
 */
extern "C" PLUGIN_API plg::vec3 GetClientCenter(int playerSlot) {
    auto [controller, pawn] = helpers::GetController2(playerSlot);
    if (!pawn) return {};
    const Vector& vec = pawn->GetCenter();
    return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Teleports an client to a specified location and orientation.
 *
 * This function teleports the specified client to the given absolute position,
 * with an optional new orientation and velocity. If the client is invalid, the function does nothing.
 *
 * @param playerSlot The index of the player's slot to teleport.
 * @param origin A pointer to a Vector representing the new absolute position. Can be nullptr.
 * @param angles A pointer to a QAngle representing the new orientation. Can be nullptr.
 * @param velocity A pointer to a Vector representing the new velocity. Can be nullptr.
 */
extern "C" PLUGIN_API void TeleportClient(int playerSlot, const Vector* origin, const QAngle* angles, const Vector* velocity) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->Teleport(origin, angles, velocity);
}

/**
 * @brief Apply an absolute velocity impulse to an client.
 *
 * Applies an instantaneous change to the client's velocity in world space.
 *
 * @param playerSlot The index of the player's slot to apply impulse to
 * @param vecImpulse Velocity impulse vector to apply
 */
extern "C" PLUGIN_API void ApplyAbsVelocityImpulseToClient(int playerSlot, const plg::vec3& vecImpulse) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->ApplyAbsVelocityImpulse(*reinterpret_cast<const Vector*>(&vecImpulse));
}

/**
 * @brief Apply a local angular velocity impulse to an client.
 *
 * Applies an instantaneous change to the client's rotational velocity in local space.
 *
 * @param playerSlot The index of the player's slot to apply impulse to
 * @param angImpulse Angular velocity impulse vector to apply
 */
extern "C" PLUGIN_API void ApplyLocalAngularVelocityImpulseToClient(int playerSlot, const plg::vec3& angImpulse) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->ApplyLocalAngularVelocityImpulse(*reinterpret_cast<const Vector*>(&angImpulse));
}

/**
 * @brief Invokes a named input method on a specified client.
 *
 * This function triggers an input action on an client, allowing dynamic interaction
 * between game objects or entities within the system. The input can be initiated
 * by another client (activator) or by a specific caller client.
 *
 * @param playerSlot The handle of the target client that will receive the input.
 * @param inputName    The name of the input action to invoke.
 * @param activatorHandle The index of the player's slot that initiated the sequence of actions.
 * @param callerHandle The index of the player's slot sending this event. Use -1 to specify
 * @param value        The value associated with the input action.
 * @param type         The type or classification of the value.
* @param outputId      An identifier for tracking the output of this operation.
 */
extern "C" PLUGIN_API void AcceptClientInput(int playerSlot, const plg::string& inputName, int activatorHandle, int callerHandle, const plg::any& value, FieldType type, int outputId) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	variant_t variant = helpers::GetVariant(value, type);
	CEntityInstance* activator = activatorHandle != INVALID_EHANDLE_INDEX ? g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) activatorHandle)) : nullptr;
	CEntityInstance* caller = callerHandle != INVALID_EHANDLE_INDEX ? g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) callerHandle)) : nullptr;
	pawn->AcceptInput(inputName.c_str(), variant, activator, caller, outputId);
}

/**
 * @brief Connects a script function to an player output.
 *
 * This function will call the specified function on this player when the output fires.
 *
 * @param playerSlot The handle of the player.
 * @param output The name of the output to connect to.
 * @param functionName The name of the script function to call.
 */
extern "C" PLUGIN_API void ConnectClientOutput(int playerSlot, const plg::string& output, const plg::string& functionName) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	reinterpret_cast<CEntityInstance2*>(pawn)->ConnectOutput(output.c_str(), functionName.c_str());
}

/**
 * @brief Disconnects a script function from an player output.
 *
 * This removes the function from being called when the output fires.
 *
 * @param playerSlot The handle of the player.
 * @param output The name of the output.
 * @param functionName The name of the script function to disconnect.
 */
extern "C" PLUGIN_API void DisconnectClientOutput(int playerSlot, const plg::string& output, const plg::string& functionName) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	reinterpret_cast<CEntityInstance2*>(pawn)->DisconnectOutput(output.c_str(), functionName.c_str());
}

/**
 * @brief Disconnects a script function from an I/O event on a different player.
 *
 * @param playerSlot The handle of the calling player.
 * @param output The name of the output.
 * @param functionName The function name to disconnect.
 * @param targetHandle The handle of the entity whose output is being disconnected.
 */
extern "C" PLUGIN_API void DisconnectClientRedirectedOutput(int playerSlot, const plg::string& output, const plg::string& functionName, int targetHandle) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	auto* target = helpers::GetEntity(targetHandle);
	if (!target) return;
	reinterpret_cast<CEntityInstance2*>(pawn)->DisconnectRedirectedOutput(output.c_str(), functionName.c_str(), target->GetScriptInstance());
}

/**
 * @brief Fires an player output.
 *
 * Calls all connected functions for the given output.
 *
 * @param playerSlot The handle of the player firing the output.
 * @param outputName The name of the output to fire.
 * @param activatorHandle The entity activating the output.
 * @param callerHandle The entity that called the output.
 * @param value The value associated with the input action.
 * @param type The type or classification of the value.
 * @param delay Delay in seconds before firing the output.
 */
extern "C" PLUGIN_API void FireClientOutput(int playerSlot, const plg::string& outputName, int activatorHandle, int callerHandle, const plg::any& value, FieldType type, float delay) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	CEntityInstance* activator = activatorHandle != INVALID_EHANDLE_INDEX ? g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) activatorHandle)) : nullptr;
	CEntityInstance* caller = callerHandle != INVALID_EHANDLE_INDEX ? g_pGameEntitySystem->GetEntityInstance(CEntityHandle((uint32) callerHandle)) : nullptr;
	variant_t variant = helpers::GetVariant(value, type);
	reinterpret_cast<CEntityInstance2*>(pawn)->FireOutput(outputName.c_str(), activator ? activator->GetScriptInstance() : nullptr, caller ? caller->GetScriptInstance() : nullptr, variant, delay);
}

/**
 * @brief Redirects an player output to call a function on another player.
 *
 * @param playerSlot The handle of the player whose output is being redirected.
 * @param output The name of the output to redirect.
 * @param functionName The function name to call on the target player.
 * @param targetHandle The handle of the entity that will receive the output call.
 */
extern "C" PLUGIN_API void RedirectClientOutput(int playerSlot, const plg::string& output, const plg::string& functionName, int targetHandle) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	auto* target = helpers::GetEntity(targetHandle);
	if (!target) return;
	reinterpret_cast<CEntityInstance2*>(pawn)->RedirectOutput(output.c_str(), functionName.c_str(), target->GetScriptInstance());
}

/**
 * @brief Makes an client follow another client with optional bone merging.
 *
 * @param playerSlot The index of the player's slot that will follow
 * @param attachmentHandle The index of the player's slot to follow
 * @param boneMerge If true, bones will be merged between entities
 */
extern "C" PLUGIN_API void FollowClient(int playerSlot, int attachmentHandle, bool boneMerge) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	auto* attach = helpers::GetEntity(attachmentHandle);
	if (!attach) return;
	pawn->FollowEntity(attach->GetScriptInstance(), boneMerge);
}

/**
 * @brief Makes an client follow another client and merge with a specific bone or attachment.
 *
 * @param playerSlot The index of the player's slot that will follow
 * @param attachmentHandle The index of the player's slot to follow
 * @param boneOrAttachName Name of the bone or attachment point to merge with
 */
extern "C" PLUGIN_API void FollowClientMerge(int playerSlot, int attachmentHandle, const plg::string& boneOrAttachName) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	auto* attach = helpers::GetEntity(attachmentHandle);
	if (!attach) return;
	pawn->FollowEntity(attach->GetScriptInstance(), boneOrAttachName.c_str());
}

/**
 * @brief Apply damage to an client.
 *
 * Creates a damage info object and applies damage to the specified client.
 *
 * @param playerSlot The index of the player's slot receiving damage
 * @param inflictorSlot The index of the player's slot inflicting damage (e.g., projectile)
 * @param attackerSlot The index of the attacking client
 * @param force Direction and magnitude of force to apply
 * @param hitPos Position where the damage hit occurred
 * @param damage Amount of damage to apply
 * @param damageTypes Bitfield of damage type flags
 * @return Amount of damage actually applied to the client
 */
extern "C" PLUGIN_API int TakeClientDamage(int playerSlot, int inflictorSlot, int attackerSlot, const plg::vec3& force, const plg::vec3& hitPos, float damage, DamageTypes_t damageTypes) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	auto [controller2, inflictor] = helpers::GetController2(inflictorSlot);
	if (!inflictor) return {};
	auto [controller3, attacker] = helpers::GetController2(attackerSlot);
	if (!attacker) return {};
	HSCRIPT takeDamageInfo = CTakeDamage{}.CreateDamageInfo(inflictor->GetScriptInstance(), attacker->GetScriptInstance(), *reinterpret_cast<const Vector*>(&force), *reinterpret_cast<const Vector*>(&hitPos), damage, damageTypes);
	int applied = pawn->TakeDamage(takeDamageInfo);
	CTakeDamage{}.DestroyDamageInfo(takeDamageInfo);
	return applied;
}

/////

/**
 * @brief Retrieves the pawn entity pointer associated with a client.
 *
 * @param playerSlot The index of the player's slot.
 * @return A pointer to the client's pawn entity, or nullptr if the client or controller is invalid.
 */
extern "C" PLUGIN_API void* GetClientPawn(int playerSlot) {
	auto controller = helpers::GetController(playerSlot);
	if (!controller) return {};
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
 * @brief Switches the player's team.
 *
 * @param playerSlot The index of the player's slot.
 * @param team The team index to switch the client to.
 */
extern "C" PLUGIN_API void SwitchClientTeam(int playerSlot, int team) {
	auto controller = helpers::GetController(playerSlot);
	if (!controller) return;
	controller->SwitchTeam(team);
}

/**
 * @brief Respawns a player.
 *
 * @param playerSlot The index of the player's slot to respawn.
 */
extern "C" PLUGIN_API void RespawnClient(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	if (pawn->IsAlive()) {
		pawn->Respawn();
	} else {
		controller->Respawn();
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
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
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
 * @brief Retrieves the handle of the client's currently active weapon.
 *
 * @param playerSlot The index of the player's slot.
 * @return The entity handle of the active weapon, or INVALID_EHANDLE_INDEX if the client is invalid or has no active weapon.
 */
extern "C" PLUGIN_API int GetClientActiveWeapon(int playerSlot) {
	auto service = helpers::GetService2<CCSPlayer_WeaponServices>(playerSlot, &CPlayerPawn::m_pWeaponServices);
	if (!service) return {};
	return service->m_hActiveWeapon->ToInt();
}

/**
 * @brief Retrieves a list of weapon handles owned by the client.
 *
 * @param playerSlot The index of the player's slot.
 * @return A vector of entity handles for the client's weapons, or an empty vector if the client is invalid or has no weapons.
 */
extern "C" PLUGIN_API plg::vector<int> GetClientWeapons(int playerSlot) {
	auto service = helpers::GetService2<CCSPlayer_WeaponServices>(playerSlot, &CPlayerPawn::m_pWeaponServices);
	if (!service) return {};

	CUtlVector<CHandle<CBasePlayerWeapon>>* weapons = service->m_hMyWeapons;
	if (!weapons) return {};

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
extern "C" PLUGIN_API void RemoveWeapons(int playerSlot, bool removeSuit) {
	auto service = helpers::GetService2<CCSPlayer_ItemServices>(playerSlot, &CPlayerPawn::m_pItemServices);
	if (!service) return;
	service->RemoveWeapons(removeSuit);
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
	auto service = helpers::GetService2<CCSPlayer_WeaponServices>(playerSlot, &CPlayerPawn::m_pWeaponServices);
	if (!service) return;
	auto weapon = helpers::GetEntity<CBasePlayerWeapon>(weaponHandle);
	if (!weapon) return;
	service->DropWeapon(weapon, reinterpret_cast<Vector*>(const_cast<plg::vec3*>(&target)), reinterpret_cast<Vector*>(const_cast<plg::vec3*>(&velocity)));
}

/**
 * @brief Selects a player's weapon.
 *
 * @param playerSlot The index of the player's slot.
 * @param weaponHandle The handle of weapon to bump.
 */
extern "C" PLUGIN_API void SelectWeapon(int playerSlot, int weaponHandle) {
	auto service = helpers::GetService2<CCSPlayer_WeaponServices>(playerSlot, &CPlayerPawn::m_pWeaponServices);
	if (!service) return;
	auto weapon = helpers::GetEntity<CBasePlayerWeapon>(weaponHandle);
	if (!weapon) return;
	service->SelectItem(weapon);
}

/**
 * @brief Switches a player's weapon.
 *
 * @param playerSlot The index of the player's slot.
 * @param weaponHandle The handle of weapon to switch.
 */
extern "C" PLUGIN_API void SwitchWeapon(int playerSlot, int weaponHandle) {
	auto service = helpers::GetService2<CCSPlayer_WeaponServices>(playerSlot, &CPlayerPawn::m_pWeaponServices);
	if (!service) return;
	auto weapon = helpers::GetEntity<CBasePlayerWeapon>(weaponHandle);
	if (!weapon) return;
	service->SwitchWeapon(weapon);
}

/**
 * @brief Removes a player's weapon.
 *
 * @param playerSlot The index of the player's slot.
 * @param weaponHandle The handle of weapon to remove.
 */
extern "C" PLUGIN_API void RemoveWeapon(int playerSlot, int weaponHandle) {
	auto service = helpers::GetService2<CCSPlayer_WeaponServices>(playerSlot, &CPlayerPawn::m_pWeaponServices);
	if (!service) return;
	auto weapon = helpers::GetEntity<CBasePlayerWeapon>(weaponHandle);
	if (!weapon) return;
	service->RemoveItem(weapon);
}

/**
 * @brief Gives a named item (e.g., weapon) to a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param itemName The name of the item to give.
 * @return The entity handle of the created item, or INVALID_EHANDLE_INDEX if the client or item is invalid.
 */
extern "C" PLUGIN_API int GiveNamedItem(int playerSlot, const plg::string& itemName) {
	auto service = helpers::GetService2<CCSPlayer_ItemServices>(playerSlot, &CPlayerPawn::m_pItemServices);
	if (!service) return {};
	CBaseEntity* weapon = service->GiveNamedItem(itemName.c_str());
	if (!weapon) return INVALID_EHANDLE_INDEX;
	return weapon->GetRefEHandle().ToInt();
}

/**
 * @brief Retrieves the state of a specific button for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param buttonIndex The index of the button (0-2).
 * @return uint64_t The state of the specified button, or 0 if the client or button index is invalid.
 */
extern "C" PLUGIN_API uint64_t GetClientButtons(int playerSlot, int buttonIndex) {
	auto service = helpers::GetService2<CPlayer_MovementServices>(playerSlot, &CPlayerPawn::m_pMovementServices);
	if (!service) return {};
	return service->m_nButtons->m_pButtonStates[buttonIndex];
}

/**
 * @brief Returns the client's armor value.
 *
 * @param playerSlot The index of the player's slot.
 * @return The armor value of the client.
 */
extern "C" PLUGIN_API int GetClientArmor(int playerSlot) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return {};
	return pawn->m_ArmorValue;
}

/**
 * @brief Sets the client's armor value.
 *
 * @param playerSlot The index of the player's slot.
 * @param armor The armor value to set.
 */
extern "C" PLUGIN_API void SetClientArmor(int playerSlot, int armor) {
	auto [controller, pawn] = helpers::GetController2(playerSlot);
	if (!pawn) return;
	pawn->m_ArmorValue = armor;
}

/**
 * @brief Retrieves the amount of money a client has.
 *
 * @param playerSlot The index of the player's slot.
 * @return The amount of money the client has, or 0 if the player slot is invalid.
 */
extern "C" PLUGIN_API int GetClientMoney(int playerSlot) {
	auto service = helpers::GetService<CCSPlayerController_InGameMoneyServices>(playerSlot, &CPlayerController::m_pInGameMoneyServices);
	if (!service) return {};
	return service->m_iAccount;
}

/**
 * @brief Sets the amount of money for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param money The amount of money to set.
 */
extern "C" PLUGIN_API void SetClientMoney(int playerSlot, int money) {
	auto service = helpers::GetService<CCSPlayerController_InGameMoneyServices>(playerSlot, &CPlayerController::m_pInGameMoneyServices);
	if (!service) return;
	service->m_iAccount = money;
}

/**
 * @brief Retrieves the number of kills for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @return The number of kills the client has, or 0 if the player slot is invalid.
 */
extern "C" PLUGIN_API int GetClientKills(int playerSlot) {
	auto service = helpers::GetService<CCSPlayerController_ActionTrackingServices>(playerSlot, &CPlayerController::m_pActionTrackingServices);
	if (!service) return {};
	return service->m_matchStats->m_iKills;
}

/**
 * @brief Sets the number of kills for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param kills The number of kills to set.
 */
extern "C" PLUGIN_API void SetClientKills(int playerSlot, int kills) {
	auto service = helpers::GetService<CCSPlayerController_ActionTrackingServices>(playerSlot, &CPlayerController::m_pActionTrackingServices);
	if (!service) return;
	service->m_matchStats->m_iKills = kills;
}

/**
 * @brief Retrieves the number of deaths for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @return The number of deaths the client has, or 0 if the player slot is invalid.
 */
extern "C" PLUGIN_API int GetClientDeaths(int playerSlot) {
	auto service = helpers::GetService<CCSPlayerController_ActionTrackingServices>(playerSlot, &CPlayerController::m_pActionTrackingServices);
	if (!service) return {};
	return service->m_matchStats->m_iDeaths;
}

/**
 * @brief Sets the number of deaths for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param deaths The number of deaths to set.
 */
extern "C" PLUGIN_API void SetClientDeaths(int playerSlot, int deaths) {
	auto service = helpers::GetService<CCSPlayerController_ActionTrackingServices>(playerSlot, &CPlayerController::m_pActionTrackingServices);
	if (!service) return;
	service->m_matchStats->m_iDeaths = deaths;
}

/**
 * @brief Retrieves the number of assists for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @return The number of assists the client has, or 0 if the player slot is invalid.
 */
extern "C" PLUGIN_API int GetClientAssists(int playerSlot) {
	auto service = helpers::GetService<CCSPlayerController_ActionTrackingServices>(playerSlot, &CPlayerController::m_pActionTrackingServices);
	if (!service) return {};
	return service->m_matchStats->m_iAssists;
}

/**
 * @brief Sets the number of assists for a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param assists The number of assists to set.
 */
extern "C" PLUGIN_API void SetClientAssists(int playerSlot, int assists) {
	auto service = helpers::GetService<CCSPlayerController_ActionTrackingServices>(playerSlot, &CPlayerController::m_pActionTrackingServices);
	if (!service) return;
	service->m_matchStats->m_iAssists = assists;
}

/**
 * @brief Retrieves the total damage dealt by a client.
 *
 * @param playerSlot The index of the player's slot.
 * @return The total damage dealt by the client, or 0 if the player slot is invalid.
 */
extern "C" PLUGIN_API int GetClientDamage(int playerSlot) {
	auto service = helpers::GetService<CCSPlayerController_ActionTrackingServices>(playerSlot, &CPlayerController::m_pActionTrackingServices);
	if (!service) return {};
	return service->m_matchStats->m_iDamage;
}

/**
 * @brief Sets the total damage dealt by a client.
 *
 * @param playerSlot The index of the player's slot.
 * @param damage The amount of damage to set.
 */
extern "C" PLUGIN_API void SetClientDamage(int playerSlot, int damage) {
	auto service = helpers::GetService<CCSPlayerController_ActionTrackingServices>(playerSlot, &CPlayerController::m_pActionTrackingServices);
	if (!service) return;
	service->m_matchStats->m_iDamage = damage;
}

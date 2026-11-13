#pragma once

#include "core/listener_manager.hpp"
#include <core/sdk/entity/cgamerules.h>
#include <cstrike15/cstrike15_item_constants.h>

class UserMessage;

template<char const* Name, typename Func>
using Manager = ListenerManager<Name, Func>;

#define LISTENER_LIST(X) \
	X(ClientConnect, ENetworkDisconnectionReason, const plg::string& playerName, const plg::string& address, uint64 steamID) \
	X(ClientConnected, void, int playerSlot) \
	X(ClientSessionStart, void, int playerSlot) \
	X(ClientPutInServer, void, int playerSlot) \
	X(ClientDisconnect, void, int playerSlot, ENetworkDisconnectionReason reason) \
	X(ClientSessionEnd, void, int playerSlot, ENetworkDisconnectionReason reason) \
	X(ClientActive, void, int playerSlot, bool loadGame) \
	X(ClientSignonStateChanged, void, int playerSlot, SignonState_t state, int spawnCount) \
	X(ClientFullyConnect, void, int playerSlot) \
	X(ClientSettingsChanged, void, int playerSlot) \
	X(ClientAuthenticated, void, int playerSlot, uint64 steamID) \
	X(ClientInventoryLoaded, void, int playerSlot) \
	X(ClientLoadoutChanged, ResultType, int playerSlot, CSTeam team, loadout_slot_t loadoutSlot) \
	X(ClientGiveNamedItem, uint64, int playerSlot, const plg::string& itemName) \
	X(ClientGiveNamedItemPost, void, int playerSlot, int entityHandle) \
	X(RoundTerminated, void, float delay, CRoundEndReason reason) \
	X(EntityCreated, void, int entityHandle) \
	X(EntitySpawned, void, int entityHandle) \
	X(EntityDeleted, void, int entityHandle) \
	X(EntityParentChanged, void, int entityHandle, int parentHandle) \
	X(ServerCheckTransmit, void, const plg::vector<CCheckTransmitInfo*>&) \
	X(ServerStartup, void) \
	X(BuildGameSessionManifest, void) \
	X(ServerActivate, void) \
	X(ServerSpawn, void) \
	X(ServerStarted, void) \
	X(MapStart, void) \
	X(MapEnd, void) \
	X(GameFrame, void, bool simulating, bool firstTick, bool lastTick) \
	X(UpdateWhenNotInGame, void, float frameTime) \
	X(PreWorldUpdate, void, bool simulating) \
	X(ClientSpeakingStart, void, int playerSlot) \
	X(ClientSpeakingEnd, void, int playerSlot) \
	X(SendNetMessage, void, int playerSlot, UserMessage* message) \
	X(SerializeMessage, void, UserMessage* message) \

extern "C" {
#define DECLARE_ACCESSOR(name, ret, ...) \
	using name##Callback = ret (*)(__VA_ARGS__); \
	static constexpr const char name##_str[] = S2SDK_PACKAGE "::" #name "Listener"; \
	inline Manager<name##_str, name##Callback> g_##name##ListenerManager;

	LISTENER_LIST(DECLARE_ACCESSOR)
}

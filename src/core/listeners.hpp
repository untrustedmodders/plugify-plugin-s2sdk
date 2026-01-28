#pragma once

#include "core/listener_manager.hpp"
#include <core/sdk/entity/cgamerules.h>

#define X(name, ret, ...) \
	using name##Callback = ret (*)(__VA_ARGS__);     \
	ListenerManager<name##Callback, std::shared_mutex>& Get##name##ListenerManager();

template<typename Fn>
using Manager = ListenerManager<Fn, std::shared_mutex>;

#define LISTENER_LIST(X) \
	X(OnClientConnect, bool, int playerSlot, const plg::string& playerName, const plg::string& networkID) \
	X(OnClientConnect_Post, void, int playerSlot) \
	X(OnClientConnected, void, int playerSlot) \
	X(OnClientPutInServer, void, int playerSlot) \
	X(OnClientDisconnect, void, int playerSlot, ENetworkDisconnectionReason reason) \
	X(OnClientDisconnect_Post, void, int playerSlot, ENetworkDisconnectionReason reason) \
	X(OnClientActive, void, int playerSlot, bool loadGame) \
	X(OnClientFullyConnect, void, int playerSlot) \
	X(OnClientSettingsChanged, void, int playerSlot) \
	X(OnClientAuthenticated, void, int playerSlot, uint64 steamID) \
	X(OnRoundTerminated, void, float delay, CRoundEndReason reason) \
	X(OnEntityCreated, void, int entityHandle) \
	X(OnEntityDeleted, void, int entityHandle) \
	X(OnEntityParentChanged, void, int entityHandle, int parentHandle) \
	X(OnServerCheckTransmit, void, const plg::vector<CCheckTransmitInfo*>&) \
	X(OnServerStartup, void) \
	X(OnBuildGameSessionManifest, void) \
	X(OnServerActivate, void) \
	X(OnServerSpawn, void) \
	X(OnServerStarted, void) \
	X(OnMapStart, void) \
	X(OnMapEnd, void) \
	X(OnGameFrame, void, bool simulating, bool firstTick, bool lastTick) \
	X(OnUpdateWhenNotInGame, void, float frameTime) \
	X(OnPreWorldUpdate, void, bool simulating) \

extern "C" {
#define DECLARE_ACCESSOR(name, ret, ...) \
	using name##Callback = ret (*)(__VA_ARGS__); \
	inline Manager<name##Callback> g_##name##ListenerManager;

	LISTENER_LIST(DECLARE_ACCESSOR)
}

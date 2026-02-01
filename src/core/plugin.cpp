#include <core/sdk/entity/cgamerules.h>
#include <core/sdk/entity/cteam.h>
#include <core/sdk/cvars.hpp>
#include <dynlibutils/module.hpp>
#include <dynlibutils/virtual.hpp>
#include <dynlibutils/vthook.hpp>
#include <eiface.h>
#include <engine/igameeventsystem.h>
#include <entity2/entitysystem.h>
#include <igameevents.h>
#include <iserver.h>
#include <netmessages.h>
#include <steam_gameserver.h>
#include <vscript/ccscript.h>
#include <vscript_server.h>

#include "plugin.hpp"

#include "con_command_manager.hpp"
#include "con_var_manager.hpp"
#include "core_config.hpp"
#include "event_listener.hpp"
#include "event_manager.hpp"
#include "hook_manager.hpp"
#include "listeners.hpp"
#include "multi_addon_manager.hpp"
#include "output_manager.hpp"
#include "panorama_vote.hpp"
#include "player_manager.hpp"
#include "server_manager.hpp"
#include "timer_system.hpp"
#include "user_message_manager.hpp"

Source2SDK g_sdk;
EXPOSE_PLUGIN(PLUGIN_API, Source2SDK, &g_sdk)

CGameEntitySystem* GameEntitySystem() {
	TRY_GET_OFFSET(g_pGameConfig, "GameEntitySystem", offset);
	return *reinterpret_cast<CGameEntitySystem**>(reinterpret_cast<uintptr_t>(g_pGameResourceServiceServer) + *offset);
}

namespace {
constexpr char CS_SCRIPT_PATH[] = "maps/editor/zoo/scripts/hello.vjs";

polyhook::ResultType Hook_StartupServer(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	//auto config = polyhook::GetArgument<const GameSessionConfiguration_t *>(params, 1);
	//auto pWorldSession = polyhook::GetArgument<ISource2WorldSession*>(params, 2);
	auto pMapName = polyhook::GetArgument<const char*>(params, 3);

	plg::print(LS_DETAILED, "[StartupServer] = {}\n", pMapName);

	g_sdk.OnServerStartup();

	if (gpGlobals == nullptr) {
		plg::print(LS_ERROR, "Failed to lookup gpGlobals\n");
		return polyhook::ResultType::Ignored;
	}

	g_OnServerStartupListenerManager();

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_DisconnectGameNow(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	auto reason = (ENetworkDisconnectionReason) polyhook::GetArgument<int>(params, 1);

	if (reason == NETWORK_DISCONNECT_LOOPDEACTIVATE  || reason == NETWORK_DISCONNECT_EXITING) {
		static ENetworkDisconnectionReason lastReason = NETWORK_DISCONNECT_INVALID;

		if (reason < lastReason)
			return polyhook::ResultType::Ignored;

		lastReason = reason;

		g_TimerSystem.OnMapEnd();

		g_Precached.clear();

		g_OnMapEndListenerManager();
	}

	if (reason == NETWORK_DISCONNECT_REQUEST_HOSTSTATE_IDLE) {
		static std::once_flag flag;
		std::call_once(flag, g_OnServerStartedListenerManager);
	}

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_Release(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	auto server = polyhook::GetArgument<CNetworkGameServerBase*>(params, 0);

	if (server->m_nRefCount > 1)
		return polyhook::ResultType::Ignored;

	// g_HookManager.RemoveHookMemFunc(&CNetworkGameServerBase::Release, server);
	// g_HookManager.RemoveHookMemFunc(&CNetworkGameServerBase::ActivateServer, server);
	// g_HookManager.RemoveHookMemFunc(&CNetworkGameServerBase::SpawnServer, server);
	// g_HookManager.RemoveHookMemFunc(&CNetworkGameServerBase::StartChangeLevel, server);
	// g_HookManager.RemoveHookMemFunc(&CNetworkGameServerBase::FinishChangeLevel, server);

	return polyhook::ResultType::Ignored;
}

void LoadMOTDFile() {
	if (!g_pNetworkStringTableServer) {
		return;
	}

	INetworkStringTable* table = g_pNetworkStringTableServer->FindTable("InfoPanel");
	if (table == nullptr) {
		plg::print(LS_WARNING, "Failed to find table InfoPanel\n");
		return;
	}
	
	using namespace std::literals;
	auto motdCvar = g_pCVar->FindConVar("motdfile");
	auto motdFile = motdCvar.IsValidRef() ? cvars::GetConVarValue<CUtlString>(motdCvar) : "motd.txt"sv;
	auto motdPath = utils::GameDirectory() / S2SDK_GAME_NAME / motdFile;

	if (!fs::exists(motdPath)) {
		std::ofstream file(motdPath, std::ios::out | std::ios::trunc);
		file << "https://plugify.net/";
	}

	if (!fs::exists(motdPath)) {
		plg::print(LS_WARNING, "File not found at {}\n", plg::as_string(motdPath));
		return;
	}

	std::ifstream file(motdPath, std::ios::binary);
	if (!file.is_open()) {
		plg::print(LS_WARNING, "Failed to open {}\n", plg::as_string(motdPath));
		return;
	}

	std::string msg(
		(std::istreambuf_iterator(file)),
		std::istreambuf_iterator<char>()
	);

	SetStringUserDataRequest_t data {
		.m_pRawData = msg.data(),
		.m_cbDataSize = static_cast<uint32_t>(msg.length() + 1)
	};

	if (table->AddString(true, "motd", &data) != INVALID_STRING_INDEX) {
		plg::print(LS_WARNING, "Successfully added MOTD string\n");
	}
}

polyhook::ResultType Hook_ActivateServer(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	plg::print(LS_DETAILED, "[ActivateServer]\n");

#if defined (CS2)
	if (g_pCoreConfig->FixFlashAlertMessage) {
		g_TimerSystem.CreateTimer(0.5f, [](uint32_t, const plg::vector<plg::any>& data) {
			auto gameRules = *reinterpret_cast<CBaseGameRules**>(plg::get<void*>(data[0]));

			if (gameRules == nullptr || gameRules->m_bWarmupPeriod || gameRules->m_iRoundWinStatus > 0) {
				return;
			}

			gameRules->m_bGameRestart = gameRules->m_flRestartRoundTime == 0.0f;

		}, TimerFlag::Repeat | TimerFlag::NoMapChange, {&g_pGameRules});
	}
#endif

	if (g_pCoreConfig->FixLoadMotd) {
		LoadMOTDFile();
	}

	g_OnServerActivateListenerManager();
	g_OnMapStartListenerManager();

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_SpawnServer(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	plg::print(LS_DETAILED, "[SpawnServer]\n");

	g_OnServerSpawnListenerManager();

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_FireEvent(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	auto event = polyhook::GetArgument<IGameEvent*>(params, 1);
	auto dontBroadcast = polyhook::GetArgument<bool>(params, 2);

	/*if (event) {
		plg::print(LS_DETAILED, "[FireEvent] = {}\n", event->GetName());
	}*/

	auto result = type == polyhook::CallbackType::Post ? g_EventManager.OnFireEvent_Post(event, dontBroadcast) : g_EventManager.OnFireEvent(event, dontBroadcast);
	if (result >= ResultType::Handled) {
		return polyhook::ResultType::Supercede;
	}

	if (result == ResultType::Changed) {
		polyhook::SetArgument<bool>(params, 2, !dontBroadcast);
		return polyhook::ResultType::Handled;
	}

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_PostEvent(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	//auto slot = polyhook::GetArgument<int>(params, 1);
	//auto localOnly = polyhook::GetArgument<bool>(params, 2);
	//auto clientCount = polyhook::GetArgument<int>(params, 3);
	auto clients = polyhook::GetArgument<uint64_t*>(params, 4);
	auto message = polyhook::GetArgument<INetworkMessageInternal*>(params, 5);
	auto pData = polyhook::GetArgument<CNetMessage*>(params, 6);

	if (clients == nullptr) {
		return polyhook::ResultType::Ignored;
	}

	//plg::print(LS_DETAILED, "[PostEvent] = {}, {}, {}, {}\n", slot, localOnly, clientCount, static_cast<void*>(clients) );

#if defined (CS2)
	if (type == polyhook::CallbackType::Pre) {
		g_MultiAddonManager.OnPostEvent(message, pData, clients);
	}
#endif

	auto result = g_UserMessageManager.ExecuteMessageCallbacks(message, pData, clients, static_cast<HookMode>(type));
	if (result >= ResultType::Handled) {
		return polyhook::ResultType::Supercede;
	}

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_GameFrame(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// bool simulating, bool bFirstTick, bool bLastTick
	auto simulating = polyhook::GetArgument<bool>(params, 1);
	auto bFirstTick = polyhook::GetArgument<bool>(params, 2);
	auto bLastTick = polyhook::GetArgument<bool>(params, 3);

#if defined (CS2)
	g_MultiAddonManager.OnGameFrame();
#endif
	g_ServerManager.OnGameFrame();
	g_PlayerManager.OnGameFrame();
	g_TimerSystem.OnGameFrame(simulating);

	g_OnGameFrameListenerManager(simulating, bFirstTick, bLastTick);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_ClientActive(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// CPlayerSlot slot, bool bLoadGame, const char* name, uint64 steamID64
	auto slot = polyhook::GetArgument<int>(params, 1);
	auto bLoadGame = polyhook::GetArgument<bool>(params, 2);
	auto name = polyhook::GetArgument<const char*>(params, 3);
	auto steamID64 = (uint64) polyhook::GetArgument<uint64_t>(params, 4);

	plg::print(LS_DETAILED, "[OnClientActive] = {}, {}, {}\n", slot, name, steamID64);

#if defined (CS2)
	g_MultiAddonManager.OnClientActive(slot, bLoadGame, name, steamID64);
#endif
	g_PlayerManager.OnClientActive(slot, bLoadGame);

	g_OnClientActiveListenerManager(slot, bLoadGame);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_ClientDisconnect(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// CPlayerSlot slot, int reason, const char* name, uint64 steamID64, const char* networkID
	auto slot = polyhook::GetArgument<int>(params, 1);
	auto reason = (ENetworkDisconnectionReason) polyhook::GetArgument<int>(params, 2);
	auto name = polyhook::GetArgument<const char*>(params, 3);
	auto steamID64 = polyhook::GetArgument<uint64_t>(params, 4);
	auto networkID = polyhook::GetArgument<const char*>(params, 5);

	plg::print(LS_DETAILED, "[ClientDisconnect] = {}, {}, {}, {}, {}\n", slot,  static_cast<int>(reason), name, steamID64, networkID);

	if (type == polyhook::CallbackType::Pre) {
#if defined (CS2)
		g_MultiAddonManager.OnClientDisconnect(slot, name, steamID64, networkID);
#endif
		g_PlayerManager.OnClientDisconnect(slot, reason);
	} else {
		g_PlayerManager.OnClientDisconnect_Post(slot, reason);
		g_PanoramaVoteHandler.RemovePlayerFromVote(slot);
	}

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_ClientPutInServer(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// CPlayerSlot slot, char const *name, int type, uint64 steamID64
	auto slot = polyhook::GetArgument<int>(params, 1);
	auto name = polyhook::GetArgument<const char*>(params, 2);
	auto conType = polyhook::GetArgument<int>(params, 3);
	auto steamID64 = polyhook::GetArgument<uint64_t>(params, 4);

	plg::print(LS_DETAILED, "[ClientPutInServer] = {}, {}, {}, {}\n", slot, name, conType, steamID64);

	g_PlayerManager.OnClientPutInServer(slot, name);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_ClientSettingsChanged(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// CPlayerSlot slot
	auto slot = polyhook::GetArgument<int>(params, 1);

	plg::print(LS_DETAILED, "[ClientSettingsChanged] = {}\n", slot);

	g_OnClientSettingsChangedListenerManager(slot);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_OnClientConnected(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// CPlayerSlot slot, const cha*r name, uint64 steamID64, const char* networkID, const char* pszAddress, bool bFakePlayer
	auto slot = polyhook::GetArgument<int>(params, 1);
	auto name = polyhook::GetArgument<const char*>(params, 2);
	auto steamID64 = polyhook::GetArgument<uint64_t>(params, 3);
	auto networkID = polyhook::GetArgument<const char*>(params, 4);
	auto pszAddress = polyhook::GetArgument<const char*>(params, 5);
	auto fakePlayer = polyhook::GetArgument<bool>(params, 6);

	plg::print(LS_DETAILED, "[OnClientConnected] = {}, {}, {}, {}, {}, {}\n", slot, name, steamID64, networkID, pszAddress, fakePlayer);

	g_PlayerManager.OnClientConnected(slot, fakePlayer);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_ClientFullyConnect(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// CPlayerSlot slot
	auto slot = polyhook::GetArgument<int>(params, 1);

	plg::print(LS_DETAILED, "[ClientFullyConnect] = {}\n", slot);

	g_OnClientFullyConnectListenerManager(slot);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_ClientConnect(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// CPlayerSlot slot, const char* name, uint64 steamID64, const char* networkID, bool unk1, CBufferString *pRejectReason
	auto slot = polyhook::GetArgument<int>(params, 1);
	auto name = polyhook::GetArgument<const char*>(params, 2);
	auto steamID64 = polyhook::GetArgument<uint64_t>(params, 3);
	auto networkID = polyhook::GetArgument<const char*>(params, 4);
	bool unk1 = polyhook::GetArgument<bool>(params, 5);
	auto pRejectReason = polyhook::GetArgument<CBufferString*>(params, 6);

	plg::print(LS_DETAILED, "[ClientConnect] = {}, {}, {}, {}, {}, {} \n", slot, name, steamID64, networkID, unk1, pRejectReason->Get());

	if (type == polyhook::CallbackType::Pre) {
#if defined (CS2)
		g_MultiAddonManager.OnClientConnect(slot, name, steamID64, networkID);
#endif
		g_PlayerManager.OnClientConnect(slot, name, steamID64, networkID);
	} else {
		bool origRet = polyhook::GetReturn<bool>(ret);
		bool newRet = g_PlayerManager.OnClientConnect_Post(slot, origRet);
		if (newRet != origRet) {
			polyhook::SetReturn<bool>(ret, newRet);
			return polyhook::ResultType::Override;
		}
	}

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_ClientCommand(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// CPlayerSlot slot, const CCommand& _cmd
	auto slot = polyhook::GetArgument<int>(params, 1);
	auto args = polyhook::GetArgument<const CCommand*>(params, 2);
	if (args == nullptr) {
		return polyhook::ResultType::Ignored;
	}

	plg::print(LS_DETAILED, "[ClientCommand] = {}, {}\n", slot, args->GetCommandString());

	const char* cmd = args->Arg(0);

	auto result = g_ConCommandManager.ExecuteCommandCallbacks(cmd, CCommandContext(CommandTarget_t::CT_NO_TARGET, slot), *args, HookMode::Pre, CommandCallingContext::Console);
	if (result >= ResultType::Handled) {
		return polyhook::ResultType::Supercede;
	}

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_GameServerSteamAPIActivated(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	plg::print(LS_DETAILED, "[GameServerSteamAPIActivated]\n");

	g_pSteam = SteamGameServer();
	g_pSteamUGC = SteamGameServerUGC();

	if (!g_pSteam || !g_pSteamUGC) {
		plg::print(LS_WARNING, "StramAPI could not be initialized!\n");
		return polyhook::ResultType::Ignored;
	}

	g_PlayerManager.OnSteamAPIActivated();
#if defined (CS2)
	g_MultiAddonManager.OnSteamAPIActivated();
#endif

	//g_OnGameServerSteamAPIActivatedListenerManager();
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_GameServerSteamAPIDeactivated(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	plg::print(LS_DETAILED, "[GameServerSteamAPIDeactivated]\n");

	g_PlayerManager.OnSteamAPIDeactivated();
#if defined (CS2)
	g_MultiAddonManager.OnSteamAPIDeactivated();
#endif

	//g_OnGameServerSteamAPIDeactivatedListenerManager();
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_UpdateWhenNotInGame(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// float flFrameTime
	auto frameTime = polyhook::GetArgument<float>(params, 1);
	//plg::print(LS_DETAILED, "UpdateWhenNotInGame = {}\n", frameTime);
	g_OnUpdateWhenNotInGameListenerManager(frameTime);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_PreWorldUpdate(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// bool simulating
	auto simulating = polyhook::GetArgument<bool>(params, 1);
	//plg::print(LS_DETAILED, "PreWorldUpdate = {}\n", simulating);

	g_ServerManager.OnPreWorldUpdate();

	g_OnPreWorldUpdateListenerManager(simulating);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_FireOutputInternal(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// CEntityIOOutput* const self, CEntityInstance* activator, CEntityInstance* caller, const CVariant* const value, float delay
	auto self = polyhook::GetArgument<CEntityIOOutput* const>(params, 0);
	auto activator = polyhook::GetArgument<CEntityInstance*>(params, 1);
	auto caller = polyhook::GetArgument<CEntityInstance*>(params, 2);
	//auto value = polyhook::GetArgument<const CVariant* const>(params, 3);
	auto delay = polyhook::GetArgument<float>(params, 6);

	ResultType result = type == polyhook::CallbackType::Post ?
		g_EntityOutputManager.FireOutputInternal_Post(self, activator, caller, delay) :
		g_EntityOutputManager.FireOutputInternal(self, activator, caller, delay);

	if (result >= ResultType::Handled) {
		return polyhook::ResultType::Supercede;
	}

	return polyhook::ResultType::Ignored;
}

#if defined (CS2)
polyhook::ResultType Hook_TerminateRound(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	//auto pGameRules = polyhook::GetArgument<CCSGameRules*>(params, 0);
	auto delay = polyhook::GetArgument<float>(params, 1);
	auto reason = static_cast<CRoundEndReason>(polyhook::GetArgument<uint32_t>(params, 2));

	if (g_pCoreConfig->FixFlashAlertMessage) {
		g_pGameRules->m_bGameRestart = false;
	}

	g_OnRoundTerminatedListenerManager(delay, reason);

	return polyhook::ResultType::Ignored;
}
#endif

polyhook::ResultType Hook_DispatchConCommand(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// auto cmd = polyhook::GetArgument<ConCommandRef* const>(params, 1);
	auto ctx = polyhook::GetArgument<const CCommandContext*>(params, 2);
	auto args = polyhook::GetArgument<const CCommand*>(params, 3);
	auto result = g_ConCommandManager.DispatchConCommand(ctx, args, static_cast<HookMode>(type));
	if (result >= ResultType::Handled) {
		return polyhook::ResultType::Supercede;
	}

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_CheckTransmit(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	auto ppInfoList = polyhook::GetArgument<CCheckTransmitInfo**>(params, 1);
	auto nInfoCount = polyhook::GetArgument<uint32_t>(params, 2);

	plg::view view(ppInfoList, nInfoCount);

	g_OnServerCheckTransmitListenerManager(view.get());

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_CallGlobalChangeCallbacks(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	// auto cmd = polyhook::GetArgument<ICvar* const>(params, 0);
	auto ref = polyhook::GetArgument<ConVarRefAbstract*>(params, 1);
	//auto nSlot = polyhook::GetArgument<CSplitScreenSlot>(params, 2);
	auto newValue = polyhook::GetArgument<const char*>(params, 3);
	auto oldValue = polyhook::GetArgument<const char*>(params, 4);

	g_ConVarManager.OnCvarsChanged(ref, newValue, oldValue);

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_HostStateRequest(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
#if defined (CS2)
	auto mgr = polyhook::GetArgument<CHostStateMgr*>(params, 0);
	auto request = polyhook::GetArgument<CHostStateRequest*>(params, 1);

	g_MultiAddonManager.OnHostStateRequest(mgr, request);
#endif

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_ReplyConnection(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
#if defined (CS2)
	auto server = polyhook::GetArgument<CNetworkGameServerBase*>(params, 0);
	auto client = polyhook::GetArgument<CServerSideClient*>(params, 1);

	type == polyhook::CallbackType::Post ? g_MultiAddonManager.OnReplyConnection_Post(server, client) : g_MultiAddonManager.OnReplyConnection(server, client);
#endif

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_SendNetMessage(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
#if defined (CS2)
	auto client = polyhook::GetArgument<CServerSideClient*>(params, 0);
	auto data = polyhook::GetArgument<CNetMessage*>(params, 1);
	auto bufType = (NetChannelBufType_t) polyhook::GetArgument<int8_t>(params, 2);

	if (client->IsFakeClient() || client->IsHLTV()) {
		// Don't send messages to fake clients or replay clients
		return polyhook::ResultType::Ignored;
	}

	g_MultiAddonManager.OnSendNetMessage(client, data, bufType);
#endif
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_OnAddEntity(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	auto handle = (CEntityHandle) polyhook::GetArgument<int>(params, 2);
	auto entity = polyhook::GetArgument<CBaseEntity*>(params, 1);
	std::string_view name(entity->GetClassname());
	if (name.ends_with("gamerules")) {
		g_pGameRulesProxy = static_cast<CBaseGameRulesProxy *>(entity);
		g_pGameRules = g_pGameRulesProxy->m_pGameRules;

#if defined (CS2)
		v8::Isolate* isolate = v8::Isolate::TryGetCurrent();
		v8::Locker locker(isolate);
		v8::Isolate::Scope isolateScope(isolate);
		v8::HandleScope handleScope(isolate);

		auto pPointScript = addresses::CreateEntityByName("point_script", -1);
		g_pPointScript = static_cast<CBaseEntity*>(pPointScript);
		g_pPointScript->DispatchSpawn({
			{"target_name", "script_main"},
			{"cs_script", CS_SCRIPT_PATH}
		});
#endif
		TRY_GET_OFFSET(g_pGameConfig, "CCSScript_EntityScript", offset);
		g_pScripts->AddToTail(reinterpret_cast<uint8_t*>(pPointScript) + *offset);
	} else if (name.ends_with("team_manager")) {
		g_pTeamManagers[entity->m_iTeamNum] = static_cast<CTeam *>(entity);
	}

	g_OnEntityCreatedListenerManager(handle.ToInt());
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_OnRemoveEntity(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	auto handle = (CEntityHandle) polyhook::GetArgument<int>(params, 2);
	auto entity = polyhook::GetArgument<CBaseEntity*>(params, 1);
	std::string_view name(entity->GetClassname());
	if (name.ends_with("gamerules")) {
		g_pGameRulesProxy = nullptr;
		g_pGameRules = nullptr;
	} else if (name.ends_with("team_manager")) {
		g_pTeamManagers.erase(entity->m_iTeamNum);
	}

	g_OnEntityDeletedListenerManager(handle.ToInt());
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_OnEntityParentChanged(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	auto entity = polyhook::GetArgument<CBaseEntity*>(params, 1);
	auto newParent = polyhook::GetArgument<CBaseEntity*>(params, 2);

	g_OnEntityParentChangedListenerManager(entity->GetRefEHandle().ToInt(), newParent ? newParent->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_ProcessRespondCvarValue(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	auto pClient = polyhook::GetArgument<CServerSideClientBase*>(params, 0);
	auto pMsg = polyhook::GetArgument<CCLCMsg_RespondCvarValue_t*>(params, 1);

	g_PlayerManager.OnRespondCvarValue(pClient, *pMsg);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_BuildGameSessionManifest(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	//auto pSystem = polyhook::GetArgument<IGameSystem*>(params, 0);
	auto pMsg = polyhook::GetArgument<EventBuildGameSessionManifest_t*>(params, 1);

	g_OnBuildGameSessionManifestListenerManager();

	pMsg->m_pResourceManifest->AddResource(CS_SCRIPT_PATH);
	for (const auto& resource : g_Precached) {
		pMsg->m_pResourceManifest->AddResource(resource.c_str());
	}

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_RegisterFunction(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	g_pScriptVM = polyhook::GetArgument<IScriptVM*>(params, 0);
	auto pBinding = polyhook::GetArgument<ScriptFunctionBinding_t*>(params, 1);
	vscript::RegisterFunction(pBinding);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_RegisterScriptClass(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	g_pScriptVM = polyhook::GetArgument<IScriptVM*>(params, 0);
	auto pClassDesc = polyhook::GetArgument<ScriptClassDesc_t*>(params, 1);
	vscript::RegisterScriptClass(pClassDesc);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_RegisterInstance(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	g_pScriptVM = polyhook::GetArgument<IScriptVM*>(params, 0);
	auto pClassDesc = polyhook::GetArgument<ScriptClassDesc_t*>(params, 1);
	auto pInstance = polyhook::GetArgument<void*>(params, 2);
	vscript::RegisterScriptClass(pClassDesc, pInstance);
	return polyhook::ResultType::Ignored;
}
/*
plg::flat_hash_map<HSCRIPT, const char*> scriptFunctionMap = {};

polyhook::ResultType Hook_LookupFunction(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	//g_pScriptVM = polyhook::GetArgument<IScriptVM*>(params, 0);
	auto pszFunction = polyhook::GetArgument<const char *>(params, 1);
	//auto hScope = polyhook::GetArgument<HSCRIPT>(params, 2);
	//auto raw = polyhook::GetArgument<bool>(params, 3);
	auto hScript = polyhook::GetReturn<HSCRIPT>(ret);
	scriptFunctionMap[hScript] = pszFunction;
	plg::print(LS_MESSAGE, "LookupFunction - {}\n", pszFunction);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_ReleaseFunction(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	//g_pScriptVM = polyhook::GetArgument<IScriptVM*>(params, 0);
	auto hScript = polyhook::GetArgument<HSCRIPT>(params, 1);
	//scriptFunctionMap.erase(hScript);
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_ExecuteFunction(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	//g_pScriptVM = polyhook::GetArgument<IScriptVM*>(params, 0);
	auto hFunction = polyhook::GetArgument<HSCRIPT>(params, 1);
	auto pArgs = polyhook::GetArgument<ScriptVariant_t*>(params, 2);
	//auto nArgs = polyhook::GetArgument<int>(params, 3);
	//auto pReturn = polyhook::GetArgument<ScriptVariant_t*>(params, 4);
	auto hScope = polyhook::GetArgument<HSCRIPT>(params, 5);
	//auto bWait = polyhook::GetArgument<bool>(params, 6);

	std::string_view current = scriptFunctionMap[hFunction];

	plg::print(LS_MESSAGE, "ExecuteFunction - {}\n", current);

	CBaseEntity* entity = nullptr;
	ScriptVariant_t thisEntity;
	if (g_pScriptVM->GetValue(hScope, "thisEntity", &thisEntity)) {
		entity = (CBaseEntity*)g_pScriptVM->GetInstanceValue(thisEntity);
	}
	if (entity == nullptr) {
		return polyhook::ResultType::Ignored;
	}

	if (current == "OnSpawn") {
		CScriptKeyValues* spawnkeys = reinterpret_cast<CScriptKeyValues*>(g_pScriptVM->GetInstanceValue(pArgs[0]));

		//g_OnEntitySpawnListenerManager(entity->GetRefEHandle().ToInt(), spawnkeys->m_pKeyValues);

	} else if (currentlyExecutingScriptFunction == "OnDelete") {
	} else if (current == "DispatchPrecache") {
		CScriptPrecacheContext* context = reinterpret_cast<CScriptPrecacheContext*>(g_pScriptVM->GetInstanceValue(pArgs[0]));
		auto funcs = GetOnEntityPrecacheListenerManager().Get();
		for (const auto& func : funcs) {
			auto precached = func(entity->GetRefEHandle().ToInt());
			for (const auto& precache : precached) {
				context->m_pContext->m_pManifest->AddResource(precache.c_str());
			}
		}
		if (context != nullptr) {
		}
	}

	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_SetValue(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	auto pScript = polyhook::GetArgument<IScriptVM*>(params, 0);
	auto value = polyhook::GetArgument<const ScriptVariant_t*>(params, 3);
	//vscript::SetValue(pScript, *value);
	return polyhook::ResultType::Ignored;
}
*/
#if defined (CS2)
polyhook::ResultType Hook_IsolateEnter(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	auto isolate = polyhook::GetArgument<v8::Isolate*>(params, 0);
	isolate->SetData(v8::Isolate::GetNumberOfDataSlots() - 2, new v8::Locker(isolate));
	return polyhook::ResultType::Ignored;
}

polyhook::ResultType Hook_IsolateExit(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	auto isolate = polyhook::GetArgument<v8::Isolate*>(params, 0);
	delete reinterpret_cast<v8::Locker*>(isolate->GetData(v8::Isolate::GetNumberOfDataSlots() - 2));
	isolate->SetData(v8::Isolate::GetNumberOfDataSlots() - 2, nullptr);
	return polyhook::ResultType::Ignored;
}
#endif

#if _WIN32

#if PLUGIFY_ARCH_BITS == 64
constexpr WORD PE_FILE_MACHINE = IMAGE_FILE_MACHINE_AMD64;
constexpr WORD PE_NT_OPTIONAL_HDR_MAGIC = IMAGE_NT_OPTIONAL_HDR64_MAGIC;
#else
constexpr WORD PE_FILE_MACHINE = IMAGE_FILE_MACHINE_I386;
constexpr WORD PE_NT_OPTIONAL_HDR_MAGIC = IMAGE_NT_OPTIONAL_HDR32_MAGIC;
#endif // PLUGIFY_ARCH_BITS

polyhook::ResultType Hook_PreloadLibrary(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type) {
	HMODULE hModule = (HMODULE) polyhook::GetArgument<void*>(params, 0);

	IMAGE_DOS_HEADER* pDOSHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(hModule);
	IMAGE_NT_HEADERS* pNTHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<uintptr_t>(hModule) + pDOSHeader->e_lfanew);
	IMAGE_OPTIONAL_HEADER* pOptionalHeader = &pNTHeaders->OptionalHeader;

	IMAGE_DATA_DIRECTORY clrRuntimeHeader = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];
	if (clrRuntimeHeader.VirtualAddress != 0 && clrRuntimeHeader.Size != 0) {
		return polyhook::ResultType::Supercede;
	}

	return polyhook::ResultType::Ignored;
}
#else
#include <dlfcn.h>
#endif
}

void Source2SDK::OnPluginStart() {
	plg::print(LS_DETAILED, "[OnPluginStart] - Source2SDK!\n");

	globals::Initialize({
		{ "base", g_sdk.GetLocation() },
		{ "configs", plg::GetConfigsDir() },
		{ "data", plg::GetDataDir() },
	});

	using enum polyhook::CallbackType;

	g_HookManager.AddHookVTableFunc(&IGameEventManager2::FireEvent, g_pGameEventManager, Hook_FireEvent, Pre, Post);
	using PostEventAbstract = void(IGameEventSystem::*)( CSplitScreenSlot slot, bool bLocalOnly, int nClientCount, const uint64 *clients, INetworkMessageInternal *pEvent, const CNetMessage *pData, unsigned long nSize, NetChannelBufType_t bufType);
	g_HookManager.AddHookVTableFunc<PostEventAbstract>(&IGameEventSystem::PostEventAbstract, g_pGameEventSystem, Hook_PostEvent, Pre, Post);

	g_HookManager.AddHookVTableFunc(&IServerGameClients::ClientCommand, g_pSource2GameClients, Hook_ClientCommand, Pre);
	g_HookManager.AddHookVTableFunc(&IServerGameClients::ClientActive, g_pSource2GameClients, Hook_ClientActive, Post);
	g_HookManager.AddHookVTableFunc(&IServerGameClients::ClientDisconnect, g_pSource2GameClients, Hook_ClientDisconnect, Pre, Post);
	g_HookManager.AddHookVTableFunc(&IServerGameClients::ClientPutInServer, g_pSource2GameClients, Hook_ClientPutInServer, Post);
	g_HookManager.AddHookVTableFunc(&IServerGameClients::ClientSettingsChanged, g_pSource2GameClients, Hook_ClientSettingsChanged, Post);
	g_HookManager.AddHookVTableFunc(&IServerGameClients::OnClientConnected, g_pSource2GameClients, Hook_OnClientConnected, Post);
	g_HookManager.AddHookVTableFunc(&IServerGameClients::ClientFullyConnect, g_pSource2GameClients, Hook_ClientFullyConnect, Post);
	g_HookManager.AddHookVTableFunc(&IServerGameClients::ClientConnect, g_pSource2GameClients, Hook_ClientConnect, Pre, Post);
	g_HookManager.AddHookVTableFunc(&INetworkServerService::StartupServer, g_pNetworkServerService, Hook_StartupServer, Post);
	g_HookManager.AddHookVTableFunc(&INetworkServerService::DisconnectGameNow, g_pNetworkServerService, Hook_DisconnectGameNow, Post);
	g_HookManager.AddHookVTableFunc(&ISource2Server::GameServerSteamAPIActivated, g_pSource2Server, Hook_GameServerSteamAPIActivated, Post);
	g_HookManager.AddHookVTableFunc(&ISource2Server::GameServerSteamAPIDeactivated, g_pSource2Server, Hook_GameServerSteamAPIDeactivated, Post);
	g_HookManager.AddHookVTableFunc(&ISource2Server::UpdateWhenNotInGame, g_pSource2Server, Hook_UpdateWhenNotInGame, Post);
	g_HookManager.AddHookVTableFunc(&ISource2Server::PreWorldUpdate, g_pSource2Server, Hook_PreWorldUpdate, Post);
	g_HookManager.AddHookVTableFunc(&IServerGameDLL::GameFrame, g_pSource2Server, Hook_GameFrame, Post);
	g_HookManager.AddHookVTableFunc(&ICvar::DispatchConCommand, g_pCVar, Hook_DispatchConCommand, Pre, Post);
	g_HookManager.AddHookVTableFunc(&ICvar::CallGlobalChangeCallbacks, g_pCVar, Hook_CallGlobalChangeCallbacks, Post);
	g_HookManager.AddHookVTableFunc(&ISource2GameEntities::CheckTransmit, g_pSource2GameEntities, Hook_CheckTransmit, Post);

	//using LogDirect = LoggingResponse_t (*)(void* loggingSystem, LoggingChannelID_t channel, LoggingSeverity_t severity, LeafCodeInfo_t*, LoggingMetaData_t*, Color, char const*, va_list*);
	//g_HookManager.AddHookDetourFunc<LogDirect>("LogDirect", Hook_LogDirect, Pre);

	TRY_GET_VTABLE(g_pGameConfig, "CLuaVM", CLuaVM);
	g_HookManager.AddHookVFuncFunc(&IScriptVM::RegisterFunction, &CLuaVM, Hook_RegisterFunction, Pre);
	g_HookManager.AddHookVFuncFunc(&IScriptVM::RegisterScriptClass, &CLuaVM, Hook_RegisterScriptClass, Pre);
	using RegisterInstanceFn = HSCRIPT(IScriptVM::*)(ScriptClassDesc_t *pDesc, void *pInstance);
	g_HookManager.AddHookVFuncFunc<RegisterInstanceFn>(&IScriptVM::RegisterInstance, &CLuaVM, Hook_RegisterInstance, Pre);
	//using SetValueFn = bool(IScriptVM::*)(HSCRIPT hScope, const char *pszKey, const ScriptVariant_t &value);
	//g_HookManager.AddHookVFuncFunc<SetValueFn>(&IScriptVM::SetValue, &*table3, Hook_SetValue, Pre);
	//g_HookManager.AddHookVFuncFunc(&IScriptVM::LookupFunction, &*table3, Hook_LookupFunction, Post);
	//g_HookManager.AddHookVFuncFunc(&IScriptVM::ReleaseFunction, &*table3, Hook_ReleaseFunction, Pre);
	//g_HookManager.AddHookVFuncFunc(&IScriptVM::ExecuteFunction, &*table3, Hook_ExecuteFunction, Pre);

#if defined (CS2)
	using HostStateRequestFn = void* (*)(CHostStateMgr *manager, CHostStateRequest* request);
	g_HookManager.AddHookDetourFunc<HostStateRequestFn>("CHostStateMgr::StartNewRequest", Hook_HostStateRequest, Pre);
	using ReplyConnectionFn = void (*)(CNetworkGameServerBase *server, CServerSideClient* client);
	g_HookManager.AddHookDetourFunc<ReplyConnectionFn>("CNetworkGameServer::ReplyConnection", Hook_ReplyConnection, Pre, Post);
	using FireOutputInternalFn = uint64_t(*)(CEntityIOOutput*, CEntityInstance*, CEntityInstance*, const CVariant*, int32_t*, int16_t*, float);
	g_HookManager.AddHookDetourFunc<FireOutputInternalFn>("CEntityIOOutput::FireOutputInternal", Hook_FireOutputInternal, Pre, Post);

	TRY_GET_VTABLE(g_pGameConfig, "CServerSideClient", CServerSideClient);
	g_HookManager.AddHookVFuncFunc(&CServerSideClientBase::ProcessRespondCvarValue, &CServerSideClient, Hook_ProcessRespondCvarValue, Pre);
	g_HookManager.AddHookVFuncFunc(&CServerSideClientBase::SendNetMessage, &CServerSideClient, Hook_SendNetMessage, Pre);

	TRY_GET_VTABLE(g_pGameConfig, "CGameRulesGameSystem", CGameRulesGameSystem);
	g_HookManager.AddHookVFuncFunc(&IGameSystem::BuildGameSessionManifest, &CGameRulesGameSystem, Hook_BuildGameSessionManifest, Pre);

	using TerminateRoundFn = void(*)(CGameRules*, float, uint32_t, uint64_t, uint32_t);
	g_HookManager.AddHookDetourFunc<TerminateRoundFn>("CGameRules::TerminateRound", Hook_TerminateRound, Pre);

	using v8IsolateFn = void(*)(v8::Isolate*);

	void* v8IsolateEnterPtr;
	TRY_GET_ADDRESS(g_pGameConfig, "v8::Isolate::Enter", v8IsolateEnterPtr);

	void* v8IsolateExitPtr;
	TRY_GET_ADDRESS(g_pGameConfig, "v8::Isolate::Exit", v8IsolateExitPtr);

#if _WIN32
	const uint8_t fix = 0;
#else
	const uint8_t fix = 6; // skip plt staff
#endif

	g_HookManager.AddHookDetourFunc<v8IsolateFn>(reinterpret_cast<uintptr_t>(static_cast<uint8_t*>(v8IsolateEnterPtr) + fix), Hook_IsolateEnter, Pre);
	g_HookManager.AddHookDetourFunc<v8IsolateFn>(reinterpret_cast<uintptr_t>(static_cast<uint8_t*>(v8IsolateExitPtr) + fix), Hook_IsolateExit, Post);

	if (Module v8("plugify-module-v8"); v8.IsValid()) {
		using SetModuleResolverFn = void(*)(v8::Module::ResolveModuleCallback);
		auto resolve = v8.GetFunctionByName("SetModuleResolver").RCast<SetModuleResolverFn>();
		if (!resolve) {
			plg::print(LS_ERROR, "SetModuleResolver not found!\n");
			return;
		}
		resolve(addresses::CSScript_ResolveModule);
	}
#endif

#if _WIN32
	using PreloadLibrary = void(*)(void*);
	g_HookManager.AddHookDetourFunc<PreloadLibrary>("PreloadLibrary", Hook_PreloadLibrary, Pre);
#endif

	OnServerStartup();// for late load*/
}

void Source2SDK::OnPluginEnd() {
	globals::Terminate();
	g_HookManager.UnhookAll();
	g_PlayerManager.OnSteamAPIDeactivated();
#if defined (CS2)
	g_MultiAddonManager.OnSteamAPIDeactivated();
#endif
	UnregisterEventListeners();

	plg::print(LS_DETAILED, "[OnPluginEnd] = Source2SDK!\n");
}

void Source2SDK::OnServerStartup() {
	g_pNetworkGameServer = g_pNetworkServerService->GetIGameServer();

	if (g_pNetworkGameServer != nullptr) {
		gpGlobals = g_pNetworkGameServer->GetGlobals();
		g_HookManager.AddHookVTableFunc(&CNetworkGameServerBase::Release, g_pNetworkGameServer, Hook_Release, polyhook::CallbackType::Pre);
		g_HookManager.AddHookVTableFunc(&CNetworkGameServerBase::ActivateServer, g_pNetworkGameServer, Hook_ActivateServer, polyhook::CallbackType::Post);
		g_HookManager.AddHookVTableFunc(&CNetworkGameServerBase::SpawnServer, g_pNetworkGameServer, Hook_SpawnServer, polyhook::CallbackType::Post);
	}

	if (g_pGameEntitySystem != nullptr) {
		g_HookManager.RemoveHookVirtualFunc(&CEntitySystem::OnAddEntity, g_pGameEntitySystem);
		g_HookManager.RemoveHookVirtualFunc(&CEntitySystem::OnRemoveEntity, g_pGameEntitySystem);
		g_HookManager.RemoveHookVirtualFunc(&CEntitySystem::OnEntityParentChanged, g_pGameEntitySystem);
	}

	g_pGameEntitySystem = GameEntitySystem();

	if (g_pGameEntitySystem != nullptr) {
		g_HookManager.AddHookVTableFunc(&CEntitySystem::OnAddEntity, g_pGameEntitySystem, Hook_OnAddEntity, polyhook::CallbackType::Post);
		g_HookManager.AddHookVTableFunc(&CEntitySystem::OnRemoveEntity, g_pGameEntitySystem, Hook_OnRemoveEntity, polyhook::CallbackType::Post);
		g_HookManager.AddHookVTableFunc(&CEntitySystem::OnEntityParentChanged, g_pGameEntitySystem, Hook_OnEntityParentChanged, polyhook::CallbackType::Post);
	}

#if defined (CS2)
	g_MultiAddonManager.OnStartupServer();
#endif

	RegisterEventListeners();
}

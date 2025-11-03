#include <core/sdk/entity/cgamerules.h>
#include <core/sdk/entity/cteam.h>
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
void* string_S_empty_rep_storage = nullptr;
void* wstring_S_empty_rep_storage = nullptr;

constexpr char CS_SCRIPT_PATH[] = "maps/editor/zoo/scripts/hello.vjs";

poly::ReturnAction Hook_StartupServer(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//auto config = poly::GetArgument<const GameSessionConfiguration_t *>(params, 1);
	//auto pWorldSession = poly::GetArgument<ISource2WorldSession*>(params, 2);
	auto pMapName = poly::GetArgument<const char*>(params, 3);

	plg::print(LS_DETAILED, "[StartupServer] = {}\n", pMapName);

	g_sdk.OnServerStartup();

	if (gpGlobals == nullptr) {
		plg::print(LS_ERROR, "Failed to lookup gpGlobals\n");
		return poly::ReturnAction::Ignored;
	}

	g_OnServerStartupListenerManager();

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_DisconnectGameNow(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto reason = (ENetworkDisconnectionReason) poly::GetArgument<int>(params, 1);

	if (reason == NETWORK_DISCONNECT_LOOPDEACTIVATE  || reason == NETWORK_DISCONNECT_EXITING) {
		static ENetworkDisconnectionReason lastReason = NETWORK_DISCONNECT_INVALID;

		if (reason < lastReason)
			return poly::ReturnAction::Ignored;

		lastReason = reason;

		g_TimerSystem.OnMapEnd();

		g_OnMapEndListenerManager();
	}

	if (reason == NETWORK_DISCONNECT_REQUEST_HOSTSTATE_IDLE) {
		ExecuteOnce( {
			g_OnServerStartedListenerManager();
		});
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_Release(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto server = poly::GetArgument<CNetworkGameServerBase*>(params, 1);

	if (server->m_nRefCount > 1)
		return poly::ReturnAction::Ignored;

	// g_HookManager.RemoveHookMemFunc(&CNetworkGameServerBase::Release, server);
	// g_HookManager.RemoveHookMemFunc(&CNetworkGameServerBase::ActivateServer, server);
	// g_HookManager.RemoveHookMemFunc(&CNetworkGameServerBase::SpawnServer, server);
	// g_HookManager.RemoveHookMemFunc(&CNetworkGameServerBase::StartChangeLevel, server);
	// g_HookManager.RemoveHookMemFunc(&CNetworkGameServerBase::FinishChangeLevel, server);

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ActivateServer(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	plg::print(LS_DETAILED, "[ActivateServer]\n");

#if defined (CS2)
	g_TimerSystem.CreateTimer(0.5f, [](uint32_t, const plg::vector<plg::any>& data) {
		auto gameRules = *(CBaseGameRules**)plg::get<void*>(data[0]);

		if (gameRules == nullptr || gameRules->m_bWarmupPeriod || gameRules->m_iRoundWinStatus > 0) {
			return;
		}

		gameRules->m_bGameRestart = gameRules->m_flRestartRoundTime == 0.0f;

	}, TimerFlag::Repeat | TimerFlag::NoMapChange, {&g_pGameRules});
#endif

	g_Precached.clear();

	g_OnServerActivateListenerManager();
	g_OnMapStartListenerManager();

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_SpawnServer(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	plg::print(LS_DETAILED, "[SpawnServer]\n");

	g_OnServerSpawnListenerManager();

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_FireEvent(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto event = poly::GetArgument<IGameEvent*>(params, 1);
	auto dontBroadcast = poly::GetArgument<bool>(params, 2);

	/*if (event) {
		plg::print(LS_DETAILED, "[FireEvent] = {}\n", event->GetName());
	}*/

	auto result = type == poly::CallbackType::Post ? g_EventManager.OnFireEvent_Post(event, dontBroadcast) : g_EventManager.OnFireEvent(event, dontBroadcast);
	if (result >= ResultType::Handled) {
		return poly::ReturnAction::Supercede;
	}

	if (result == ResultType::Changed) {
		poly::SetArgument<bool>(params, 2, !dontBroadcast);
		return poly::ReturnAction::Handled;
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_PostEvent(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//auto slot = poly::GetArgument<int>(params, 1);
	//auto localOnly = poly::GetArgument<bool>(params, 2);
	//auto clientCount = poly::GetArgument<int>(params, 3);
	auto clients = poly::GetArgument<uint64_t*>(params, 4);
	auto message = poly::GetArgument<INetworkMessageInternal*>(params, 5);
	auto pData = poly::GetArgument<CNetMessage*>(params, 6);

	//plg::print(LS_DETAILED, "[PostEvent] = {}, {}, {}, {}\n", slot, localOnly, clientCount, static_cast<void*>(clients) );

	if (type == poly::CallbackType::Pre) {
		g_MultiAddonManager.OnPostEvent(message, pData, clients);
	}

	auto result = g_UserMessageManager.ExecuteMessageCallbacks(message, pData, clients, static_cast<HookMode>(type));
	if (result >= ResultType::Handled) {
		return poly::ReturnAction::Supercede;
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_GameFrame(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// bool simulating, bool bFirstTick, bool bLastTick
	auto simulating = poly::GetArgument<bool>(params, 1);
	auto bFirstTick = poly::GetArgument<bool>(params, 2);
	auto bLastTick = poly::GetArgument<bool>(params, 3);

	g_MultiAddonManager.OnGameFrame();
	g_ServerManager.OnGameFrame();
	g_TimerSystem.OnGameFrame(simulating);

	g_OnGameFrameListenerManager(simulating, bFirstTick, bLastTick);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ClientActive(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, bool bLoadGame, const char* name, uint64 steamID64
	auto slot = poly::GetArgument<int>(params, 1);
	auto bLoadGame = poly::GetArgument<bool>(params, 2);
	auto name = poly::GetArgument<const char*>(params, 3);
	auto steamID64 = (uint64) poly::GetArgument<uint64_t>(params, 4);

	plg::print(LS_DETAILED, "[OnClientActive] = {}, {}, {}\n", slot, name, steamID64);

	g_MultiAddonManager.OnClientActive(slot, bLoadGame, name, steamID64);
	g_PlayerManager.OnClientActive(slot, bLoadGame);

	g_OnClientActiveListenerManager(slot, bLoadGame);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ClientDisconnect(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, int reason, const char* name, uint64 steamID64, const char* networkID
	auto slot = poly::GetArgument<int>(params, 1);
	auto reason = (ENetworkDisconnectionReason) poly::GetArgument<int>(params, 2);
	auto name = poly::GetArgument<const char*>(params, 3);
	auto steamID64 = poly::GetArgument<uint64_t>(params, 4);
	auto networkID = poly::GetArgument<const char*>(params, 5);

	plg::print(LS_DETAILED, "[ClientDisconnect] = {}, {}, {}, {}, {}\n", slot,  static_cast<int>(reason), name, steamID64, networkID);

	if (type == poly::CallbackType::Pre) {
		g_MultiAddonManager.OnClientDisconnect(slot, name, steamID64, networkID);
		g_PlayerManager.OnClientDisconnect(slot, reason);
	} else {
		g_PlayerManager.OnClientDisconnect_Post(slot, reason);
		g_PanoramaVoteHandler.RemovePlayerFromVote(slot);
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ClientPutInServer(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, char const *name, int type, uint64 steamID64
	auto slot = poly::GetArgument<int>(params, 1);
	auto name = poly::GetArgument<const char*>(params, 2);
	auto conType = poly::GetArgument<int>(params, 3);
	auto steamID64 = poly::GetArgument<uint64_t>(params, 4);

	plg::print(LS_DETAILED, "[ClientPutInServer] = {}, {}, {}, {}\n", slot, name, conType, steamID64);

	g_PlayerManager.OnClientPutInServer(slot, name);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ClientSettingsChanged(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot
	auto slot = poly::GetArgument<int>(params, 1);

	plg::print(LS_DETAILED, "[ClientSettingsChanged] = {}\n", slot);

	g_OnClientSettingsChangedListenerManager(slot);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_OnClientConnected(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, const cha*r name, uint64 steamID64, const char* networkID, const char* pszAddress, bool bFakePlayer
	auto slot = poly::GetArgument<int>(params, 1);
	auto name = poly::GetArgument<const char*>(params, 2);
	auto steamID64 = poly::GetArgument<uint64_t>(params, 3);
	auto networkID = poly::GetArgument<const char*>(params, 4);
	auto pszAddress = poly::GetArgument<const char*>(params, 5);
	auto fakePlayer = poly::GetArgument<bool>(params, 6);

	plg::print(LS_DETAILED, "[OnClientConnected] = {}, {}, {}, {}, {}, {}\n", slot, name, steamID64, networkID, pszAddress, fakePlayer);

	g_PlayerManager.OnClientConnected(slot, fakePlayer);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ClientFullyConnect(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot
	auto slot = poly::GetArgument<int>(params, 1);

	plg::print(LS_DETAILED, "[ClientFullyConnect] = {}\n", slot);

	g_OnClientFullyConnectListenerManager(slot);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ClientConnect(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, const char* name, uint64 steamID64, const char* networkID, bool unk1, CBufferString *pRejectReason
	auto slot = poly::GetArgument<int>(params, 1);
	auto name = poly::GetArgument<const char*>(params, 2);
	auto steamID64 = poly::GetArgument<uint64_t>(params, 3);
	auto networkID = poly::GetArgument<const char*>(params, 4);
	bool unk1 = poly::GetArgument<bool>(params, 5);
	auto pRejectReason = poly::GetArgument<CBufferString*>(params, 6);

	plg::print(LS_DETAILED, "[ClientConnect] = {}, {}, {}, {}, {}, {} \n", slot, name, steamID64, networkID, unk1, pRejectReason->Get());

	if (type == poly::CallbackType::Pre) {
		g_MultiAddonManager.OnClientConnect(slot, name, steamID64, networkID);
		g_PlayerManager.OnClientConnect(slot, name, steamID64, networkID);
	} else {
		bool origRet = poly::GetReturn<bool>(ret);
		bool newRet = g_PlayerManager.OnClientConnect_Post(slot, origRet);
		if (newRet != origRet) {
			poly::SetReturn<bool>(ret, newRet);
			return poly::ReturnAction::Override;
		}
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ClientCommand(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, const CCommand& _cmd
	auto slot = poly::GetArgument<int>(params, 1);
	auto args = poly::GetArgument<const CCommand*>(params, 2);
	if (args == nullptr) {
		return poly::ReturnAction::Ignored;
	}

	plg::print(LS_DETAILED, "[ClientCommand] = {}, {}\n", slot, args->GetCommandString());

	const char* cmd = args->Arg(0);

	auto result = g_ConCommandManager.ExecuteCommandCallbacks(cmd, CCommandContext(CommandTarget_t::CT_NO_TARGET, slot), *args, HookMode::Pre, CommandCallingContext::Console);
	if (result >= ResultType::Handled) {
		return poly::ReturnAction::Supercede;
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_GameServerSteamAPIActivated(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	plg::print(LS_DETAILED, "[GameServerSteamAPIActivated]\n");

	g_SteamAPI.Init();
	//g_http = g_steamAPI.SteamHTTP();

	g_PlayerManager.OnSteamAPIActivated();
	g_MultiAddonManager.OnSteamAPIActivated();

	//g_OnGameServerSteamAPIActivatedListenerManager();
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_GameServerSteamAPIDeactivated(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	plg::print(LS_DETAILED, "[GameServerSteamAPIDeactivated]\n");

	//g_http = nullptr;

	//g_OnGameServerSteamAPIDeactivatedListenerManager();
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_UpdateWhenNotInGame(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// float flFrameTime
	auto frameTime = poly::GetArgument<float>(params, 1);
	//plg::print(LS_DETAILED, "UpdateWhenNotInGame = {}\n", frameTime);
	g_OnUpdateWhenNotInGameListenerManager(frameTime);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_PreWorldUpdate(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// bool simulating
	auto simulating = poly::GetArgument<bool>(params, 1);
	//plg::print(LS_DETAILED, "PreWorldUpdate = {}\n", simulating);

	g_ServerManager.OnPreWorldUpdate();

	g_OnPreWorldUpdateListenerManager(simulating);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_FireOutputInternal(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CEntityIOOutput* const self, CEntityInstance* activator, CEntityInstance* caller, const CVariant* const value, float delay
	auto self = poly::GetArgument<CEntityIOOutput* const>(params, 0);
	auto activator = poly::GetArgument<CEntityInstance*>(params, 1);
	auto caller = poly::GetArgument<CEntityInstance*>(params, 2);
	//auto value = poly::GetArgument<const CVariant* const>(params, 3);
	auto delay = poly::GetArgument<float>(params, 6);

	ResultType result = type == poly::CallbackType::Post ?
		g_EntityOutputManager.FireOutputInternal_Post(self, activator, caller, delay) :
		g_EntityOutputManager.FireOutputInternal(self, activator, caller, delay);

	if (result >= ResultType::Handled) {
		return poly::ReturnAction::Supercede;
	}

	return poly::ReturnAction::Ignored;
}

#if defined (CS2)
poly::ReturnAction Hook_TerminateRound(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//auto pGameRules = poly::GetArgument<CCSGameRules*>(params, 0);
	auto delay = poly::GetArgument<float>(params, 1);
	auto reason = static_cast<CSRoundEndReason>(poly::GetArgument<uint32_t>(params, 2));

	g_pGameRules->m_bGameRestart = false;

	g_OnRoundTerminatedListenerManager(delay, reason);

	return poly::ReturnAction::Ignored;
}
#endif

poly::ReturnAction Hook_DispatchConCommand(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// auto cmd = poly::GetArgument<ConCommandRef* const>(params, 1);
	auto ctx = poly::GetArgument<const CCommandContext*>(params, 2);
	auto args = poly::GetArgument<const CCommand*>(params, 3);
	auto result = g_ConCommandManager.DispatchConCommand(ctx, args, static_cast<HookMode>(type));
	if (result >= ResultType::Handled) {
		return poly::ReturnAction::Supercede;
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_CallGlobalChangeCallbacks(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// auto cmd = poly::GetArgument<ICvar* const>(params, 0);
	auto ref = poly::GetArgument<ConVarRefAbstract*>(params, 1);
	//auto nSlot = poly::GetArgument<CSplitScreenSlot>(params, 2);
	auto newValue = poly::GetArgument<const char*>(params, 3);
	auto oldValue = poly::GetArgument<const char*>(params, 4);

	g_ConVarManager.OnCvarsChanged(ref, newValue, oldValue);

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_HostStateRequest(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto mgr = poly::GetArgument<CHostStateMgr*>(params, 0);
	auto request = poly::GetArgument<CHostStateRequest*>(params, 1);

	g_MultiAddonManager.OnHostStateRequest(mgr, request);

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ReplyConnection(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto server = poly::GetArgument<CNetworkGameServerBase*>(params, 0);
	auto client = poly::GetArgument<CServerSideClient*>(params, 1);

	type == poly::CallbackType::Post ? g_MultiAddonManager.OnReplyConnection_Post(server, client) : g_MultiAddonManager.OnReplyConnection(server, client);

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_SendNetMessage(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto client = poly::GetArgument<CServerSideClient*>(params, 0);
	auto data = poly::GetArgument<CNetMessage*>(params, 1);
	auto bufType = (NetChannelBufType_t) poly::GetArgument<int8_t>(params, 2);

	if (client->IsFakeClient() || client->IsHLTV()) {
		// Don't send messages to fake clients or replay clients
		return poly::ReturnAction::Ignored;
	}

	g_MultiAddonManager.OnSendNetMessage(client, data, bufType);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_OnAddEntity(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto handle = (CEntityHandle) poly::GetArgument<int>(params, 2);
#if defined (CS2)
	auto entity = poly::GetArgument<CBaseEntity*>(params, 1);
	std::string_view name(entity->GetClassname());
	if (name == "cs_gamerules") {
		g_pGameRulesProxy = static_cast<CBaseGameRulesProxy *>(entity);
		g_pGameRules = g_pGameRulesProxy->m_pGameRules;

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
		TRY_GET_OFFSET(g_pGameConfig, "CCSScript_EntityScript", offset);
		g_pScripts->AddToTail(reinterpret_cast<uint8_t*>(pPointScript) + *offset);
	} else if (name == "cs_team_manager") {
		g_pTeamManagers[entity->m_iTeamNum] = static_cast<CTeam *>(entity);
	}
#endif

	g_OnEntityCreatedListenerManager(handle.ToInt());
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_OnRemoveEntity(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto handle = (CEntityHandle) poly::GetArgument<int>(params, 2);
#if defined (CS2)
	auto entity = poly::GetArgument<CBaseEntity*>(params, 1);
	std::string_view name(entity->GetClassname());
	if (name == "cs_gamerules") {
		g_pGameRulesProxy = nullptr;
		g_pGameRules = nullptr;
	} else if (name == "cs_team_manager") {
		g_pTeamManagers.erase(entity->m_iTeamNum);
	}
#endif

	g_OnEntityDeletedListenerManager(handle.ToInt());
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_OnEntityParentChanged(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto entity = poly::GetArgument<CBaseEntity*>(params, 1);
	auto newParent = poly::GetArgument<CBaseEntity*>(params, 2);

	g_OnEntityParentChangedListenerManager(entity->GetRefEHandle().ToInt(), newParent ? newParent->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ProcessRespondCvarValue(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto pClient = poly::GetArgument<CServerSideClientBase*>(params, 0);
	auto pMsg = poly::GetArgument<CCLCMsg_RespondCvarValue_t*>(params, 1);

	g_PlayerManager.OnRespondCvarValue(pClient, *pMsg);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_BuildGameSessionManifest(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//auto pSystem = poly::GetArgument<IGameSystem*>(params, 0);
	auto pMsg = poly::GetArgument<EventBuildGameSessionManifest_t*>(params, 1);

	pMsg->m_pResourceManifest->AddResource(CS_SCRIPT_PATH);
	for (const auto& resource : g_Precached) {
		pMsg->m_pResourceManifest->AddResource(resource.c_str());
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_RegisterFunction(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	g_pScriptVM = poly::GetArgument<IScriptVM*>(params, 0);
	auto pBinding = poly::GetArgument<ScriptFunctionBinding_t*>(params, 1);
	vscript::RegisterFunction(pBinding);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_RegisterScriptClass(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	g_pScriptVM = poly::GetArgument<IScriptVM*>(params, 0);
	auto pClassDesc = poly::GetArgument<ScriptClassDesc_t*>(params, 1);
	vscript::RegisterScriptClass(pClassDesc);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_RegisterInstance(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	g_pScriptVM = poly::GetArgument<IScriptVM*>(params, 0);
	auto pClassDesc = poly::GetArgument<ScriptClassDesc_t*>(params, 1);
	auto pInstance = poly::GetArgument<void*>(params, 2);
	vscript::RegisterScriptClass(pClassDesc, pInstance);
	return poly::ReturnAction::Ignored;
}
/*
plg::flat_hash_map<HSCRIPT, const char*> scriptFunctionMap = {};

poly::ReturnAction Hook_LookupFunction(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//g_pScriptVM = poly::GetArgument<IScriptVM*>(params, 0);
	auto pszFunction = poly::GetArgument<const char *>(params, 1);
	//auto hScope = poly::GetArgument<HSCRIPT>(params, 2);
	//auto raw = poly::GetArgument<bool>(params, 3);
	auto hScript = poly::GetReturn<HSCRIPT>(ret);
	scriptFunctionMap[hScript] = pszFunction;
	plg::print(LS_MESSAGE, "LookupFunction - {}\n", pszFunction);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ReleaseFunction(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//g_pScriptVM = poly::GetArgument<IScriptVM*>(params, 0);
	auto hScript = poly::GetArgument<HSCRIPT>(params, 1);
	//scriptFunctionMap.erase(hScript);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_ExecuteFunction(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//g_pScriptVM = poly::GetArgument<IScriptVM*>(params, 0);
	auto hFunction = poly::GetArgument<HSCRIPT>(params, 1);
	auto pArgs = poly::GetArgument<ScriptVariant_t*>(params, 2);
	//auto nArgs = poly::GetArgument<int>(params, 3);
	//auto pReturn = poly::GetArgument<ScriptVariant_t*>(params, 4);
	auto hScope = poly::GetArgument<HSCRIPT>(params, 5);
	//auto bWait = poly::GetArgument<bool>(params, 6);

	std::string_view current = scriptFunctionMap[hFunction];

	plg::print(LS_MESSAGE, "ExecuteFunction - {}\n", current);

	CBaseEntity* entity = nullptr;
	ScriptVariant_t thisEntity;
	if (g_pScriptVM->GetValue(hScope, "thisEntity", &thisEntity)) {
		entity = (CBaseEntity*)g_pScriptVM->GetInstanceValue(thisEntity);
	}
	if (entity == nullptr) {
		return poly::ReturnAction::Ignored;
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

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_SetValue(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto pScript = poly::GetArgument<IScriptVM*>(params, 0);
	auto value = poly::GetArgument<const ScriptVariant_t*>(params, 3);
	//vscript::SetValue(pScript, *value);
	return poly::ReturnAction::Ignored;
}
*/
#if defined (CS2)
poly::ReturnAction Hook_IsolateEnter(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto isolate = poly::GetArgument<v8::Isolate*>(params, 0);
	isolate->SetData(v8::Isolate::GetNumberOfDataSlots() - 2, new v8::Locker(isolate));
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Hook_IsolateExit(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto isolate = poly::GetArgument<v8::Isolate*>(params, 0);
	delete reinterpret_cast<v8::Locker*>(isolate->GetData(v8::Isolate::GetNumberOfDataSlots() - 2));
	isolate->SetData(v8::Isolate::GetNumberOfDataSlots() - 2, nullptr);
	return poly::ReturnAction::Ignored;
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

poly::ReturnAction Hook_PreloadLibrary(poly::PHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	HMODULE hModule = (HMODULE) poly::GetArgument<void*>(params, 0);

	IMAGE_DOS_HEADER* pDOSHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(hModule);
	IMAGE_NT_HEADERS* pNTHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<uintptr_t>(hModule) + pDOSHeader->e_lfanew);
	IMAGE_OPTIONAL_HEADER* pOptionalHeader = &pNTHeaders->OptionalHeader;

	IMAGE_DATA_DIRECTORY clrRuntimeHeader = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];
	if (clrRuntimeHeader.VirtualAddress != 0 && clrRuntimeHeader.Size != 0) {
		return poly::ReturnAction::Supercede;
	}

	return poly::ReturnAction::Ignored;
}
#else
#include <dlfcn.h>
/*using GetRepStorageFn = void*(*)();
extern "C" {
	void* __wrap__ZNSs12_S_empty_repEv() {
		if (string_S_empty_rep_storage == nullptr) {
			char path[MAX_PATH]{};
			strcpy(path, Plat_GetGameDirectory());
			strcat(path, "/bin/linuxsteamrt64/libtier0.so");
			void* tier0 = dlopen(path, RTLD_LAZY | RTLD_NOLOAD);
			string_S_empty_rep_storage = reinterpret_cast<GetRepStorageFn>(dlsym(tier0, "_ZNSs4_Rep12_S_empty_repEv"))();
			dlclose(tier0);
		}
		return string_S_empty_rep_storage;
	}
	void* __wrap__ZNSbIwSt11char_traitsIwESaIwEE12_S_empty_repEv() {
		if (wstring_S_empty_rep_storage == nullptr) {
			char path[MAX_PATH]{};
			strcpy(path, Plat_GetGameDirectory());
			strcat(path, "/bin/linuxsteamrt64/libtier0.so");
			void* tier0 = dlopen(path, RTLD_LAZY | RTLD_NOLOAD);
			wstring_S_empty_rep_storage = reinterpret_cast<GetRepStorageFn>(dlsym(tier0, "_ZNSbIwSt11char_traitsIwESaIwEE4_Rep12_S_empty_repEv"))();
			dlclose(tier0);
		}
		return wstring_S_empty_rep_storage;
	}
}*/
#endif
}

void Source2SDK::OnPluginStart() {
	plg::print(LS_DETAILED, "[OnPluginStart] - Source2SDK!\n");

	globals::Initialize({
		{ "base", g_sdk.GetLocation() },
		{ "configs", plg::GetConfigsDir() },
		{ "data", plg::GetDataDir() },
	});

	using enum poly::CallbackType;

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
	UnregisterEventListeners();

	plg::print(LS_DETAILED, "[OnPluginEnd] = Source2SDK!\n");
}

void Source2SDK::OnServerStartup() {
	g_pNetworkGameServer = g_pNetworkServerService->GetIGameServer();

	if (g_pNetworkGameServer != nullptr) {
		gpGlobals = g_pNetworkGameServer->GetGlobals();
		g_HookManager.AddHookVTableFunc(&CNetworkGameServerBase::Release, g_pNetworkGameServer, Hook_Release, poly::CallbackType::Pre);
		g_HookManager.AddHookVTableFunc(&CNetworkGameServerBase::ActivateServer, g_pNetworkGameServer, Hook_ActivateServer, poly::CallbackType::Post);
		g_HookManager.AddHookVTableFunc(&CNetworkGameServerBase::SpawnServer, g_pNetworkGameServer, Hook_SpawnServer, poly::CallbackType::Post);
	}

	if (g_pGameEntitySystem != nullptr) {
		g_HookManager.RemoveHookVirtualFunc(&CEntitySystem::OnAddEntity, g_pGameEntitySystem);
		g_HookManager.RemoveHookVirtualFunc(&CEntitySystem::OnRemoveEntity, g_pGameEntitySystem);
		g_HookManager.RemoveHookVirtualFunc(&CEntitySystem::OnEntityParentChanged, g_pGameEntitySystem);
	}

	g_pGameEntitySystem = GameEntitySystem();

	if (g_pGameEntitySystem != nullptr) {
		g_HookManager.AddHookVTableFunc(&CEntitySystem::OnAddEntity, g_pGameEntitySystem, Hook_OnAddEntity, poly::CallbackType::Post);
		g_HookManager.AddHookVTableFunc(&CEntitySystem::OnRemoveEntity, g_pGameEntitySystem, Hook_OnRemoveEntity, poly::CallbackType::Post);
		g_HookManager.AddHookVTableFunc(&CEntitySystem::OnEntityParentChanged, g_pGameEntitySystem, Hook_OnEntityParentChanged, poly::CallbackType::Post);
	}

	g_MultiAddonManager.OnStartupServer();

	RegisterEventListeners();
}


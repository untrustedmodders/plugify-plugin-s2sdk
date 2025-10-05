#include <core/sdk/entity/cgamerules.h>
#include <core/sdk/entity/cteam.h>
#include <dynlibutils/module.hpp>
#include <dynlibutils/virtual.hpp>
#include <dynlibutils/vthook.hpp>
#include <eiface.h>
#include <engine/igameeventsystem.h>
#include <entity2/entitysystem.h>
#include <vscript/ccscript.h>
#include <igameevents.h>
#include <iserver.h>
#include <netmessages.h>
#include <steam_gameserver.h>

#include "plugin.hpp"

#include "con_command_manager.hpp"
#include "con_var_manager.hpp"
#include "core_config.hpp"
#include "event_listener.hpp"
#include "event_manager.hpp"
#include "hook_holder.hpp"
#include "listeners.hpp"
#include "output_manager.hpp"
#include "panorama_vote.hpp"
#include "player_manager.hpp"
#include "server_manager.hpp"
#include "timer_system.hpp"
#include "user_message_manager.hpp"

#undef FindResource

Source2SDK g_sdk;
EXPOSE_PLUGIN(PLUGIN_API, Source2SDK, &g_sdk)

CGameEntitySystem* GameEntitySystem() {
	static int offset = g_pGameConfig->GetOffset("GameEntitySystem");
	return *reinterpret_cast<CGameEntitySystem**>(reinterpret_cast<uintptr_t>(g_pGameResourceServiceServer) + offset);
}

DynLibUtils::CVTFHookAuto<&CServerSideClientBase::ProcessRespondCvarValue> s_ProcessRespondCvarValue;
DynLibUtils::CVTFHookAuto<&IGameSystem::BuildGameSessionManifest> s_BuildGameSessionManifest;
void* _ZNSs4_Rep20_S_empty_rep_storageE_ptr;
void* _ZNSbIwSt11char_traitsIwESaIwEE4_Rep20_S_empty_rep_storageE_ptr;

//constexpr auto CS_SCRIPT_PATH = "scripts/vscripts/test.vjs";
constexpr auto CS_SCRIPT_PATH = "maps/editor/zoo/scripts/hello.vjs";

void Source2SDK::OnPluginStart() {
	S2_LOG(LS_DEBUG, "[OnPluginStart] - Source2SDK!\n");

	globals::Initialize({
		{ "base", g_sdk.GetLocation() },
		{ "configs", plg::GetConfigsDir() },
		{ "data", plg::GetDataDir() },
	});

	using enum poly::CallbackType;

	g_PH.AddHookMemFunc(&IGameEventManager2::FireEvent, g_pGameEventManager, Hook_FireEvent, Pre, Post);
	using PostEventAbstract = void(IGameEventSystem::*)( CSplitScreenSlot slot, bool bLocalOnly, int nClientCount, const uint64 *clients, INetworkMessageInternal *pEvent, const CNetMessage *pData, unsigned long nSize, NetChannelBufType_t bufType);
	g_PH.AddHookMemFunc<PostEventAbstract>(&IGameEventSystem::PostEventAbstract, g_pGameEventSystem, Hook_PostEvent, Pre, Post);

	g_PH.AddHookMemFunc(&IServerGameClients::ClientCommand, g_pSource2GameClients, Hook_ClientCommand, Pre);
	g_PH.AddHookMemFunc(&IServerGameClients::ClientActive, g_pSource2GameClients, Hook_ClientActive, Post);
	g_PH.AddHookMemFunc(&IServerGameClients::ClientDisconnect, g_pSource2GameClients, Hook_ClientDisconnect, Pre, Post);
	g_PH.AddHookMemFunc(&IServerGameClients::ClientPutInServer, g_pSource2GameClients, Hook_ClientPutInServer, Post);
	g_PH.AddHookMemFunc(&IServerGameClients::ClientSettingsChanged, g_pSource2GameClients, Hook_ClientSettingsChanged, Post);
	g_PH.AddHookMemFunc(&IServerGameClients::OnClientConnected, g_pSource2GameClients, Hook_OnClientConnected, Post);
	g_PH.AddHookMemFunc(&IServerGameClients::ClientFullyConnect, g_pSource2GameClients, Hook_ClientFullyConnect, Post);
	g_PH.AddHookMemFunc(&IServerGameClients::ClientConnect, g_pSource2GameClients, Hook_ClientConnect, Pre, Post);
	g_PH.AddHookMemFunc(&INetworkServerService::StartupServer, g_pNetworkServerService, Hook_StartupServer, Post);
	g_PH.AddHookMemFunc(&INetworkServerService::DisconnectGameNow, g_pNetworkServerService, Hook_DisconnectGameNow, Post);
	g_PH.AddHookMemFunc(&ISource2Server::GameServerSteamAPIActivated, g_pSource2Server, Hook_GameServerSteamAPIActivated, Post);
	g_PH.AddHookMemFunc(&ISource2Server::GameServerSteamAPIDeactivated, g_pSource2Server, Hook_GameServerSteamAPIDeactivated, Post);
	g_PH.AddHookMemFunc(&ISource2Server::UpdateWhenNotInGame, g_pSource2Server, Hook_UpdateWhenNotInGame, Post);
	g_PH.AddHookMemFunc(&ISource2Server::PreWorldUpdate, g_pSource2Server, Hook_PreWorldUpdate, Post);
	g_PH.AddHookMemFunc(&IServerGameDLL::GameFrame, g_pSource2Server, Hook_GameFrame, Post);
	g_PH.AddHookMemFunc(&ICvar::DispatchConCommand, g_pCVar, Hook_DispatchConCommand, Pre, Post);

	using FireOutputInternalFn = void(*)(CEntityIOOutput*, CEntityInstance*, CEntityInstance*, const CVariant*, float);
	g_PH.AddHookDetourFunc<FireOutputInternalFn>("CEntityIOOutput_FireOutputInternal", Hook_FireOutputInternal, Pre, Post);

	{
		auto engine2 = g_GameConfigManager.GetModule("engine2");
		auto table = engine2->GetVirtualTableByName("CServerSideClient");
		DynLibUtils::CVirtualTable vtable(table);
		s_ProcessRespondCvarValue.Hook(vtable, [](CServerSideClientBase* pThis, const CCLCMsg_RespondCvarValue_t& msg) -> bool {
			g_PlayerManager.OnRespondCvarValue(pThis, msg);
			return s_ProcessRespondCvarValue.Call(pThis, msg);
		});
	}

	{
		auto server = g_GameConfigManager.GetModule("server");
		auto table = server->GetVirtualTableByName("CGameRulesGameSystem");
		DynLibUtils::CVirtualTable vtable(table);
		s_BuildGameSessionManifest.Hook(vtable, [](IGameSystem* pThis, const EventBuildGameSessionManifest_t& msg) {
			msg.m_pResourceManifest->AddResource(CS_SCRIPT_PATH);
			s_BuildGameSessionManifest.Call(pThis, msg);
		});
	}

	using CCSServerPointScriptEntityEnterScope = void*(*)(void*, void*);
	g_PH.AddHookDetourFunc<CCSServerPointScriptEntityEnterScope>("CCSServerPointScriptEntityEnterScope", Hook_CCSServerPointScriptEntity, Pre);

	using v8IsolateFn = void(*)(v8::Isolate*);

	auto v8IsolateEnterPtr = g_pGameConfig->GetAddress("v8::Isolate::Enter").CCast<void*>();
	if (!v8IsolateEnterPtr) {
		S2_LOG(LS_ERROR, "v8::Isolate::Enter not found!\n");
		return;
	}

	auto v8IsolateExitPtr = g_pGameConfig->GetAddress("v8::Isolate::Exit").CCast<void*>();
	if (!v8IsolateExitPtr) {
		S2_LOG(LS_ERROR, "v8::Isolate::Exit not found!\n");
		return;
	}

	g_PH.AddHookDetourFunc<v8IsolateFn>((uintptr_t)v8IsolateEnterPtr, Hook_IsolateEnter, Pre);
	g_PH.AddHookDetourFunc<v8IsolateFn>((uintptr_t)v8IsolateExitPtr, Hook_IsolateExit, Post);

	{
		using SetModuleResolverFn = void(*)(v8::Module::ResolveModuleCallback);
		DynLibUtils::CModule v8("plugify-module-v8");
		auto resolve = v8.GetFunctionByName("SetModuleResolver").RCast<SetModuleResolverFn>();
		if (!resolve) {
			S2_LOG(LS_ERROR, "SetModuleResolver not found!\n");
			return;
		}
		resolve(addresses::CSScriptResolveModule);
	}

#if S2SDK_PLATFORM_WINDOWS
	using PreloadLibrary = void(*)(void*);
	g_PH.AddHookDetourFunc<PreloadLibrary>("PreloadLibrary", Hook_PreloadLibrary, Pre);
#else
	using GetRepStorageFn = void*(*)();
	auto tier0 = g_GameConfigManager.GetModule("tier0");
	_ZNSs4_Rep20_S_empty_rep_storageE_ptr = tier0->GetFunctionByName("_ZNSs4_Rep12_S_empty_repEv").RCast<GetRepStorageFn>()();
	_ZNSbIwSt11char_traitsIwESaIwEE4_Rep20_S_empty_rep_storageE_ptr = tier0->GetFunctionByName("_ZNSbIwSt11char_traitsIwESaIwEE4_Rep12_S_empty_repEv").RCast<GetRepStorageFn>()();
#endif

	OnServerStartup();// for late load*/
}

void Source2SDK::OnPluginEnd() {
	globals::Terminate();
	g_PH.UnhookAll();
	UnregisterEventListeners();

	S2_LOG(LS_DEBUG, "[OnPluginEnd] = Source2SDK!\n");
}

void Source2SDK::OnServerStartup() {
	g_pNetworkGameServer = g_pNetworkServerService->GetIGameServer();

	if (g_pNetworkGameServer != nullptr) {
		gpGlobals = g_pNetworkGameServer->GetGlobals();
		g_PH.AddHookMemFunc(&CNetworkGameServerBase::Release, g_pNetworkGameServer, Hook_Release, poly::CallbackType::Pre);
		g_PH.AddHookMemFunc(&CNetworkGameServerBase::ActivateServer, g_pNetworkGameServer, Hook_ActivateServer, poly::CallbackType::Post);
		g_PH.AddHookMemFunc(&CNetworkGameServerBase::SpawnServer, g_pNetworkGameServer, Hook_SpawnServer, poly::CallbackType::Post);
	}

	if (g_pGameEntitySystem != nullptr) {
		g_PH.RemoveHookMemFunc(&CEntitySystem::OnAddEntity, g_pGameEntitySystem);
		g_PH.RemoveHookMemFunc(&CEntitySystem::OnRemoveEntity, g_pGameEntitySystem);
		g_PH.RemoveHookMemFunc(&CEntitySystem::OnEntityParentChanged, g_pGameEntitySystem);
	}

	g_pGameEntitySystem = GameEntitySystem();

	if (g_pGameEntitySystem != nullptr) {
		//g_pGameEntitySystem->AddListenerEntity(&g_pEntityListener);
		g_PH.AddHookMemFunc(&CEntitySystem::OnAddEntity, g_pGameEntitySystem, Hook_OnAddEntity, poly::CallbackType::Post);
		g_PH.AddHookMemFunc(&CEntitySystem::OnRemoveEntity, g_pGameEntitySystem, Hook_OnRemoveEntity, poly::CallbackType::Post);
		g_PH.AddHookMemFunc(&CEntitySystem::OnEntityParentChanged, g_pGameEntitySystem, Hook_OnEntityParentChanged, poly::CallbackType::Post);
	}

	//g_MultiAddonManager.OnStartupServer();

	RegisterEventListeners();
}

poly::ReturnAction Source2SDK::Hook_StartupServer(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//auto config = poly::GetArgument<const GameSessionConfiguration_t *>(params, 1);
	//auto pWorldSession = poly::GetArgument<ISource2WorldSession*>(params, 2);
	//auto pMapName = poly::GetArgument<const char*>(params, 3);

	//S2_LOGF(LS_DEBUG, "[StartupServer] = {}\n", pMapName);

	OnServerStartup();

	if (gpGlobals == nullptr) {
		S2_LOG(LS_ERROR, "Failed to lookup gpGlobals\n");
		return poly::ReturnAction::Ignored;
	}

	GetOnServerStartupListenerManager().Notify();

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_DisconnectGameNow(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto reason = (ENetworkDisconnectionReason) poly::GetArgument<int>(params, 1);

	if (reason == NETWORK_DISCONNECT_LOOPDEACTIVATE  || reason == NETWORK_DISCONNECT_EXITING) {
		static ENetworkDisconnectionReason lastReason = NETWORK_DISCONNECT_INVALID;

		if (reason < lastReason)
			return poly::ReturnAction::Ignored;

		lastReason = reason;

		g_TimerSystem.OnMapEnd();

		GetOnMapEndListenerManager().Notify();
	}

	if (reason == NETWORK_DISCONNECT_REQUEST_HOSTSTATE_IDLE) {
		ExecuteOnce( {
			GetOnServerStartedListenerManager().Notify();
		});
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_Release(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto server = poly::GetArgument<CNetworkGameServerBase*>(params, 1);

	if (server->m_nRefCount > 1)
		return poly::ReturnAction::Ignored;

	//S2_LOGF(LS_DEBUG, "[Release] {}\n", server->m_nRefCount);

	// g_PH.RemoveHookMemFunc(&CNetworkGameServerBase::Release, server);
	// g_PH.RemoveHookMemFunc(&CNetworkGameServerBase::ActivateServer, server);
	// g_PH.RemoveHookMemFunc(&CNetworkGameServerBase::SpawnServer, server);
	// g_PH.RemoveHookMemFunc(&CNetworkGameServerBase::FinishChangeLevel, server);

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_ActivateServer(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//S2_LOGF(LS_DEBUG, "[ActivateServer]\n");

	GetOnServerActivateListenerManager().Notify();

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_SpawnServer(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//S2_LOGF(LS_DEBUG, "[SpawnServer]\n");

	GetOnServerSpawnListenerManager().Notify();

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_FireEvent(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//S2_LOGF(LS_DEBUG, "FireEvent = {}\n", event->GetName() );
	auto event = poly::GetArgument<IGameEvent*>(params, 1);
	auto dontBroadcast = poly::GetArgument<bool>(params, 2);

	auto result = type == poly::CallbackType::Post ? g_EventManager.OnFireEvent_Post(event, dontBroadcast) : g_EventManager.OnFireEvent(event, dontBroadcast);
	if (result >= ResultType::Handled) {
		return poly::ReturnAction::Supercede;
	}

	if (result == ResultType::Changed) {
		poly::SetArgument<bool>(params, 2, !dontBroadcast);
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_PostEvent(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//S2_LOGF(LS_DEBUG, "[PostEvent] = {}, {}, {}, {}\n", slot, bLocalOnly, nClientCount, clients );
	//auto clientCount = poly::GetArgument<int>(params, 3);
	auto clients = poly::GetArgument<uint64_t*>(params, 4);
	auto message = poly::GetArgument<INetworkMessageInternal*>(params, 5);
	auto pData = poly::GetArgument<CNetMessage*>(params, 6);

	auto result = g_UserMessageManager.ExecuteMessageCallbacks(message, pData, clients, static_cast<HookMode>(type));
	if (result >= ResultType::Handled) {
		return poly::ReturnAction::Supercede;
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_GameFrame(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// bool simulating, bool bFirstTick, bool bLastTick
	auto simulating = poly::GetArgument<bool>(params, 1);
	auto bFirstTick = poly::GetArgument<bool>(params, 2);
	auto bLastTick = poly::GetArgument<bool>(params, 3);

	g_ServerManager.OnGameFrame();
	g_TimerSystem.OnGameFrame(simulating);

	GetOnGameFrameListenerManager().Notify(simulating, bFirstTick, bLastTick);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_ClientActive(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, bool bLoadGame, const char* name, uint64 steamID64
	auto slot = (CPlayerSlot) poly::GetArgument<int>(params, 1);
	auto bLoadGame = poly::GetArgument<bool>(params, 2);
	//auto name = poly::GetArgument<const char*>(params, 3);
	//auto steamID64 = (uint64) poly::GetArgument<uint64_t>(params, 4);

	//S2_LOGF(LS_DEBUG, "[OnClientActive] = {}, \"{}\", {}\n", slot, name, steamID64);

	g_PlayerManager.OnClientActive(slot, bLoadGame);

	GetOnClientActiveListenerManager().Notify(slot, bLoadGame);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_ClientDisconnect(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, int reason, const char* name, uint64 steamID64, const char* networkID
	auto slot = (CPlayerSlot) poly::GetArgument<int>(params, 1);
	auto reason = (ENetworkDisconnectionReason) poly::GetArgument<int>(params, 2);
	//auto name = poly::GetArgument<const char*>(params, 3);
	//auto steamID64 = poly::GetArgument<uint64_t>(params, 4);
	//auto networkID = poly::GetArgument<const char*>(params, 5);

	//S2_LOGF(LS_DEBUG, "[ClientDisconnect] = {}, {}, \"{}\", {}, \"{}\"\n", slot, reason, name, steamID64, networkID);

	if (type == poly::CallbackType::Pre) {
		g_PlayerManager.OnClientDisconnect(slot, reason);
	} else {
		g_PlayerManager.OnClientDisconnect_Post(slot, reason);
		g_PanoramaVoteHandler.RemovePlayerFromVote(slot);
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_ClientPutInServer(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, char const *name, int type, uint64 steamID64
	auto slot = (CPlayerSlot) poly::GetArgument<int>(params, 1);
	auto name = poly::GetArgument<const char*>(params, 2);
	//auto conType = poly::GetArgument<int>(params, 3);
	//auto steamID64 = poly::GetArgument<uint64_t>(params, 4);

	//S2_LOGF(LS_DEBUG, "[ClientPutInServer] = {}, \"{}\", {}, {}, {}\n", slot, name, conType, steamID64);

	g_PlayerManager.OnClientPutInServer(slot, name);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_ClientSettingsChanged(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot
	auto slot = (CPlayerSlot) poly::GetArgument<int>(params, 1);

	//S2_LOGF(LS_DEBUG, "[ClientSettingsChanged] = {}\n", slot);

	GetOnClientSettingsChangedListenerManager().Notify(slot);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_OnClientConnected(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, const cha*r name, uint64 steamID64, const char* networkID, const char* pszAddress, bool bFakePlayer
	auto slot = (CPlayerSlot) poly::GetArgument<int>(params, 1);
	/*auto name = poly::GetArgument<const char*>(params, 2);
	auto steamID64 = poly::GetArgument<uint64_t>(params, 3);
	auto networkID = poly::GetArgument<const char*>(params, 4);
	auto pszAddress = poly::GetArgument<const char*>(params, 5);*/
	auto bFakePlayer = poly::GetArgument<bool>(params, 6);

	// S2_LOGF(LS_DEBUG, "[OnClientConnected] = {}, \"{}\", {}, \"{}\", \"{}\", {}\n", slot, name,
	// steamID64, networkID, pszAddress, bFakePlayer);

	g_PlayerManager.OnClientConnected(slot, bFakePlayer);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_ClientFullyConnect(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot
	auto slot = (CPlayerSlot) poly::GetArgument<int>(params, 1);

	//S2_LOGF(LS_DEBUG, "[ClientFullyConnect] = {}\n", slot);

	GetOnClientFullyConnectListenerManager().Notify(slot);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_ClientConnect(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, const char* name, uint64 steamID64, const char* networkID, bool unk1, CBufferString *pRejectReason
	auto slot = (CPlayerSlot) poly::GetArgument<int>(params, 1);
	auto name = poly::GetArgument<const char*>(params, 2);
	auto steamID64 = poly::GetArgument<uint64_t>(params, 3);
	auto networkID = poly::GetArgument<const char*>(params, 4);
	//bool unk1 = poly::GetArgument<bool>(params, 5);
	//auto pRejectReason = poly::GetArgument<CBufferString*>(params, 6);

	//S2_LOGF(LS_DEBUG, "[ClientConnect] = {}, \"{}\", {}, \"{}\", {}, \"{}\" \n", slot, name, steamID64, networkID, unk1, pRejectReason->Get());

	if (type == poly::CallbackType::Pre) {
		g_PlayerManager.OnClientConnect(slot, name, steamID64, networkID);
	} else {
		bool origRet = poly::GetReturn<bool>(ret);
		bool newRet = g_PlayerManager.OnClientConnect_Post(slot, origRet);
		if (newRet != origRet) {
			poly::SetReturn<bool>(ret, newRet);
			return poly::ReturnAction::Handled;
		}
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_ClientCommand(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CPlayerSlot slot, const CCommand& _cmd
	auto slot = (CPlayerSlot) poly::GetArgument<int>(params, 1);
	auto args = poly::GetArgument<const CCommand*>(params, 2);
	if (args == nullptr) {
		return poly::ReturnAction::Ignored;
	}

	//S2_LOGF(LS_DEBUG, "[ClientCommand] = {}, \"{}\"\n", slot, args->GetCommandString());

	const char* cmd = args->Arg(0);

	auto result = g_CommandManager.ExecuteCommandCallbacks(cmd, CCommandContext(CommandTarget_t::CT_NO_TARGET, slot), *args, HookMode::Pre, CommandCallingContext::Console);
	if (result >= ResultType::Handled) {
		return poly::ReturnAction::Supercede;
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_GameServerSteamAPIActivated(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	S2_LOG(LS_DEBUG, "[GameServerSteamAPIActivated]\n");

	g_SteamAPI.Init();
	//g_http = g_steamAPI.SteamHTTP();

	g_PlayerManager.OnSteamAPIActivated();

	//GetOnGameServerSteamAPIActivatedListenerManager().Notify();
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_GameServerSteamAPIDeactivated(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	S2_LOG(LS_DEBUG, "[GameServerSteamAPIDeactivated]\n");

	//g_http = nullptr;

	//GetOnGameServerSteamAPIDeactivatedListenerManager().Notify();
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_UpdateWhenNotInGame(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// float flFrameTime
	auto flFrameTime = poly::GetArgument<float>(params, 1);
	//S2_LOGF(LS_DEBUG, "UpdateWhenNotInGame = {}\n", flFrameTime);
	GetOnUpdateWhenNotInGameListenerManager().Notify(flFrameTime);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_PreWorldUpdate(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// bool simulating
	auto simulating = poly::GetArgument<bool>(params, 1);
	//S2_LOGF(LS_DEBUG, "PreWorldUpdate = {}\n", simulating);

	g_ServerManager.OnPreWorldUpdate();

	GetOnPreWorldUpdateListenerManager().Notify(simulating);
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_FireOutputInternal(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// CEntityIOOutput* const pThis, CEntityInstance* pActivator, CEntityInstance* pCaller, const CVariant* const value, float flDelay
	auto pThis = poly::GetArgument<CEntityIOOutput* const>(params, 0);
	auto pActivator = poly::GetArgument<CEntityInstance*>(params, 1);
	auto pCaller = poly::GetArgument<CEntityInstance*>(params, 2);
	//auto value = poly::GetArgument<const CVariant* const>(params, 3);
	auto flDelay = poly::GetArgument<float>(params, 4);

	ResultType result = type == poly::CallbackType::Post ? g_OutputManager.FireOutputInternal_Post(pThis, pActivator, pCaller, flDelay) : g_OutputManager.FireOutputInternal(pThis, pActivator, pCaller, flDelay);

	if (result >= ResultType::Handled) {
		return poly::ReturnAction::Supercede;
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_DispatchConCommand(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	// auto cmd = poly::GetArgument<ConCommandRef* const>(params, 1);
	auto ctx = poly::GetArgument<const CCommandContext*>(params, 2);
	auto args = poly::GetArgument<const CCommand*>(params, 3);
	auto result = g_CommandManager.DispatchConCommand(ctx, args, static_cast<HookMode>(type));
	if (result >= ResultType::Handled) {
		return poly::ReturnAction::Supercede;
	}

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_OnAddEntity(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto pEntity = poly::GetArgument<CBaseEntity*>(params, 1);
	auto handle = (CEntityHandle) poly::GetArgument<int>(params, 2);

	std::string_view name(pEntity->GetClassname());
	if (name == "cs_gamerules") {
		g_pGameRulesProxy = static_cast<CBaseGameRulesProxy *>(pEntity);
		g_pGameRules = g_pGameRulesProxy->m_pGameRules;

		v8::Isolate* isolate = v8::Isolate::TryGetCurrent();
		v8::Locker locker(isolate);
		v8::Isolate::Scope isolateScope(isolate);
		v8::HandleScope handleScope(isolate);

		g_pPointScript = static_cast<CBaseEntity*>(addresses::CreateEntityByName("point_script", -1));
		g_pPointScript->DispatchSpawn({
			{"target_name", "script_main" },
			{"cs_script", CS_SCRIPT_PATH}
		});
	} else if (name == "cs_team_manager") {
		g_pTeamManagers[pEntity->m_iTeamNum] = static_cast<CTeam *>(pEntity);
	}
	GetOnEntityCreatedListenerManager().Notify(handle.ToInt());
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_OnRemoveEntity(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto pEntity = poly::GetArgument<CBaseEntity*>(params, 1);
	auto handle = (CEntityHandle) poly::GetArgument<int>(params, 2);

	std::string_view name(pEntity->GetClassname());
	if (name == "cs_gamerules") {
		g_pGameRulesProxy = nullptr;
		g_pGameRules = nullptr;
	} else if (name == "cs_team_manager") {
		g_pTeamManagers.erase(pEntity->m_iTeamNum);
	}
	GetOnEntityDeletedListenerManager().Notify(handle.ToInt());
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_OnEntityParentChanged(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto pEntity = poly::GetArgument<CBaseEntity*>(params, 1);
	auto pNewParent = poly::GetArgument<CBaseEntity*>(params, 2);

	GetOnEntityParentChangedListenerManager().Notify(pEntity->GetRefEHandle().ToInt(), pNewParent ? pNewParent->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX);
	return poly::ReturnAction::Ignored;
}

/*poly::ReturnAction Source2SDK::Hook_CCSScriptConstuctor(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto pScript = poly::GetArgument<CCSScript*>(params, 0);

	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	auto* cxt = reinterpret_cast<v8::Global<v8::Context>*>(isolate->GetData(v8::Isolate::GetNumberOfDataSlots() - 2));

	pScript->v8_api_global.Reset(isolate, cxt->Get(isolate));

	return poly::ReturnAction::Ignored;
}*/

poly::ReturnAction Source2SDK::Hook_CCSServerPointScriptEntity(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	//auto pEntity = poly::GetArgument<CCSPointScriptEntity*>(params, 0);
	auto pScript = poly::GetArgument<CCSScript_EntityScript*>(params, 1);

	g_pScripts->AddToTail(pScript);

	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_IsolateEnter(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto isolate = poly::GetArgument<v8::Isolate*>(params, 0);
	isolate->SetData(v8::Isolate::GetNumberOfDataSlots() - 2, new v8::Locker(isolate));
	return poly::ReturnAction::Ignored;
}

poly::ReturnAction Source2SDK::Hook_IsolateExit(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
	auto isolate = poly::GetArgument<v8::Isolate*>(params, 0);
	delete reinterpret_cast<v8::Locker*>(isolate->GetData(v8::Isolate::GetNumberOfDataSlots() - 2));
	isolate->SetData(v8::Isolate::GetNumberOfDataSlots() - 2, nullptr);
	return poly::ReturnAction::Ignored;
}

#if S2SDK_PLATFORM_WINDOWS

#if PLUGIFY_ARCH_BITS == 64
constexpr WORD PE_FILE_MACHINE = IMAGE_FILE_MACHINE_AMD64;
constexpr WORD PE_NT_OPTIONAL_HDR_MAGIC = IMAGE_NT_OPTIONAL_HDR64_MAGIC;
#else
constexpr WORD PE_FILE_MACHINE = IMAGE_FILE_MACHINE_I386;
constexpr WORD PE_NT_OPTIONAL_HDR_MAGIC = IMAGE_NT_OPTIONAL_HDR32_MAGIC;
#endif // PLUGIFY_ARCH_BITS

poly::ReturnAction Source2SDK::Hook_PreloadLibrary(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type) {
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
extern "C" {
	void* __wrap__ZNSs4_Rep20_S_empty_rep_storageE() {
		return _ZNSs4_Rep20_S_empty_rep_storageE_ptr;
	}
	void* __wrap__ZNSbIwSt11char_traitsIwESaIwEE4_Rep20_S_empty_rep_storageE() {
		return _ZNSbIwSt11char_traitsIwESaIwEE4_Rep20_S_empty_rep_storageE_ptr;
	}
}
#endif
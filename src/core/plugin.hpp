#pragma once

#include <plg/plugin.hpp>
#include <polyhook/polyhook.hpp>
#include <plugin_export.h>

#include <igamesystem.h>
#include <igamesystemfactory.h>
#include <networksystem/inetworkmessages.h>

class Source2SDK final : public plg::IPluginEntry {
public:
	void OnPluginStart() final;
	void OnPluginEnd() final;
	static void OnServerStartup();

	static poly::ReturnAction Hook_StartupServer(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_DisconnectGameNow(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_Release(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_ActivateServer(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_SpawnServer(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_FireEvent(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_PostEvent(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_GameFrame(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_ClientActive(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_ClientDisconnect(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_ClientPutInServer(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_ClientSettingsChanged(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_OnClientConnected(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_ClientFullyConnect(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_ClientConnect(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_ClientCommand(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_GameServerSteamAPIActivated(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_GameServerSteamAPIDeactivated(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_UpdateWhenNotInGame(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_PreWorldUpdate(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_FireOutputInternal(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_DispatchConCommand(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_OnAddEntity(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_OnRemoveEntity(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_OnEntityParentChanged(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_CCSServerPointScriptEntity(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_IsolateEnter(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
	static poly::ReturnAction Hook_IsolateExit(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
#if S2SDK_PLATFORM_WINDOWS
	static poly::ReturnAction Hook_PreloadLibrary(poly::IHook& hook, poly::Params& params, int count, poly::Return& ret, poly::CallbackType type);
#endif
};
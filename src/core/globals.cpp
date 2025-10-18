
#include <core/sdk/utils.h>
#include <core/sdk/virtual.h>
#include <engine/igameeventsystem.h>
#include <IEngineSound.h>
#include <igameevents.h>
#include <igamesystemfactory.h>

#include "globals.hpp"

#include "con_var_manager.hpp"
#include "core_config.hpp"
#include "game_config.hpp"
#include "hook_holder.hpp"

#define RESOLVE_SIG(gameConfig, name, variable) \
	variable = (gameConfig)->ResolveSignature(name).RCast<decltype(variable)>(); \
	if (!(variable)) { \
		plg::print(LS_ERROR, "Failed to resolve signature for {}\n", #name); \
		return; \
	} \

CoreConfig* g_pCoreConfig = nullptr;
GameConfig* g_pGameConfig = nullptr;

namespace globals {
	void Initialize(plg::flat_hash_map<plg::string, plg::string> paths) {
		g_pCoreConfig = new CoreConfig(plg::vector{
				paths["base"] + "/settings.jsonc",
				paths["configs"] + "/settings.jsonc",
				paths["data"] + "/settings.jsonc"
		});
		if (!g_pCoreConfig->Initialize()) {
			plg::print(LS_ERROR, "Failed to load settings configuration!\n");
			return;
		}
		uint32_t id = g_GameConfigManager.LoadGameConfigFile(plg::vector{
				paths["base"] + "/gamedata.jsonc",
				paths["configs"] + "/gamedata.jsonc",
				paths["data"] + "/gamedata.jsonc"
		});
		g_pGameConfig = g_GameConfigManager.GetGameConfig(id);
		if (!g_pGameConfig) {
			plg::print(LS_ERROR, "Failed to load gamedata configuration!\n");
			return;
		}

		CAppSystemDict** p_ppCurrentAppSystem = g_pGameConfig->GetAddress("&s_pCurrentAppSystem").RCast<CAppSystemDict**>();
		if (!p_ppCurrentAppSystem) {
			plg::print(LS_ERROR, "s_pCurrentAppSystem not found!\n");
			return;
		}
		g_pCurrentAppSystem = *p_ppCurrentAppSystem;

		IGameEventManager2** p_ppGameEventManager = g_pGameConfig->GetAddress("&s_GameEventManager").RCast<IGameEventManager2**>();
		if (!p_ppGameEventManager) {
			plg::print(LS_ERROR, "s_GameEventManager not found!\n");
			return;
		}
		g_pGameEventManager = *p_ppGameEventManager;

#if defined (CS2)
		g_pScripts = g_pGameConfig->GetAddress("&s_pScripts").RCast<CUtlVector<void*>*>();
		if (!g_pScripts) {
			plg::print(LS_ERROR, "s_pScripts not found!\n");
			return;
		}
#endif

		g_pCVar = static_cast<ICvar*>(QueryInterface("tier0", CVAR_INTERFACE_VERSION));
		g_pSchemaSystem = static_cast<ISchemaSystem*>(QueryInterface("schemasystem", SCHEMASYSTEM_INTERFACE_VERSION));
		g_pSource2Server = static_cast<ISource2Server*>(QueryInterface("server", SOURCE2SERVER_INTERFACE_VERSION));
		g_pSource2GameEntities = static_cast<ISource2GameEntities*>(QueryInterface("server", SOURCE2GAMEENTITIES_INTERFACE_VERSION));
		g_pSource2GameClients = static_cast<ISource2GameClients*>(QueryInterface("server", SOURCE2GAMECLIENTS_INTERFACE_VERSION));
		g_pGameResourceServiceServer = static_cast<IGameResourceService*>(QueryInterface("engine2", GAMERESOURCESERVICESERVER_INTERFACE_VERSION));
		g_pEngineServiceMgr = static_cast<IEngineServiceMgr*>(QueryInterface("engine2", ENGINESERVICEMGR_INTERFACE_VERSION));

		g_pEngineServer = static_cast<IVEngineServer2*>(QueryInterface("engine2", SOURCE2ENGINETOSERVER_INTERFACE_VERSION));
		g_pFullFileSystem = static_cast<IFileSystem*>(QueryInterface("filesystem", FILESYSTEM_INTERFACE_VERSION));
		g_pGameEventSystem = static_cast<IGameEventSystem*>(QueryInterface("engine2", GAMEEVENTSYSTEM_INTERFACE_VERSION));
		g_pNetworkServerService = static_cast<INetworkServerService*>(QueryInterface("engine", NETWORKSERVERSERVICE_INTERFACE_VERSION));
		g_pNetworkMessages = static_cast<INetworkMessages*>(QueryInterface("networksystem", NETWORKMESSAGES_INTERFACE_VERSION));

		ConVarManager::Init();

		// load more if needed
		RESOLVE_SIG(g_pGameConfig, "LegacyGameEventListener", addresses::GetLegacyGameEventListener);
		RESOLVE_SIG(g_pGameConfig, "CBasePlayerController::SetPawn", addresses::CBasePlayerController_SetPawn);
		RESOLVE_SIG(g_pGameConfig, "CBaseModelEntity::SetModel", addresses::CBaseModelEntity_SetModel);
		RESOLVE_SIG(g_pGameConfig, "CGameEntitySystem::FindEntityByClassName", addresses::CGameEntitySystem_FindEntityByClassName);
		RESOLVE_SIG(g_pGameConfig, "CGameEntitySystem::FindEntityByName", addresses::CGameEntitySystem_FindEntityByName);
		RESOLVE_SIG(g_pGameConfig, "CreateEntityByName", addresses::CreateEntityByName);
		RESOLVE_SIG(g_pGameConfig, "CBaseEntity::DispatchSpawn", addresses::DispatchSpawn);
		RESOLVE_SIG(g_pGameConfig, "UTIL_Remove", addresses::UTIL_Remove);
		RESOLVE_SIG(g_pGameConfig, "CEntityInstance::AcceptInput", addresses::CEntityInstance_AcceptInput);
		RESOLVE_SIG(g_pGameConfig, "CEntityIdentity::SetEntityName", addresses::CEntityIdentity_SetEntityName);
		//RESOLVE_SIG(g_pGameConfig, "CBaseEntity_EmitSoundParams", addresses::CBaseEntity_EmitSoundParams);
		RESOLVE_SIG(g_pGameConfig, "CBaseEntity::SetParent", addresses::CBaseEntity_SetParent);
		RESOLVE_SIG(g_pGameConfig, "CBaseEntity::EmitSoundFilter", addresses::CBaseEntity_EmitSoundFilter);
		RESOLVE_SIG(g_pGameConfig, "CBaseEntity::SetMoveType", addresses::CBaseEntity_SetMoveType);
		//RESOLVE_SIG(g_pGameConfig, "CCSServerPointScriptEntityEnterScope", addresses::CCSServerPointScriptEntityEnterScope);
		RESOLVE_SIG(g_pGameConfig, "CSScript::ResolveModule", addresses::CSScript_ResolveModule);

#if defined (CS2)
		RESOLVE_SIG(g_pGameConfig, "CCSPlayer_WeaponServices::RemoveItem", addresses::CCSPlayer_WeaponServices_RemoveItem);
		RESOLVE_SIG(g_pGameConfig, "CCSPlayerController::SwitchTeam", addresses::CCSPlayerController_SwitchTeam);
		RESOLVE_SIG(g_pGameConfig, "CGameRules::TerminateRound", addresses::CGameRules_TerminateRound);
		RESOLVE_SIG(g_pGameConfig, "GetCSWeaponDataFromKey", addresses::GetCSWeaponDataFromKey);
#endif
	}

	void Terminate() {
		delete g_pCoreConfig;
		g_pCoreConfig = nullptr;
		g_pGameConfig = nullptr;
	}

	PlatModule_t FindModule(std::string_view name) {
		for (const auto& module : g_pCurrentAppSystem->m_Modules) {
			if (module.m_pModuleName == name) {
				return module.m_hModule;
			}
		}
		plg::print(LS_ERROR, "Could not find module at \"{}\"\n", name);
		return {};
	}
	
	IAppSystem* FindInterface(std::string_view name) {
		for (const auto& system : g_pCurrentAppSystem->m_Systems) {
			if (system.m_pInterfaceName && system.m_pInterfaceName == name) {
				return system.m_pSystem;
			}
		}
		plg::print(LS_ERROR, "Could not find interface at \"{}\"\n", name);
		return {};
	}

	void* QueryInterface(std::string_view module, std::string_view name) {
		for (const auto& system : g_pCurrentAppSystem->m_Systems) {
			if (system.m_pInterfaceName && system.m_pInterfaceName == name) {
				return system.m_pSystem;
			}

			if (system.m_pModuleName && system.m_pModuleName == module) {
				if (auto* interface = system.m_pSystem->QueryInterface(name.data())) {
					return interface;
				}
			}
		}

		plg::print(LS_ERROR, "Could not query interface at \"{}\"\n", name);
		return {};
	}
}// namespace globals

#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/utils.hpp>
#include <core/sdk/virtual.hpp>
#include <engine/igameeventsystem.h>
#include <IEngineSound.h>
#include <igameevents.h>
#include <igamesystemfactory.h>

#include "globals.hpp"

#include "sdk/helpers.hpp"

#include "con_var_manager.hpp"
#include "core_config.hpp"
#include "game_config.hpp"
#include "hook_manager.hpp"

CoreConfig* g_pCoreConfig = nullptr;
GameConfig* g_pGameConfig = nullptr;

namespace globals {
	void Initialize(plg::flat_map<plg::string, plg::string> paths) {
		{
			g_pCoreConfig = new CoreConfig(plg::vector{
				   paths["base"] + "/settings.jsonc",
				   paths["configs"] + "/settings.jsonc",
				   paths["data"] + "/settings.jsonc"
		   });
			if (!g_pCoreConfig->Initialize()) {
				plg::print(LS_ERROR, "Failed to load settings configuration!\n");
				return;
			}
		}
		{
			auto result = g_GameConfigManager.LoadConfig({ plg::vector{
				   paths["base"] + "/gamedata.jsonc",
				   paths["configs"] + "/gamedata.jsonc",
				   paths["data"] + "/gamedata.jsonc"
			   }
		   });
			if (!result) {
				plg::print(LS_WARNING, "Failed to load gamedata configuration: {}\n", result.error());
			}

			g_pGameConfig = g_GameConfigManager.GetConfig(*result);
			if (!g_pGameConfig) {
				plg::print(LS_ERROR, "Failed to load gamedata configuration!\n");
				return;
			}
		}

		TRY_GET_ADDRESS(g_pGameConfig, "&s_GameEventManager", g_pGameEventManager);
#if defined (CS2)
		TRY_GET_ADDRESS(g_pGameConfig, "&s_pScripts", g_pScripts);
#endif

		// load more if needed
		TRY_GET_SIGNATURE(g_pGameConfig, "LegacyGameEventListener", addresses::GetLegacyGameEventListener);
		TRY_GET_SIGNATURE(g_pGameConfig, "CBasePlayerController::SetPawn", addresses::CBasePlayerController_SetPawn);
		TRY_GET_SIGNATURE(g_pGameConfig, "CGameEntitySystem::CreateEntityByName", addresses::CreateEntityByName);
		TRY_GET_SIGNATURE(g_pGameConfig, "CGameEntitySystem::DispatchSpawn", addresses::DispatchSpawn);
		TRY_GET_SIGNATURE(g_pGameConfig, "CEntityInstance::AcceptInput", addresses::CEntityInstance_AcceptInput);
		//TRY_GET_SIGNATURE(g_pGameConfig, "CBaseEntity::EmitSoundFilter", addresses::CBaseEntity_EmitSoundFilter);
		TRY_GET_SIGNATURE(g_pGameConfig, "CBaseEntity::SetMoveType", addresses::CBaseEntity_SetMoveType);

#if defined (CS2)
		TRY_GET_SIGNATURE(g_pGameConfig, "CSScript::ResolveModule", addresses::CSScript_ResolveModule);
		TRY_GET_SIGNATURE(g_pGameConfig, "CCSPlayer_WeaponServices::RemoveItem", addresses::CCSPlayer_WeaponServices_RemoveItem);
		TRY_GET_SIGNATURE(g_pGameConfig, "CCSPlayerController::SwitchTeam", addresses::CCSPlayerController_SwitchTeam);
		TRY_GET_SIGNATURE(g_pGameConfig, "CGameRules::TerminateRound", addresses::CGameRules_TerminateRound);
		TRY_GET_SIGNATURE(g_pGameConfig, "GetCSWeaponDataFromKey", addresses::GetCSWeaponDataFromKey);
#endif

		g_pCVar = static_cast<ICvar*>(QueryInterface("tier0", CVAR_INTERFACE_VERSION));
		g_pSchemaSystem = static_cast<ISchemaSystem*>(QueryInterface("schemasystem", SCHEMASYSTEM_INTERFACE_VERSION));
		g_pSource2Server = static_cast<ISource2Server*>(QueryInterface("server", SOURCE2SERVER_INTERFACE_VERSION));
		g_pSource2GameEntities = static_cast<ISource2GameEntities*>(QueryInterface("server", SOURCE2GAMEENTITIES_INTERFACE_VERSION));
		g_pSource2GameClients = static_cast<ISource2GameClients*>(QueryInterface("server", SOURCE2GAMECLIENTS_INTERFACE_VERSION));
		g_pGameResourceServiceServer = static_cast<IGameResourceService*>(QueryInterface("engine2", GAMERESOURCESERVICESERVER_INTERFACE_VERSION));
		g_pEngineServiceMgr = static_cast<IEngineServiceMgr*>(QueryInterface("engine2", ENGINESERVICEMGR_INTERFACE_VERSION));

		g_pEngineServer = static_cast<IVEngineServer2*>(QueryInterface("engine2", SOURCE2ENGINETOSERVER_INTERFACE_VERSION));
		g_pFullFileSystem = static_cast<IFileSystem*>(QueryInterface("filesystem_stdio", FILESYSTEM_INTERFACE_VERSION));
		g_pGameEventSystem = static_cast<IGameEventSystem*>(QueryInterface("engine2", GAMEEVENTSYSTEM_INTERFACE_VERSION));
		g_pNetworkServerService = static_cast<INetworkServerService*>(QueryInterface("engine2", NETWORKSERVERSERVICE_INTERFACE_VERSION));
		g_pNetworkMessages = static_cast<INetworkMessages*>(QueryInterface("networksystem", NETWORKMESSAGES_INTERFACE_VERSION));
		g_pNetworkSystem = static_cast<INetworkSystem*>(QueryInterface("networksystem", NETWORKSYSTEM_INTERFACE_VERSION));
		g_pScriptManager = static_cast<IScriptManager*>(QueryInterface("vscript", VSCRIPT_INTERFACE_VERSION));
#if defined (CS2)
		g_pNetworkStringTableServer = static_cast<INetworkStringTableContainer*>(QueryInterface("engine2", INTERFACENAME_NETWORKSTRINGTABLESERVER));
#endif

		ConVarManager::Init();
	}

	void Terminate() {
		delete g_pCoreConfig;
		g_pCoreConfig = nullptr;
		g_pGameConfig = nullptr;
	}

	PlatModule_t FindModule(std::string_view name) {
		/*for (const auto& module : pAppSystemDict->m_pDict->m_Modules) {
			if (module.m_pModuleName == name) {
				return module.m_hModule;
			}
		}*/
		plg::print(LS_ERROR, "Could not find module at \"{}\"\n", name);
		return {};
	}
	
	IAppSystem* FindInterface(std::string_view name) {
		/*for (const auto& system : pAppSystemDict->m_pDict->m_Systems) {
			if (system.m_pInterfaceName && system.m_pInterfaceName == name) {
				return system.m_pSystem;
			}
		}*/
		plg::print(LS_ERROR, "Could not find interface at \"{}\"\n", name);
		return {};
	}

	void* QueryInterface(std::string_view module, std::string_view name) {
		/*for (const auto& system : pAppSystemDict->m_pDict->m_Systems) {
			if (system.m_pInterfaceName && system.m_pInterfaceName == name) {
				return system.m_pSystem;
			}

			if (system.m_pModuleName && system.m_pModuleName == module) {
				if (auto* interface = system.m_pSystem->QueryInterface(name.data())) {
					return interface;
				}
			}
		}*/

		if (const Module moduleLib(module); moduleLib.IsValid()) {
			if (auto createInterface = moduleLib.GetFunctionByName("CreateInterface").RCast<CreateInterfaceFn>()) {
				if (auto* interface = createInterface(name.data(), nullptr)) {
					return interface;
				}
			}
		}

		plg::print(LS_ERROR, "Could not query interface at \"{}\"\n", name);
		return {};
	}
}// namespace globals

namespace entities {
	CEntityInstance* CreateEntityByName(const char* className) {
		auto handle = CEntities{}.CreateByClassname(className);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindEntityByName(CEntityInstance* startEntity, const char* name) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByName(params(0), name);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindByNameNearest(const char* name, const Vector& origin, float maxRadius) {
		auto handle = CEntities{}.FindByNameNearest(name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindByNameWithin(CEntityInstance* startEntity, const char* name, const Vector& origin, float maxRadius) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByNameWithin(params(0), name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindEntityByClassName(CEntityInstance* startEntity, const char* name) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByClassname(params(0), name);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindEntityByClassNameNearest(const char* name, const Vector& origin, float maxRadius) {
		auto handle = CEntities{}.FindByClassnameNearest(name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindEntityByClassNameWithin(CEntityInstance* startEntity, const char* name, const Vector& origin, float maxRadius) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByClassnameWithin(params(0), name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindByModel(CEntityInstance* startEntity, const char* name) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByModel(params(0), name);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindByModel(CEntityInstance* startEntity, const char* name, const Vector& origin, float maxRadius) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByModelWithin(params(0), name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindByTarget(CEntityInstance* startEntity, const char* name) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByTarget(params(0), name);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindInSphere(CEntityInstance* startEntity, const char* name, const Vector& origin, float maxRadius) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindInSphere(params(0), name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}
}// namespace entities

static_assert(std::is_trivially_copyable_v<Color>);
static_assert(std::is_trivially_copyable_v<Vector2D>);
static_assert(std::is_trivially_copyable_v<Vector>);
static_assert(std::is_trivially_copyable_v<QAngle>);
static_assert(std::is_trivially_copyable_v<Vector4D>);
static_assert(std::is_trivially_copyable_v<Quaternion>);
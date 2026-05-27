
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
	Result<void> Initialize(plg::flat_map<plg::string, plg::string> paths) {
		g_GameConfigManager.GetModuleProvider().PreloadModules();

		{
			g_pCoreConfig = new CoreConfig(plg::vector{
				   paths["base"] + "/settings.jsonc",
				   paths["configs"] + "/settings.jsonc",
				   paths["data"] + "/settings.jsonc"
		   });
			auto result = g_pCoreConfig->Initialize();
			if (!result) {
				return MakeError("Failed to load settings configuration: {}", result.error());
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
				return MakeError("Failed to load gamedata configuration: {}", result.error());
			}

			g_pGameConfig = g_GameConfigManager.GetConfig(*result);
			if (!g_pGameConfig) {
				return MakeError("Failed to load gamedata configuration!");
			}
		}

		UNWRAP(g_pGameEventManager, g_pGameConfig->GetAddress("&s_GameEventManager"));
#if defined (CS2)
		UNWRAP(g_pScripts, g_pGameConfig->GetAddress("&s_pScripts"));
#endif

		// load more if needed
		UNWRAP(addresses::GetLegacyGameEventListener, g_pGameConfig->GetSignature("LegacyGameEventListener"));
		UNWRAP(addresses::CBasePlayerController_SetPawn, g_pGameConfig->GetSignature("CBasePlayerController::SetPawn"));
		UNWRAP(addresses::CreateEntityByName, g_pGameConfig->GetSignature("CGameEntitySystem::CreateEntityByName"));
		UNWRAP(addresses::DispatchSpawn, g_pGameConfig->GetSignature("CGameEntitySystem::DispatchSpawn"));
		UNWRAP(addresses::CEntityInstance_AcceptInput, g_pGameConfig->GetSignature("CEntityInstance::AcceptInput"));
		//TRY(addresses::CBaseEntity_EmitSoundFilter, g_pGameConfig->GetSignature("CBaseEntity::EmitSoundFilter"));
		UNWRAP(addresses::CBaseEntity_SetMoveType, g_pGameConfig->GetSignature("CBaseEntity::SetMoveType"));

#if defined (CS2)
		UNWRAP(addresses::CSScript_ResolveModule, g_pGameConfig->GetSignature("CSScript::ResolveModule"));
		UNWRAP(addresses::CCSPlayer_WeaponServices_RemoveItem, g_pGameConfig->GetSignature("CCSPlayer_WeaponServices::RemoveItem"));
		UNWRAP(addresses::CCSPlayerController_SwitchTeam, g_pGameConfig->GetSignature("CCSPlayerController::SwitchTeam"));
		UNWRAP(addresses::CGameRules_TerminateRound, g_pGameConfig->GetSignature("CGameRules::TerminateRound"));
		UNWRAP(addresses::GetCSWeaponDataFromKey, g_pGameConfig->GetSignature("GetCSWeaponDataFromKey"));
#endif

		UNWRAP(g_pCVar, utils::QueryInterface("tier0", CVAR_INTERFACE_VERSION));
		UNWRAP(g_pSchemaSystem, utils::QueryInterface("schemasystem", SCHEMASYSTEM_INTERFACE_VERSION));
		UNWRAP(g_pSource2Server, utils::QueryInterface("server", SOURCE2SERVER_INTERFACE_VERSION));
		UNWRAP(g_pSource2GameEntities, utils::QueryInterface("server", SOURCE2GAMEENTITIES_INTERFACE_VERSION));
		UNWRAP(g_pSource2GameClients, utils::QueryInterface("server", SOURCE2GAMECLIENTS_INTERFACE_VERSION));
		UNWRAP(g_pGameResourceServiceServer, utils::QueryInterface("engine2", GAMERESOURCESERVICESERVER_INTERFACE_VERSION));
		UNWRAP(g_pEngineServiceMgr, utils::QueryInterface("engine2", ENGINESERVICEMGR_INTERFACE_VERSION));

		UNWRAP(g_pEngineServer, utils::QueryInterface("engine2", SOURCE2ENGINETOSERVER_INTERFACE_VERSION));
		UNWRAP(g_pFullFileSystem, utils::QueryInterface("filesystem_stdio", FILESYSTEM_INTERFACE_VERSION));
		UNWRAP(g_pGameEventSystem, utils::QueryInterface("engine2", GAMEEVENTSYSTEM_INTERFACE_VERSION));
		UNWRAP(g_pNetworkServerService, utils::QueryInterface("engine2", NETWORKSERVERSERVICE_INTERFACE_VERSION));
		UNWRAP(g_pNetworkMessages, utils::QueryInterface("networksystem", NETWORKMESSAGES_INTERFACE_VERSION));
		UNWRAP(g_pNetworkSystem, utils::QueryInterface("networksystem", NETWORKSYSTEM_INTERFACE_VERSION));
		UNWRAP(g_pScriptManager, utils::QueryInterface("vscript", VSCRIPT_INTERFACE_VERSION));
		UNWRAP(g_pNetworkStringTableServer, utils::QueryInterface("engine2", INTERFACENAME_NETWORKSTRINGTABLESERVER));

		ConVarManager::Init();

		return {};
	}

	void Terminate() {
		delete g_pCoreConfig;
		g_pCoreConfig = nullptr;
		g_pGameConfig = nullptr;
	}
}// namespace globals

namespace entities {
	CEntityInstance* CreateEntityByName(const char* className) {
		auto handle = CEntities{}.CreateByClassname(className);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindEntityByName(CEntityInstance* startEntity, const char* name) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByName(params[0], name);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindByNameNearest(const char* name, const Vector& origin, float maxRadius) {
		auto handle = CEntities{}.FindByNameNearest(name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindByNameWithin(CEntityInstance* startEntity, const char* name, const Vector& origin, float maxRadius) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByNameWithin(params[0], name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindEntityByClassName(CEntityInstance* startEntity, const char* name) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByClassname(params[0], name);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindEntityByClassNameNearest(const char* name, const Vector& origin, float maxRadius) {
		auto handle = CEntities{}.FindByClassnameNearest(name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindEntityByClassNameWithin(CEntityInstance* startEntity, const char* name, const Vector& origin, float maxRadius) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByClassnameWithin(params[0], name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindByModel(CEntityInstance* startEntity, const char* name) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByModel(params[0], name);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindByModel(CEntityInstance* startEntity, const char* name, const Vector& origin, float maxRadius) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByModelWithin(params[0], name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindByTarget(CEntityInstance* startEntity, const char* name) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindByTarget(params[0], name);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}

	CEntityInstance* FindInSphere(CEntityInstance* startEntity, const char* name, const Vector& origin, float maxRadius) {
		ParamScope params(startEntity);
		auto handle = CEntities{}.FindInSphere(params[0], name, origin, maxRadius);
		return reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(handle));
	}
}// namespace entities

static_assert(std::is_trivially_copyable_v<Color>);
static_assert(std::is_trivially_copyable_v<Vector2D>);
static_assert(std::is_trivially_copyable_v<Vector>);
static_assert(std::is_trivially_copyable_v<QAngle>);
static_assert(std::is_trivially_copyable_v<Vector4D>);
static_assert(std::is_trivially_copyable_v<Quaternion>);
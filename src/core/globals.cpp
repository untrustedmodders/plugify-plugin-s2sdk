
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

		UNWRAP(g_pGameEventManager, TryGetAddress<decltype(g_pGameEventManager)>(g_pGameConfig, "&s_GameEventManager"));
#if defined (CS2)
		UNWRAP(g_pScripts, TryGetAddress<decltype(g_pScripts)>(g_pGameConfig, "&s_pScripts"));
#endif

		// load more if needed
		UNWRAP(addresses::GetLegacyGameEventListener, TryGetSignature<decltype(addresses::GetLegacyGameEventListener)>(g_pGameConfig, "LegacyGameEventListener"));
		UNWRAP(addresses::CBasePlayerController_SetPawn, TryGetSignature<decltype(addresses::CBasePlayerController_SetPawn)>(g_pGameConfig, "CBasePlayerController::SetPawn"));
		UNWRAP(addresses::CreateEntityByName, TryGetSignature<decltype(addresses::CreateEntityByName)>(g_pGameConfig, "CGameEntitySystem::CreateEntityByName"));
		UNWRAP(addresses::DispatchSpawn, TryGetSignature<decltype(addresses::DispatchSpawn)>(g_pGameConfig, "CGameEntitySystem::DispatchSpawn"));
		UNWRAP(addresses::CEntityInstance_AcceptInput, TryGetSignature<decltype(addresses::CEntityInstance_AcceptInput)>(g_pGameConfig, "CEntityInstance::AcceptInput"));
		//TRY(addresses::CBaseEntity_EmitSoundFilter, TryGetSignature<decltype(addresses::CBaseEntity_EmitSoundFilter)>(g_pGameConfig, "CBaseEntity::EmitSoundFilter"));
		UNWRAP(addresses::CBaseEntity_SetMoveType, TryGetSignature<decltype(addresses::CBaseEntity_SetMoveType)>(g_pGameConfig, "CBaseEntity::SetMoveType"));

#if defined (CS2)
		UNWRAP(addresses::CSScript_ResolveModule, TryGetSignature<decltype(addresses::CSScript_ResolveModule)>(g_pGameConfig, "CSScript::ResolveModule"));
		UNWRAP(addresses::CCSPlayer_WeaponServices_RemoveItem, TryGetSignature<decltype(addresses::CCSPlayer_WeaponServices_RemoveItem)>(g_pGameConfig, "CCSPlayer_WeaponServices::RemoveItem"));
		UNWRAP(addresses::CCSPlayerController_SwitchTeam, TryGetSignature<decltype(addresses::CCSPlayerController_SwitchTeam)>(g_pGameConfig, "CCSPlayerController::SwitchTeam"));
		UNWRAP(addresses::CGameRules_TerminateRound, TryGetSignature<decltype(addresses::CGameRules_TerminateRound)>(g_pGameConfig, "CGameRules::TerminateRound"));
		UNWRAP(addresses::GetCSWeaponDataFromKey, TryGetSignature<decltype(addresses::GetCSWeaponDataFromKey)>(g_pGameConfig, "GetCSWeaponDataFromKey"));
#endif

		UNWRAP(g_pCVar, utils::QueryInterface<ICvar*>("tier0", CVAR_INTERFACE_VERSION));
		UNWRAP(g_pSchemaSystem, utils::QueryInterface<ISchemaSystem*>("schemasystem", SCHEMASYSTEM_INTERFACE_VERSION));
		UNWRAP(g_pSource2Server, utils::QueryInterface<ISource2Server*>("server", SOURCE2SERVER_INTERFACE_VERSION));
		UNWRAP(g_pSource2GameEntities, utils::QueryInterface<ISource2GameEntities*>("server", SOURCE2GAMEENTITIES_INTERFACE_VERSION));
		UNWRAP(g_pSource2GameClients, utils::QueryInterface<ISource2GameClients*>("server", SOURCE2GAMECLIENTS_INTERFACE_VERSION));
		UNWRAP(g_pGameResourceServiceServer, utils::QueryInterface<IGameResourceService*>("engine2", GAMERESOURCESERVICESERVER_INTERFACE_VERSION));
		UNWRAP(g_pEngineServiceMgr, utils::QueryInterface<IEngineServiceMgr*>("engine2", ENGINESERVICEMGR_INTERFACE_VERSION));

		UNWRAP(g_pEngineServer, utils::QueryInterface<IVEngineServer2*>("engine2", SOURCE2ENGINETOSERVER_INTERFACE_VERSION));
		UNWRAP(g_pFullFileSystem, utils::QueryInterface<IFileSystem*>("filesystem_stdio", FILESYSTEM_INTERFACE_VERSION));
		UNWRAP(g_pGameEventSystem, utils::QueryInterface<IGameEventSystem*>("engine2", GAMEEVENTSYSTEM_INTERFACE_VERSION));
		UNWRAP(g_pNetworkServerService, utils::QueryInterface<INetworkServerService*>("engine2", NETWORKSERVERSERVICE_INTERFACE_VERSION));
		UNWRAP(g_pNetworkMessages, utils::QueryInterface<INetworkMessages*>("networksystem", NETWORKMESSAGES_INTERFACE_VERSION));
		UNWRAP(g_pNetworkSystem, utils::QueryInterface<INetworkSystem*>("networksystem", NETWORKSYSTEM_INTERFACE_VERSION));
		UNWRAP(g_pScriptManager, utils::QueryInterface<IScriptManager*>("vscript", VSCRIPT_INTERFACE_VERSION));
		UNWRAP(g_pNetworkStringTableServer, utils::QueryInterface<INetworkStringTableContainer*>("engine2", INTERFACENAME_NETWORKSTRINGTABLESERVER));

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
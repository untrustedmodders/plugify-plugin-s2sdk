#pragma once

#include <playerslot.h>
#include <plg/enum.hpp>
#include <plg/path.hpp>
#include <plg/plugin.hpp>
#include <plg/inplace_vector.hpp>
#include <steam_api.h>
#include <tier0/platform.h>
#include <tier0/utlstring.h>
#include <variant.h>
#include <v8.h>

#define CS_TEAM_NONE 0
#define CS_TEAM_SPECTATOR 1
#define CS_TEAM_T 2
#define CS_TEAM_CT 3

enum class HudDest : uint32_t {
	Notify = 1,
	Console = 2,
	Chat = 3,
	Center = 4,
	Alert = 6,
};

#define MAXPLAYERS 64

class CAppSystemDict;
class IAppSystem;
class IGameEventSystem;
class IGameEventManager2;
class CNetworkGameServerBase;
class CGlobalVars;
class CGameEntitySystem;
class IGameEventListener2;
class CEntitySystem;

class CEntityInstance;
class CBasePlayerController;
class CBaseEntity;
class CHostStateMgr;
struct CHostStateRequest;
class CGameRules;
class CTeam;
#if defined(CS2)
class CCSGameRules;
class CCSGameRulesProxy;
using CBaseGameRules = CCSGameRules;
using CBaseGameRulesProxy = CCSGameRulesProxy;
#elif defined(DEADLOCK)
class CCitadelGameRules;
class CCitadelGameRulesProxy;
using CBaseGameRules = CCitadelGameRules;
using CBaseGameRulesProxy = CCitadelGameRulesProxy;
#endif

inline IGameEventSystem* g_pGameEventSystem = nullptr;
inline IGameEventManager2* g_pGameEventManager = nullptr;
inline CUtlVector<void*>* g_pScripts = nullptr;
inline CAppSystemDict* g_pCurrentAppSystem = nullptr;
inline CNetworkGameServerBase* g_pNetworkGameServer = nullptr;
inline CGlobalVars* gpGlobals = nullptr;
inline CGameEntitySystem* g_pGameEntitySystem = nullptr;
inline CBaseGameRules* g_pGameRules = nullptr;
inline CBaseGameRulesProxy* g_pGameRulesProxy = nullptr;
inline CBaseEntity* g_pPointScript = nullptr;
inline plg::flat_hash_map<int, CTeam*> g_pTeamManagers;
//inline ISteamHTTP* g_http = nullptr;
inline CSteamGameServerAPIContext g_SteamAPI = {};
inline plg::flat_hash_set<plg::string> g_Precached;

class CoreConfig;
class GameConfig;

extern CoreConfig* g_pCoreConfig;
extern GameConfig* g_pGameConfig;

namespace globals {
	void Initialize(plg::flat_hash_map<plg::string, plg::string> paths);
	void Terminate();

	PlatModule_t FindModule(std::string_view name);
	IAppSystem* FindInterface(std::string_view name);
	void* QueryInterface(std::string_view module, std::string_view name);
}// namespace globals

class CPlayer_WeaponServices;
class IEntityFindFilter;
class IRecipientFilter;
struct EmitSound_t;
struct SndOpEventGuid_t;
class CEntityKeyValues;
class CBaseModelEntity;
class CTakeDamageInfo;
class INetworkStringTable;
class CBasePlayerPawn;
class CBasePlayerWeapon;
class CServerSideClient;
class CServerSideClientBase;
class CNetMessage;
class CCSWeaponBaseVData;
class CCLCMsg_RespondCvarValue_t;
class CEconItemSchema;

namespace addresses {
	inline IGameEventListener2* (*GetLegacyGameEventListener)(CPlayerSlot slot);

	inline void (*CBasePlayerController_SetPawn)(CBasePlayerController* controller, CBasePlayerPawn* pawn, bool, bool, bool);

	inline CEntityInstance* (*CGameEntitySystem_FindEntityByClassName)(CEntitySystem* entitySystem, CEntityInstance* startEntity, const char* szName);

	inline CEntityInstance* (*CGameEntitySystem_FindEntityByName)(CEntitySystem* entitySystem, CEntityInstance* startEntity, const char* name, CEntityInstance* searchingEntity, CEntityInstance* activator, CEntityInstance* caller, IEntityFindFilter* filter);

	inline CEntityInstance* (*CreateEntityByName)(const char* className, int forceEdictIndex);

	inline void (*DispatchSpawn)(CEntityInstance* entity, CEntityKeyValues* entityKeyValues);

	inline void (*CBaseModelEntity_SetModel)(CBaseModelEntity* entity, const char* model);

	inline void (*UTIL_Remove)(CEntityInstance*);

	inline void (*CEntityInstance_AcceptInput)(CEntityInstance* self, const char* inputName, CEntityInstance* activator, CEntityInstance* caller, variant_t* value, int outputID);

	inline void (*CEntityIdentity_SetEntityName)(CEntityIdentity* entity, const char* name);

	inline void (*CBaseEntity_EmitSoundParams)(CBaseEntity* entity, const char* sound, int pitch, float volume, float delay);

	inline void (*CBaseEntity_SetParent)(CBaseEntity* entity, CBaseEntity* newParent, CUtlStringToken nBoneOrAttachName, matrix3x4a_t* offsetTransform);

	inline SndOpEventGuid_t (*CBaseEntity_EmitSoundFilter)(IRecipientFilter& filter, CEntityIndex ent, const EmitSound_t& params);

	inline void (*CBaseEntity_SetMoveType)(CBaseEntity* self, MoveType_t moveType, MoveCollide_t moveCollide);

	inline void (*CTakeDamageInfo_Constructor)(CTakeDamageInfo* self, CBaseEntity* inflictor, CBaseEntity* attacker, CBaseEntity* ability, const Vector* damageForce, const Vector* damagePosition, float damage, int bitsDamageType, int customDamage, void* a10);

	inline void (*CNetworkStringTable_DeleteAllStrings)(INetworkStringTable* self);

	// cs go

	inline void (*CCSPlayer_WeaponServices_RemoveItem)(CPlayer_WeaponServices* player, CBasePlayerWeapon* weapon);

	inline void (*CCSPlayerController_SwitchTeam)(CBasePlayerController* controller, int team);

	inline void (*CGameRules_TerminateRound)(CGameRules* gameRules, float delay, unsigned int reason, int64 a4, unsigned int a5);

	inline CCSWeaponBaseVData* (*GetCSWeaponDataFromKey)(int, const char*);

	//inline int16_t (*GetItemDefinitionByName)(CEconItemSchema*, const char*);

	inline v8::MaybeLocal<v8::Module> (*CSScript_ResolveModule)(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer);

}// namespace addresses

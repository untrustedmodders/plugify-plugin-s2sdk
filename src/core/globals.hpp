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
inline std::map<int, CTeam*> g_pTeamManagers;
//inline ISteamHTTP* g_http = nullptr;
inline CSteamGameServerAPIContext g_SteamAPI = {};
inline plg::set<plg::string> g_Precached;

class CoreConfig;
class GameConfig;

extern CoreConfig* g_pCoreConfig;
extern GameConfig* g_pGameConfig;

namespace globals {
	void Initialize(std::map<plg::string, plg::string> paths);
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

	inline CEntityInstance* (*CGameEntitySystem_FindEntityByClassName)(CEntitySystem* pEntitySystem, CEntityInstance* pStartEntity, const char* szName);

	inline CEntityInstance* (*CGameEntitySystem_FindEntityByName)(CEntitySystem* pEntitySystem, CEntityInstance* pStartEntity, const char* szName, CEntityInstance* pSearchingEntity, CEntityInstance* pActivator, CEntityInstance* pCaller, IEntityFindFilter* pFilter);

	inline CEntityInstance* (*CreateEntityByName)(const char* className, int iForceEdictIndex);

	inline void (*DispatchSpawn)(CEntityInstance* pEntity, CEntityKeyValues* pEntityKeyValues);

	inline void (*CBaseModelEntity_SetModel)(CBaseModelEntity* pModel, const char* szModel);

	inline void (*UTIL_Remove)(CEntityInstance*);

	inline void (*CEntityInstance_AcceptInput)(CEntityInstance* pThis, const char* pInputName, CEntityInstance* pActivator, CEntityInstance* pCaller, variant_t* value, int nOutputID);

	inline void (*CEntityIdentity_SetEntityName)(CEntityIdentity* pEntity, const char* pName);

	inline void (*CBaseEntity_EmitSoundParams)(CBaseEntity* pEntity, const char* pszSound, int nPitch, float flVolume, float flDelay);

	inline void (*CBaseEntity_SetParent)(CBaseEntity* pEntity, CBaseEntity* pNewParent, CUtlStringToken nBoneOrAttachName, matrix3x4a_t* pOffsetTransform);

	inline SndOpEventGuid_t (*CBaseEntity_EmitSoundFilter)(IRecipientFilter& filter, CEntityIndex ent, const EmitSound_t& params);

	inline void (*CBaseEntity_SetMoveType)(CBaseEntity* pThis, MoveType_t nMoveType, MoveCollide_t nMoveCollide);

	inline void (*CTakeDamageInfo_Constructor)(CTakeDamageInfo* pThis, CBaseEntity* pInflictor, CBaseEntity* pAttacker, CBaseEntity* pAbility, const Vector* vecDamageForce, const Vector* vecDamagePosition, float flDamage, int bitsDamageType, int iCustomDamage, void* a10);

	inline void (*CNetworkStringTable_DeleteAllStrings)(INetworkStringTable* pThis);

	// cs go

	inline void (*CCSPlayer_WeaponServices_RemoveItem)(CPlayer_WeaponServices* player, CBasePlayerWeapon* weapon);

	inline void (*CCSPlayerController_SwitchTeam)(CBasePlayerController* controller, int team);

	inline void (*CGameRules_TerminateRound)(CGameRules* pGameRules, float delay, unsigned int reason, int64 a4, unsigned int a5);

	inline CCSWeaponBaseVData* (*GetCSWeaponDataFromKey)(int, const char*);

	//inline int16_t (*GetItemDefinitionByName)(CEconItemSchema*, const char*);

	inline v8::MaybeLocal<v8::Module> (*CSScriptResolveModule)(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer);

}// namespace addresses

using HostStateRequestFn = void* (*)(CHostStateMgr *pMgrDoNotUse, CHostStateRequest* pRequest);
using ReplyConnectionFn = void (*)(CNetworkGameServerBase *server, CServerSideClient* client);
using SendNetMessageFn = void* (*)(const CServerSideClientBase*, const CNetMessage* data, uint8 bufType);
using ProcessRespondCvarValueFn = void* (*)(const CServerSideClientBase*, const CCLCMsg_RespondCvarValue_t& msg);

inline HostStateRequestFn g_pfnSetPendingHostStateRequest;
inline ReplyConnectionFn g_pfnReplyConnection;
inline SendNetMessageFn g_pfnSendNetMessage;
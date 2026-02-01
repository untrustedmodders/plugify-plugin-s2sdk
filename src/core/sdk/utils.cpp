#include "utils.hpp"

#include <engine/igameeventsystem.h>
#include <entity2/entitysystem.h>
#include <igameevents.h>
#include <iserver.h>
#include <netmessages.h>
#include <networkbasetypes.pb.h>
#include <networksystem/inetworkmessages.h>
#include <recipientfilter.h>

#include "entity/cbaseplayerpawn.h"
#include "entity/cplayercontroller.h"
#include "entity/globaltypes.h"
#include "core/player_manager.hpp"

/*bool utils::TraceLine(const Vector& vecStart, const Vector& vecEnd, CEntityInstance* ignore1, CGameTrace* tr, uint64 traceLayer, uint64 excludeLayer) {
	Ray_t ray;
	CTraceFilter filter;
	filter.SetPassEntity1(ignore1);
	filter.m_nInteractsWith = traceLayer;
	filter.m_nInteractsExclude = excludeLayer;

	return MEM::CALL::TraceShape(ray, vecStart, vecEnd, filter, tr);
}

void utils::GetPlayerAiming(CCSPlayerPawnBase* pPlayer, CGameTrace& ret) {
	Vector from = pPlayer->GetEyePosition();

	Vector forward;
	AngleVectors(pPlayer->m_angEyeAngles(), &forward);
	Vector to = from + forward * MAX_COORD_FLOAT;

	TraceLine(from, to, pPlayer, &ret, MASK_SOLID, CONTENTS_TRIGGER | CONTENTS_PLAYER);
}

CBaseEntity* utils::CreateBeam(const Vector& from, const Vector& to, Color color, float width, CBaseEntity* owner) {
	CBeam* beam = (CBeam*)MEM::CALL::CreateEntityByName("beam");
	if (!beam) {
		return nullptr;
	}

	beam->Teleport(&from, nullptr, nullptr);

	beam->m_clrRender(color);
	beam->m_fWidth(width);
	beam->m_vecEndPos(to);
	beam->m_fadeMinDist(-1.0f);

	if (owner != nullptr) {
		beam->m_hOwnerEntity(owner->GetRefEHandle());
	}

	beam->DispatchSpawn();

	return beam;
}*/

CBaseEntity* utils::FindEntityByClassname(CEntityInstance* start, const char* name) {
	if (!g_pGameEntitySystem) {
		return nullptr;
	}
	EntityInstanceByClassIter_t iter(start, name);
	return static_cast<CBaseEntity*>(iter.Next());
}

#define FCVAR_FLAGS_TO_REMOVE (FCVAR_HIDDEN | FCVAR_DEVELOPMENTONLY | FCVAR_MISSING0 | FCVAR_MISSING1 | FCVAR_MISSING2 | FCVAR_MISSING3)

CBasePlayerController* utils::GetController(CBaseEntity* entity) {
	std::string_view name(entity->GetClassname());
	if (name == "observer") {
		CBasePlayerPawn* pawn = static_cast<CBasePlayerPawn*>(entity);
		if (!pawn->m_hController->IsValid() || pawn->m_hController == nullptr) {
			for (int i = 0; i < gpGlobals->maxClients; ++i) {
				CPlayerController* controller = static_cast<CPlayerController*>(utils::GetController(CPlayerSlot(i)));
				if (controller && controller->GetObserverPawn() && controller->GetObserverPawn() == entity) {
					return controller;
				}
			}
			return nullptr;
		}
		return *pawn->m_hController;
	}
	if (entity->IsPlayerPawn()) {
		CBasePlayerPawn* pawn = static_cast<CBasePlayerPawn*>(entity);
		if (!pawn->m_hController->IsValid() || pawn->m_hController == nullptr) {
			// Seems like the pawn lost its controller, we can try looping through the controllers to find this pawn instead.
			for (int i = 0; i < gpGlobals->maxClients; ++i) {
				CPlayerController* controller = static_cast<CPlayerController*>(utils::GetController(CPlayerSlot(i)));
				if (controller && controller->GetPlayerPawn() && controller->GetPlayerPawn() == entity) {
					return controller;
				}
			}
			return nullptr;
		}
		return *pawn->m_hController;
	} else if (entity->IsPlayerController()) {
		return static_cast<CBasePlayerController*>(entity);
	} else {
		return nullptr;
	}
}

CBasePlayerController* utils::GetController(CPlayerSlot slot) {
	if (!g_pGameEntitySystem || !IsPlayerSlot(slot)) {
		return nullptr;
	}

	return static_cast<CBasePlayerController*>(g_pGameEntitySystem->GetEntityInstance(CEntityIndex(slot + 1)));
}

CPlayerSlot utils::GetEntityPlayerSlot(CBaseEntity* entity) {
	CBasePlayerController* controller = utils::GetController(entity);
	if (!controller) {
		return -1;
	} else {
		return controller->m_pEntity->m_EHandle.GetEntryIndex() - 1;
	}
}

bool utils::IsPlayerSlot(CPlayerSlot slot) {
	return slot >= 0 && slot < gpGlobals->maxClients;
}

CUtlClientVector* utils::GetClientList() {
	if (!g_pNetworkGameServer)
		return nullptr;

	return &g_pNetworkGameServer->m_Clients;
}

CServerSideClientBase* utils::GetClientBySlot(CPlayerSlot slot) {
	if (!utils::IsPlayerSlot(slot))
		return nullptr;

	CUtlVector<CServerSideClientBase*, CPlayerSlot>* clientList = GetClientList();
	return (clientList && GetController(slot)) ? clientList->Element(slot) : nullptr;
}

void utils::PlaySoundToClient(CPlayerSlot player, int channel, const char* soundName, float volume, soundlevel_t soundLevel, int flags, int pitch, const Vector& origin, float soundTime) {
	CSingleRecipientFilter filter(player.Get());
	EmitSound_t soundParams;
	soundParams.m_nChannel = channel;
	soundParams.m_pSoundName = soundName;
	soundParams.m_flVolume = volume;
	soundParams.m_SoundLevel = soundLevel;
	soundParams.m_nFlags = flags;
	soundParams.m_nPitch = pitch;
	soundParams.m_pOrigin = origin.IsValid() ? &origin : nullptr;
	soundParams.m_flSoundTime = soundTime;
	addresses::CBaseEntity_EmitSoundFilter(filter, player.Get() + 1, soundParams);
}

float utils::NormalizeDeg(float a) {
	a = fmod(a, 360.0f);
	if (a >= 180.0f)
		a -= 360.0f;
	else if (a < -180.0f)
		a += 360.0f;
	return a;
}

float utils::GetAngleDifference(float source, float target, float c, bool relative) {
	if (relative)
		return fmod((fmod(target - source, 2 * c) + 3 * c), 2 * c) - c;
	return fmod(fabs(target - source) + c, 2 * c) - c;
}

bool utils::IsSpawnValid(const Vector& origin) {
	/*bbox_t bounds = {{-16.0f, -16.0f, 0.0f}, {16.0f, 16.0f, 72.0f}};
	CTraceFilterS2 filter;
	filter.attr.m_bHitSolid = true;
	filter.attr.m_bHitSolidRequiresGenerateContacts = true;
	filter.attr.m_bShouldIgnoreDisabledPairs = true;
	filter.attr.m_nCollisionGroup = COLLISION_GROUP_PLAYER_MOVEMENT;
	filter.attr.m_nInteractsWith = 0x2c3011;
	filter.attr.m_bUnkFlag3 = true;
	filter.attr.m_nObjectSetMask = RNQUERY_OBJECTS_ALL;
	filter.attr.m_nInteractsAs = 0x40000;
	trace_t_s2 tr;
	addresses::TracePlayerBBox(origin, origin, bounds, &filter, tr);
	if (tr.fraction != 1.0 || tr.startsolid)
	{
		return false;
	}
	return true;*/
	return false;
}

bool utils::FindValidSpawn(Vector& origin, QAngle& angles) {
	bool foundValidSpawn = false;
	bool searchCT = false;
	Vector spawnOrigin;
	QAngle spawnAngles;
	CBaseEntity* spawnEntity = nullptr;
	while (!foundValidSpawn) {
		if (searchCT) {
			spawnEntity = FindEntityByClassname(spawnEntity, "info_player_counterterrorist");
		} else {
			spawnEntity = FindEntityByClassname(spawnEntity, "info_player_terrorist");
		}

		if (spawnEntity != nullptr) {
			spawnOrigin = spawnEntity->m_CBodyComponent->m_pSceneNode->m_vecAbsOrigin;
			spawnAngles = spawnEntity->m_CBodyComponent->m_pSceneNode->m_angRotation;
			if (utils::IsSpawnValid(spawnOrigin)) {
				origin = spawnOrigin;
				angles = spawnAngles;
				foundValidSpawn = true;
			}
		} else if (!searchCT) {
			searchCT = true;
		} else {
			break;
		}
	}
	return foundValidSpawn;
}

const fs::path& utils::GameDirectory() {
	static fs::path gameDirectory(Plat_GetGameDirectory());
	return gameDirectory;
}

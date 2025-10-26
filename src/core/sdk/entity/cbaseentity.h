/**
  * =============================================================================
 * s2sdk
 * Copyright (C) 2023-2025 untrustedmodders
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <core/game_config.hpp>
#include <core/sdk/schema.hpp>
#include <core/sdk/vscript.hpp>
#include <ehandle.h>
#include <entitykeyvalues.h>

#include "ccollisionproperty.h"
#include "ctakedamageinfo.h"
#include "globaltypes.h"
#include "services.h"

extern GameConfig* g_pGameConfig;

class CGameSceneNode {
public:
	DECLARE_SCHEMA_CLASS(CGameSceneNode)

	SCHEMA_FIELD(CEntityInstance*, m_pOwner)
	SCHEMA_FIELD(CGameSceneNode*, m_pParent)
	SCHEMA_FIELD(CGameSceneNode*, m_pChild)
	SCHEMA_FIELD(CNetworkOriginCellCoordQuantizedVector, m_vecOrigin)
	SCHEMA_FIELD(QAngle, m_angRotation)
	SCHEMA_FIELD(float, m_flScale)
	SCHEMA_FIELD(float, m_flAbsScale)
	SCHEMA_FIELD(Vector, m_vecAbsOrigin)
	SCHEMA_FIELD(QAngle, m_angAbsRotation)
	SCHEMA_FIELD(Vector, m_vRenderOrigin)

	matrix3x4_t EntityToWorldTransform() {
		matrix3x4_t mat;

		// issues with this and im tired so hardcoded it
		// AngleMatrix(this->m_angAbsRotation(), this->m_vecAbsOrigin(), mat);

		QAngle angles = this->m_angAbsRotation;
		float sr, sp, sy, cr, cp, cy;
		SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
		SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
		SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);
		mat[0][0] = cp * cy;
		mat[1][0] = cp * sy;
		mat[2][0] = -sp;

		float crcy = cr * cy;
		float crsy = cr * sy;
		float srcy = sr * cy;
		float srsy = sr * sy;
		mat[0][1] = sp * srcy - crsy;
		mat[1][1] = sp * srsy + crcy;
		mat[2][1] = sr * cp;

		mat[0][2] = (sp * crcy + srsy);
		mat[1][2] = (sp * crsy - srcy);
		mat[2][2] = cr * cp;

		Vector pos = this->m_vecAbsOrigin;
		mat[0][3] = pos.x;
		mat[1][3] = pos.y;
		mat[2][3] = pos.z;

		return mat;
	}
};

class CBodyComponent {
public:
	DECLARE_SCHEMA_CLASS(CBodyComponent)

	SCHEMA_FIELD(CGameSceneNode*, m_pSceneNode)
};

class CModelState {
public:
	DECLARE_SCHEMA_CLASS(CModelState)

	SCHEMA_FIELD(CUtlSymbolLarge, m_ModelName)
};

class CSkeletonInstance : public CGameSceneNode {
public:
	DECLARE_SCHEMA_CLASS(CSkeletonInstance)

	SCHEMA_FIELD(CModelState, m_modelState)
};

class CEntitySubclassVDataBase {
public:
	DECLARE_SCHEMA_CLASS(CEntitySubclassVDataBase)
};

class CBaseEntity : public CEntityInstance {
public:
	DECLARE_SCHEMA_CLASS(CBaseEntity)

	SCHEMA_FIELD(CBodyComponent*, m_CBodyComponent)
	SCHEMA_FIELD(CBitVec<64>, m_isSteadyState)
	SCHEMA_FIELD(float, m_lastNetworkChange)
	SCHEMA_FIELD_POINTER(CNetworkTransmitComponent, m_NetworkTransmitComponent)
	SCHEMA_FIELD(int, m_iHealth)
	SCHEMA_FIELD(int, m_iMaxHealth)
	SCHEMA_FIELD(int, m_iTeamNum)
	SCHEMA_FIELD(bool, m_bLagCompensate)
	SCHEMA_FIELD(Vector, m_vecAbsVelocity)
	SCHEMA_FIELD(Vector, m_vecBaseVelocity)
	SCHEMA_FIELD(CCollisionProperty*, m_pCollision)
	SCHEMA_FIELD(MoveCollide_t, m_MoveCollide)
	SCHEMA_FIELD(MoveType_t, m_MoveType)
	SCHEMA_FIELD(MoveType_t, m_nActualMoveType)
	SCHEMA_FIELD(CHandle<CBaseEntity>, m_hEffectEntity)
	SCHEMA_FIELD(uint32, m_spawnflags)
	SCHEMA_FIELD(uint32, m_fFlags)
	SCHEMA_FIELD(LifeState_t, m_lifeState)
	SCHEMA_FIELD(float, m_flDamageAccumulator)
	SCHEMA_FIELD(bool, m_bTakesDamage)
	SCHEMA_FIELD(TakeDamageFlags_t, m_nTakeDamageFlags)
	SCHEMA_FIELD_POINTER(CUtlStringToken, m_nSubclassID)
	SCHEMA_FIELD(float, m_flFriction)
	SCHEMA_FIELD(float, m_flGravityScale)
	SCHEMA_FIELD(bool, m_bGravityDisabled)
	SCHEMA_FIELD(bool, m_bGravityActuallyDisabled)
	SCHEMA_FIELD(float, m_flTimeScale)
	SCHEMA_FIELD(float, m_flSpeed)
	SCHEMA_FIELD(CUtlString, m_sUniqueHammerID);
	SCHEMA_FIELD(CUtlSymbolLarge, m_target);
	SCHEMA_FIELD(CUtlSymbolLarge, m_iGlobalname);
	SCHEMA_FIELD(CHandle<CBaseEntity>, m_hGroundEntity)
	SCHEMA_FIELD(float, m_flWaterLevel)
	SCHEMA_FIELD(int, m_fEffects)

	using string = const char*;
	using handle = CBaseEntity*;
	VSCRIPT_MEMBER_FUNCTION(AddEffects, void, int)
    VSCRIPT_MEMBER_FUNCTION(ApplyAbsVelocityImpulse, void, Vector)
    VSCRIPT_MEMBER_FUNCTION(ApplyLocalAngularVelocityImpulse, void, Vector)
    VSCRIPT_MEMBER_FUNCTION(Attribute_GetFloatValue, float, string, float)
    VSCRIPT_MEMBER_FUNCTION(Attribute_GetIntValue, int, string, int)
    VSCRIPT_MEMBER_FUNCTION(Attribute_SetFloatValue, void, string, float)
    VSCRIPT_MEMBER_FUNCTION(Attribute_SetIntValue, void, string, int)
    VSCRIPT_MEMBER_FUNCTION(DeleteAttribute, void, string)
    VSCRIPT_MEMBER_FUNCTION(EmitSound, void, string)
    VSCRIPT_MEMBER_FUNCTION(EmitSoundParams, void, string, int, float, float)
    VSCRIPT_MEMBER_FUNCTION(EyeAngles, QAngle)
    VSCRIPT_MEMBER_FUNCTION(EyePosition, Vector)
    VSCRIPT_MEMBER_FUNCTION(FirstMoveChild, handle)
    VSCRIPT_MEMBER_FUNCTION(FollowEntity, void, handle, bool)
    VSCRIPT_MEMBER_FUNCTION(FollowEntityMerge, void, handle, string)
    VSCRIPT_MEMBER_FUNCTION(GatherCriteria, void, handle)
    VSCRIPT_MEMBER_FUNCTION(GetAbsOrigin, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetAbsScale, float)
    VSCRIPT_MEMBER_FUNCTION(GetAngles, QAngle)
    VSCRIPT_MEMBER_FUNCTION(GetAnglesAsVector, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetAngularVelocity, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetBaseVelocity, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetBoundingMaxs, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetBoundingMins, Vector)
    //VSCRIPT_MEMBER_FUNCTION(GetBounds, table)
    VSCRIPT_MEMBER_FUNCTION(GetCenter, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetChildren, handle)
    //VSCRIPT_MEMBER_FUNCTION(GetContext, table, string)
    VSCRIPT_MEMBER_FUNCTION(GetForwardVector, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetHealth, int)
    VSCRIPT_MEMBER_FUNCTION(GetLeftVector, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetLocalAngles, QAngle)
    VSCRIPT_MEMBER_FUNCTION(GetLocalAngularVelocity, QAngle)
    VSCRIPT_MEMBER_FUNCTION(GetLocalOrigin, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetLocalScale, float)
    VSCRIPT_MEMBER_FUNCTION(GetLocalVelocity, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetMass, float)
    VSCRIPT_MEMBER_FUNCTION(GetMaxHealth, int)
    VSCRIPT_MEMBER_FUNCTION(GetModelName, string)
    VSCRIPT_MEMBER_FUNCTION(GetMoveParent, handle)
    VSCRIPT_MEMBER_FUNCTION(GetOrigin, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetOwner, handle)
    VSCRIPT_MEMBER_FUNCTION(GetOwnerEntity, handle)
    VSCRIPT_MEMBER_FUNCTION(GetRightVector, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetRootMoveParent, handle)
    VSCRIPT_MEMBER_FUNCTION(GetSoundDuration, float, string, string)
    VSCRIPT_MEMBER_FUNCTION(GetSpawnGroupHandle, int)
    VSCRIPT_MEMBER_FUNCTION(GetTeam, int)
    VSCRIPT_MEMBER_FUNCTION(GetTeamNumber, int)
    VSCRIPT_MEMBER_FUNCTION(GetUpVector, Vector)
    VSCRIPT_MEMBER_FUNCTION(GetVelocity, Vector)
    VSCRIPT_MEMBER_FUNCTION(HasAttribute, bool, string)
    VSCRIPT_MEMBER_FUNCTION(IsAlive, bool)
    VSCRIPT_MEMBER_FUNCTION(IsNPC, bool)
    VSCRIPT_MEMBER_FUNCTION(IsPlayerController, bool)
    VSCRIPT_MEMBER_FUNCTION(IsPlayerPawn, bool)
    VSCRIPT_MEMBER_FUNCTION(Kill, void)
    VSCRIPT_MEMBER_FUNCTION(NextMovePeer, handle)
    VSCRIPT_MEMBER_FUNCTION(OverrideFriction, void, float, float)
    VSCRIPT_MEMBER_FUNCTION(PrecacheScriptSound, void, string)
    VSCRIPT_MEMBER_FUNCTION(RemoveEffects, void, int)
    VSCRIPT_MEMBER_FUNCTION(SetAbsAngles, void, float, float, float)
    VSCRIPT_MEMBER_FUNCTION(SetAbsOrigin, void, Vector)
    VSCRIPT_MEMBER_FUNCTION(SetAbsScale, void, float)
    VSCRIPT_MEMBER_FUNCTION(SetAngles, void, float, float, float)
    VSCRIPT_MEMBER_FUNCTION(SetAngularVelocity, void, float, float, float)
    VSCRIPT_MEMBER_FUNCTION(SetConstraint, void, Vector)
    VSCRIPT_MEMBER_FUNCTION(SetContext, void, string, string, float)
    VSCRIPT_MEMBER_FUNCTION(SetContextNum, void, string, float, float)
    VSCRIPT_MEMBER_FUNCTION(SetContextThink, void, string, handle, float)
    VSCRIPT_MEMBER_FUNCTION(SetEntityName, void, string)
    VSCRIPT_MEMBER_FUNCTION(SetForwardVector, void, Vector)
    VSCRIPT_MEMBER_FUNCTION(SetFriction, void, float)
    VSCRIPT_MEMBER_FUNCTION(SetGravity, void, float)
    VSCRIPT_MEMBER_FUNCTION(SetHealth, void, int)
    VSCRIPT_MEMBER_FUNCTION(SetLocalAngles, void, float, float, float)
    VSCRIPT_MEMBER_FUNCTION(SetLocalOrigin, void, Vector)
    VSCRIPT_MEMBER_FUNCTION(SetLocalScale, void, float)
    VSCRIPT_MEMBER_FUNCTION(SetMass, void, float)
    VSCRIPT_MEMBER_FUNCTION(SetMaxHealth, void, int)
    VSCRIPT_MEMBER_FUNCTION(SetOrigin, void, Vector)
    VSCRIPT_MEMBER_FUNCTION(SetOwner, void, handle)
    VSCRIPT_MEMBER_FUNCTION(SetParent, void, handle, string)
    VSCRIPT_MEMBER_FUNCTION(SetTeam, void, int)
    VSCRIPT_MEMBER_FUNCTION(SetVelocity, void, Vector)
    VSCRIPT_MEMBER_FUNCTION(StopSound, void, string)
    VSCRIPT_MEMBER_FUNCTION(TakeDamage, int, CTakeDamageInfo&)
    VSCRIPT_MEMBER_FUNCTION(TransformPointEntityToWorld, Vector, Vector)
    VSCRIPT_MEMBER_FUNCTION(TransformPointWorldToEntity, Vector, Vector)
    VSCRIPT_MEMBER_FUNCTION(Trigger, void)
    VSCRIPT_MEMBER_FUNCTION(ValidatePrivateScriptScope, void)

	int entindex() { return m_pEntity->m_EHandle.GetEntryIndex(); }

	const char* GetName() {
		return m_iGlobalname->String();
	}

	void SetCollisionGroup(StandardCollisionGroups_t nCollisionGroup) {
		if (m_pCollision == nullptr)
			return;

		m_pCollision->m_collisionAttribute->m_nCollisionGroup = nCollisionGroup;
		m_pCollision->m_CollisionGroup = nCollisionGroup;
		CollisionRulesChanged();
	}

	void CollisionRulesChanged() {
		TRY_GET_OFFSET(g_pGameConfig, "CBaseEntity::CollisionRulesChanged", offset);
		CALL_VIRTUAL(void, *offset, this);
	}

	void Teleport(const Vector* newPosition, const QAngle* newAngles, const Vector* newVelocity) {
		TRY_GET_OFFSET(g_pGameConfig, "CBaseEntity::Teleport", offset);
		CALL_VIRTUAL(bool, *offset, this, newPosition, newAngles, newVelocity);
	}

	CHandle<CBaseEntity> GetHandle() { return m_pEntity->m_EHandle; }

	// A double pointer to entity VData is available 4 bytes past m_nSubclassID, if applicable
	CEntitySubclassVDataBase* GetVData() {
		CUtlStringToken* subclassID = m_nSubclassID;
		return *reinterpret_cast<CEntitySubclassVDataBase**>(reinterpret_cast<uint8*>(subclassID) + 4);
	}

	void DispatchSpawn(CEntityKeyValues* kv = nullptr) {
		addresses::DispatchSpawn(this, kv);
	}

	void DispatchSpawn(const plg::vector<plg::string>& keys, const plg::vector<plg::any>& values) {
		CEntityKeyValues* ESKeyValues = new CEntityKeyValues(g_pGameEntitySystem->GetEntityKeyValuesAllocator(), EKV_ALLOCATOR_EXTERNAL);

		g_pGameEntitySystem->AddRefKeyValues(ESKeyValues);

		for (size_t i = 0; i < values.size(); ++i) {
			auto key = EntityKeyId_t::Make(keys[i].c_str());
			plg::visit([&](const auto& v) {
				using T = std::decay_t<decltype(v)>;
				if constexpr (std::is_same_v<T, plg::string>) {
					ESKeyValues->SetString(key, v.c_str());
				} else if constexpr (std::is_pointer_v<T>) {
					ESKeyValues->SetPtr(key, v);
				} else if constexpr (std::is_same_v<T, int32_t>) {
					ESKeyValues->SetInt(key, v);
				} else if constexpr (std::is_same_v<T, uint32_t>) {
					ESKeyValues->SetUint(key, v);
				} else if constexpr (std::is_same_v<T, int64_t>) {
					ESKeyValues->SetInt64(key, v);
				} else if constexpr (std::is_same_v<T, uint64_t>) {
					ESKeyValues->SetUint64(key, v);
				} else if constexpr (std::is_same_v<T, bool>) {
					ESKeyValues->SetBool(key, v);
				} else if constexpr (std::is_same_v<T, float>) {
					ESKeyValues->SetFloat(key, v);
				} else if constexpr (std::is_same_v<T, double>) {
					ESKeyValues->SetDouble(key, v);
				} else if constexpr (std::is_same_v<T, plg::vec2>) {
					ESKeyValues->SetVector2D(key, *reinterpret_cast<const Vector2D*>(&v));
				} else if constexpr (std::is_same_v<T, plg::vec3>) {
					ESKeyValues->SetVector(key, *reinterpret_cast<const Vector*>(&v));
				} else if constexpr (std::is_same_v<T, plg::vec4>) {
					ESKeyValues->SetVector4D(key, *reinterpret_cast<const Vector4D*>(&v));
				} else if constexpr (std::is_same_v<T, plg::mat4x4>) {
					ESKeyValues->SetMatrix3x4(key, *reinterpret_cast<const matrix3x4_t*>(&v));
				} else if constexpr (std::is_arithmetic_v<T>) {
					ESKeyValues->SetInt(key, static_cast<int>(v));
				}
			},
			values[i]);
		}

		DispatchSpawn(ESKeyValues);

		g_pGameEntitySystem->ReleaseKeyValues(ESKeyValues);
	}

	void DispatchSpawn(const plg::vector<std::pair<plg::string, plg::string>>& kv) {
		CEntityKeyValues* ESKeyValues = new CEntityKeyValues(g_pGameEntitySystem->GetEntityKeyValuesAllocator(), EKV_ALLOCATOR_EXTERNAL);

		g_pGameEntitySystem->AddRefKeyValues(ESKeyValues);

		for (const auto& [k, v] : kv) {
			auto key = EntityKeyId_t::Make(k.c_str());
			ESKeyValues->SetString(key, v.c_str());
		}

		DispatchSpawn(ESKeyValues);

		g_pGameEntitySystem->ReleaseKeyValues(ESKeyValues);
	}

	bool IsWeapon() {
		TRY_GET_OFFSET(g_pGameConfig, "CBaseEntity::IsWeapon", offset);
		return CALL_VIRTUAL(bool, *offset, this);
	}

	void Remove() {
		Kill();
	}

	void AcceptInput(const char* inputName, const variant_t& value = variant_t(), CEntityInstance* activator = nullptr, CEntityInstance* caller = nullptr, int outputId = 0) {
		addresses::CEntityInstance_AcceptInput(this, inputName, activator, caller, const_cast<variant_t*>(&value), outputId);
	}

	void SetMoveType(MoveType_t nMoveType) {
		addresses::CBaseEntity_SetMoveType(this, nMoveType, m_MoveCollide);
	}

	const char* GetName() const { return m_pEntity->m_name.String(); }
};

class SpawnPoint : public CBaseEntity {
public:
	DECLARE_SCHEMA_CLASS(SpawnPoint);

	SCHEMA_FIELD(bool, m_bEnabled);
};

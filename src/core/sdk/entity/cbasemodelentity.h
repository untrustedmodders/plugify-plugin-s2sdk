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

#include "cbaseentity.h"

class CBaseModelEntity : public CBaseEntity {
public:
	DECLARE_SCHEMA_CLASS(CBaseModelEntity);

	SCHEMA_FIELD(CCollisionProperty, m_Collision)
	SCHEMA_FIELD(Color, m_clrRender)
	SCHEMA_FIELD(uint8, m_nRenderMode)

	VSCRIPT_MEMBER_FUNCTION(GetAttachmentAngles, Vector, int)
    VSCRIPT_MEMBER_FUNCTION(GetAttachmentForward, Vector, int)
    VSCRIPT_MEMBER_FUNCTION(GetAttachmentOrigin, Vector, int)
    VSCRIPT_MEMBER_FUNCTION(GetMaterialGroupHash, unsigned)
    VSCRIPT_MEMBER_FUNCTION(GetMaterialGroupMask, uint64)
    VSCRIPT_MEMBER_FUNCTION(GetModelScale, float)
    VSCRIPT_MEMBER_FUNCTION(GetRenderAlpha, int)
    VSCRIPT_MEMBER_FUNCTION(GetRenderColor, Vector)
    VSCRIPT_MEMBER_FUNCTION(ScriptLookupAttachment, int, string)
    VSCRIPT_MEMBER_FUNCTION(SetBodygroup, void, int, int)
    VSCRIPT_MEMBER_FUNCTION(SetBodygroupByName, void, string, int)
    VSCRIPT_MEMBER_FUNCTION(SetLightGroup, void, string)
    VSCRIPT_MEMBER_FUNCTION(SetMaterialGroup, void, string)
    VSCRIPT_MEMBER_FUNCTION(SetMaterialGroupHash, void, unsigned)
    VSCRIPT_MEMBER_FUNCTION(SetMaterialGroupMask, void, uint64)
    VSCRIPT_MEMBER_FUNCTION(SetModel, void, string)
    VSCRIPT_MEMBER_FUNCTION(SetModelScale, void, float)
    VSCRIPT_MEMBER_FUNCTION(SetRenderAlpha, void, int)
    VSCRIPT_MEMBER_FUNCTION(SetRenderColor, void, int, int, int)
    VSCRIPT_MEMBER_FUNCTION(SetRenderMode, void, int)
    VSCRIPT_MEMBER_FUNCTION(SetSingleMeshGroup, void, string)
    VSCRIPT_MEMBER_FUNCTION(SetSize, void, Vector, Vector)
    VSCRIPT_MEMBER_FUNCTION(SetSkin, void, int)

	const char* GetModelName() {
		CGameSceneNode* sceneNode = m_CBodyComponent->m_pSceneNode;
		return static_cast<CSkeletonInstance*>(sceneNode)->m_modelState->m_ModelName->String();
	}
};

class CBaseViewModel : public CBaseModelEntity {
public:
	DECLARE_SCHEMA_CLASS(CBaseViewModel);

	SCHEMA_FIELD(int, m_nViewModelIndex);
};

class CBeam : public CBaseModelEntity {
public:
	DECLARE_SCHEMA_CLASS(CBeam);

	SCHEMA_FIELD(float, m_fWidth);
	SCHEMA_FIELD(Vector, m_vecEndPos);
};

class CModelPointEntity : public CBaseModelEntity {
public:
	DECLARE_SCHEMA_CLASS(CModelPointEntity);
};

class CPointWorldText : public CModelPointEntity {
public:
	DECLARE_SCHEMA_CLASS(CPointWorldText);

	SCHEMA_FIELD_POINTER(char, m_messageText);

	void SetText(const char* msg) {
		AcceptInput("SetMessage", msg);
	}

	void Enable() {
		AcceptInput("Enable");
	}

	void Disable() {
		AcceptInput("Disable");
	}
};
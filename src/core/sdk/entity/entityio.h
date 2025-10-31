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
#include <string_t.h>
#include <entityhandle.h>
#include <entitysystem.h>

struct EntityIOConnectionDesc_t {
	string_t m_targetDesc;
	string_t m_targetInput;
	string_t m_valueOverride;
	CEntityHandle m_hTarget;
	EntityIOTargetType_t m_nTargetType;
	int32 m_nTimesToFire;
	float m_delay;
};

struct EntityIOConnection_t : EntityIOConnectionDesc_t {
	bool m_bMarkedForRemoval;
	EntityIOConnection_t* m_pNext;
};

struct EntityIOOutputDesc_t {
	const char* m_pName;
	uint32 m_nFlags;
	uint32 m_nOutputOffset;
};

class CEntityIOOutput {
public:
	void* vtable;
	EntityIOConnection_t* m_pConnections;
	EntityIOOutputDesc_t* m_pDesc;
};

class EntityIO {
public:
	DECLARE_VSCRIPT_CLASS2(EntityIO)

	using handle = HSCRIPT;
	using string = const char*;
	using vector = const Vector&;
	VSCRIPT_GLOBAL_FUNCTION(DoEntFire, void, string, string, string, float, handle, handle)
	VSCRIPT_GLOBAL_FUNCTION(DoEntFireByInstanceHandle, void, handle, string, string, float, handle, handle)
	VSCRIPT_GLOBAL_FUNCTION(FireEntityIOInputNameOnly, void, handle, string)
	VSCRIPT_GLOBAL_FUNCTION(FireEntityIOInputString, void, handle, string, string)
	VSCRIPT_GLOBAL_FUNCTION(FireEntityIOInputVec, void, handle, string, vector)
	VSCRIPT_GLOBAL_FUNCTION(CancelEntityIOEvents, void, handle)
	//VSCRIPT_GLOBAL_FUNCTION(SetRenderingEnabled, void, handle, bool)
	//VSCRIPT_GLOBAL_FUNCTION(CreateEffect, bool, table)
	//VSCRIPT_GLOBAL_FUNCTION(StopEffect, void, table, string)
};
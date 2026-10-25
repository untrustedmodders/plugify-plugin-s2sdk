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
#include <entity2/entityio.h>

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
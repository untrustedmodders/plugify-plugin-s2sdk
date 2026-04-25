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

class Sounds {
public:
	DECLARE_VSCRIPT_CLASS2(Sounds)

	using string = const char*;
	using handle = HSCRIPT;

	VSCRIPT_GLOBAL_FUNCTION(EmitSoundOn, void, string, handle)
	VSCRIPT_GLOBAL_FUNCTION(EmitSoundOnClient, void, string, handle)
	VSCRIPT_GLOBAL_FUNCTION(StartSoundEvent, void, string, handle)
	VSCRIPT_GLOBAL_FUNCTION(StartSoundEventFromPosition, void, string, Vector)
	VSCRIPT_GLOBAL_FUNCTION(StartSoundEventFromPositionReliable, void, string, Vector)
	VSCRIPT_GLOBAL_FUNCTION(StartSoundEventFromPositionUnreliable, void, string, Vector)
	VSCRIPT_GLOBAL_FUNCTION(StartSoundEventReliable, void, string, handle)
	VSCRIPT_GLOBAL_FUNCTION(StartSoundEventUnreliable, void, string, handle)
	VSCRIPT_GLOBAL_FUNCTION(StopSoundEvent,	void, string, handle)
	VSCRIPT_GLOBAL_FUNCTION(StopSoundOn, void, string, handle)
};
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

class Debug {
public:
	DECLARE_SCHEMA_CLASS(Debug);

	VSCRIPT_GLOBAL_FUNCTION(DebugBreak, void)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawBox, void, Vector, Vector, Vector, int, int, int, int, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawBoxDirection, void, Vector, Vector, Vector, Vector, Vector, float, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawCircle, void, Vector, Vector, float, float, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawClear, void)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawLine, void, Vector, Vector, int, int, int, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawLine_vCol, void, Vector, Vector, Vector, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawScreenTextLine, void, float, float, int, const char*, int, int, int, int, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawSphere, void, Vector, Vector, float, float, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawText, void, Vector, const char*, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugScreenTextPretty, void, float, float, int, const char*, int, int, int, int, float, const char*, int, bool)
	VSCRIPT_GLOBAL_FUNCTION(DoScriptAssert, void, bool, const char*)
};

class Printing {
public:
	DECLARE_SCHEMA_CLASS(Printing);

	VSCRIPT_GLOBAL_FUNCTION(Msg, void, const char*)
	VSCRIPT_GLOBAL_FUNCTION(PrintLinkedConsoleMessage, void, const char*, const char*)
	VSCRIPT_GLOBAL_FUNCTION(Say, void, HSCRIPT, const char*, bool)
	VSCRIPT_GLOBAL_FUNCTION(ScriptPrintMessageCenterAll, void, const char*)
	VSCRIPT_GLOBAL_FUNCTION(ScriptPrintMessageCenterAllWithParams, void, const char*, const char*, const char*, const char*)
	VSCRIPT_GLOBAL_FUNCTION(ScriptPrintMessageCenterTeam, void, int, const char*)
	VSCRIPT_GLOBAL_FUNCTION(ScriptPrintMessageChatAll, void, const char*)
	VSCRIPT_GLOBAL_FUNCTION(ScriptPrintMessageChatTeam, void, int, const char*)
	VSCRIPT_GLOBAL_FUNCTION(ShowMessage, void, const char*)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_MessageText, void, int, const char*, int, int, int, int)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_MessageTextAll, void, const char*, int, int, int, int)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_MessageTextAll_WithContext, void, const char*, int, int, int, int, HSCRIPT)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_MessageText_WithContext, void, int, const char*, int, int, int, int, HSCRIPT)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_ResetMessageText, void, int)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_ResetMessageTextAll, void)
	VSCRIPT_GLOBAL_FUNCTION(Warning, void, const char*)
};

#if 0
class CDebugOverlayScriptHelper {
public:
	DECLARE_SCHEMA_CLASS(CDebugOverlayScriptHelper);

	VSCRIPT_MEMBER_FUNCTION(Axis, void, Vector, Quaternion, float, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Box, void, Vector, Vector, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(BoxAngles, void, Vector, Vector, Vector, Quaternion, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Capsule, void, Vector, Quaternion, float, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Circle, void, Vector, Quaternion, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(CircleScreenOriented, void, Vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Cone, void, Vector, Vector, float, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Cross, void, Vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Cross3D, void, Vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Cross3DOriented, void, Vector, Quaternion, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(DrawTickMarkedLine, void, Vector, Vector, float, int, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(EntityAttachments, void, HSCRIPT, float, float)
	VSCRIPT_MEMBER_FUNCTION(EntityAxis, void, HSCRIPT, float, bool, float)
	VSCRIPT_MEMBER_FUNCTION(EntityBounds, void, HSCRIPT, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(EntitySkeleton, void, HSCRIPT, float)
	VSCRIPT_MEMBER_FUNCTION(EntityText, void, HSCRIPT, int, const char*, int, int, int, int, float)
	VSCRIPT_MEMBER_FUNCTION(FilledRect2D, void, Vector2D, Vector2D, int, int, int, int, float)
	VSCRIPT_MEMBER_FUNCTION(HorzArrow, void, Vector, Vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Line, void, Vector, Vector, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Line2D, void, Vector2D, Vector2D, int, int, int, int, float)
	VSCRIPT_MEMBER_FUNCTION(PopDebugOverlayScope, void)
	VSCRIPT_MEMBER_FUNCTION(PushAndClearDebugOverlayScope, void, CUtlStringToken)
	VSCRIPT_MEMBER_FUNCTION(PushDebugOverlayScope, void, CUtlStringToken)
	VSCRIPT_MEMBER_FUNCTION(RemoveAllInScope, void, CUtlStringToken)
	VSCRIPT_MEMBER_FUNCTION(SolidCone, void, Vector, Vector, float, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Sphere, void, Vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(SweptBox, void, Vector, Vector, Vector, Vector, Quaternion, int, int, int, int, float)
	VSCRIPT_MEMBER_FUNCTION(Text, void, Vector, int, const char*, float, int, int, int, int, float)
	VSCRIPT_MEMBER_FUNCTION(Texture, void, const char*, Vector2D, Vector2D, int, int, int, int, Vector2D, Vector2D, float)
	VSCRIPT_MEMBER_FUNCTION(Triangle, void, Vector, Vector, Vector, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(VectorText3D, void, Vector, Quaternion, const char*, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(VertArrow, void, Vector, Vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(YawArrow, void, Vector, float, float, float, int, int, int, int, bool, float)
};

#endif
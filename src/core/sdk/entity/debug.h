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
	DECLARE_VSCRIPT_CLASS2(Debug);

	using handle = HSCRIPT;
	using string = const char*;
	using vector = Vector;
	VSCRIPT_GLOBAL_FUNCTION(DebugBreak, void)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawBox, void, vector, vector, vector, int, int, int, int, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawBoxDirection, void, vector, vector, vector, vector, vector, float, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawCircle, void, vector, vector, float, float, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawClear, void)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawLine, void, vector, vector, int, int, int, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawLine_vCol, void, vector, vector, vector, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawScreenTextLine, void, float, float, int, string, int, int, int, int, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawSphere, void, vector, vector, float, float, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawText, void, vector, string, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugScreenTextPretty, void, float, float, int, string, int, int, int, int, float, string, int, bool)
	VSCRIPT_GLOBAL_FUNCTION(DoScriptAssert, void, bool, string)
};

class Printing {
public:
	DECLARE_VSCRIPT_CLASS2(Printing);

	using handle = HSCRIPT;
	using string = const char*;
	VSCRIPT_GLOBAL_FUNCTION(Msg, void, string)
	VSCRIPT_GLOBAL_FUNCTION(PrintLinkedConsoleMessage, void, string, string)
	VSCRIPT_GLOBAL_FUNCTION(Say, void, handle, string, bool)
	VSCRIPT_GLOBAL_FUNCTION(ScriptPrintMessageCenterAll, void, string)
	VSCRIPT_GLOBAL_FUNCTION(ScriptPrintMessageCenterAllWithParams, void, string, string, string, string)
	VSCRIPT_GLOBAL_FUNCTION(ScriptPrintMessageCenterTeam, void, int, string)
	VSCRIPT_GLOBAL_FUNCTION(ScriptPrintMessageChatAll, void, string)
	VSCRIPT_GLOBAL_FUNCTION(ScriptPrintMessageChatTeam, void, int, string)
	VSCRIPT_GLOBAL_FUNCTION(ShowMessage, void, string)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_MessageText, void, int, string, int, int, int, int)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_MessageTextAll, void, string, int, int, int, int)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_MessageTextAll_WithContext, void, string, int, int, int, int, handle)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_MessageText_WithContext, void, int, string, int, int, int, int, handle)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_ResetMessageText, void, int)
	VSCRIPT_GLOBAL_FUNCTION(UTIL_ResetMessageTextAll, void)
	VSCRIPT_GLOBAL_FUNCTION(Warning, void, string)
};

#if 0
class CDebugOverlayScriptHelper {
public:
	DECLARE_VSCRIPT_CLASS(CDebugOverlayScriptHelper);

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
	VSCRIPT_MEMBER_FUNCTION(EntityAttachments, void, handle, float, float)
	VSCRIPT_MEMBER_FUNCTION(EntityAxis, void, handle, float, bool, float)
	VSCRIPT_MEMBER_FUNCTION(EntityBounds, void, handle, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(EntitySkeleton, void, handle, float)
	VSCRIPT_MEMBER_FUNCTION(EntityText, void, handle, int, string, int, int, int, int, float)
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
	VSCRIPT_MEMBER_FUNCTION(Text, void, Vector, int, string, float, int, int, int, int, float)
	VSCRIPT_MEMBER_FUNCTION(Texture, void, string, Vector2D, Vector2D, int, int, int, int, Vector2D, Vector2D, float)
	VSCRIPT_MEMBER_FUNCTION(Triangle, void, Vector, Vector, Vector, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(VectorText3D, void, Vector, Quaternion, string, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(VertArrow, void, Vector, Vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(YawArrow, void, Vector, float, float, float, int, int, int, int, bool, float)
};

#endif
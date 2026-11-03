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
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawBox, void, const Vector&, const Vector&, const Vector&, int, int, int, int, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawBoxDirection, void, vector, vector, vector, vector, vector, float, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawCircle, void, vector, vector, float, float, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawClear, void)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawLine, void, const Vector&, const Vector&, int, int, int, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawLine_vCol, void, vector, vector, vector, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawScreenTextLine, void, float, float, int, string, int, int, int, int, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawSphere, void, vector, vector, float, float, bool, float)
	VSCRIPT_GLOBAL_FUNCTION(DebugDrawText, void, const Vector&, string, bool, float)
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

class CDebugOverlayScriptHelper {
public:
	DECLARE_VSCRIPT_CLASS(CDebugOverlayScriptHelper);

	using handle = int;
	using string = const char*;
	using vector = const Vector&;
	using vector2d = const Vector2D&;
	using quaternion = const Quaternion&;
	VSCRIPT_MEMBER_FUNCTION(Axis, void, vector, quaternion, float, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Box, void, vector, vector, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(BoxAngles, void, vector, vector, vector, quaternion, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Capsule, void, vector, quaternion, float, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Circle, void, vector, quaternion, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(CircleScreenOriented, void, vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Cone, void, vector, vector, float, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Cross, void, vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Cross3D, void, vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Cross3DOriented, void, vector, quaternion, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(DrawTickMarkedLine, void, vector, vector, float, int, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(EntityAttachments, void, handle, float, float)
	VSCRIPT_MEMBER_FUNCTION(EntityAxis, void, handle, float, bool, float)
	VSCRIPT_MEMBER_FUNCTION(EntityBounds, void, handle, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(EntitySkeleton, void, handle, float)
	VSCRIPT_MEMBER_FUNCTION(EntityText, void, handle, int, string, int, int, int, int, float)
	VSCRIPT_MEMBER_FUNCTION(FilledRect2D, void, vector2d, vector2d, int, int, int, int, float)
	VSCRIPT_MEMBER_FUNCTION(HorzArrow, void, vector, vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Line, void, vector, vector, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Line2D, void, vector2d, vector2d, int, int, int, int, float)
	VSCRIPT_MEMBER_FUNCTION(PopDebugOverlayScope, void)
	VSCRIPT_MEMBER_FUNCTION(PushAndClearDebugOverlayScope, void, CUtlStringToken)
	VSCRIPT_MEMBER_FUNCTION(PushDebugOverlayScope, void, CUtlStringToken)
	VSCRIPT_MEMBER_FUNCTION(RemoveAllInScope, void, CUtlStringToken)
	VSCRIPT_MEMBER_FUNCTION(SolidCone, void, vector, vector, float, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(Sphere, void, vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(SweptBox, void, vector, vector, vector, vector, quaternion, int, int, int, int, float)
	VSCRIPT_MEMBER_FUNCTION(Text, void, vector, int, string, float, int, int, int, int, float)
	VSCRIPT_MEMBER_FUNCTION(Texture, void, string, vector2d, vector2d, int, int, int, int, vector2d, vector2d, float)
	VSCRIPT_MEMBER_FUNCTION(Triangle, void, vector, vector, vector, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(VectorText3D, void, vector, quaternion, string, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(VertArrow, void, vector, vector, float, int, int, int, int, bool, float)
	VSCRIPT_MEMBER_FUNCTION(YawArrow, void, vector, float, float, float, int, int, int, int, bool, float)
};
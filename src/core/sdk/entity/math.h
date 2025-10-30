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

class Math {
public:
	using ThisClass = Math;
	static constexpr const char m_className[] = "";
    
	VSCRIPT_GLOBAL_FUNCTION(AngleDiff, float, float, float)
	VSCRIPT_GLOBAL_FUNCTION(AnglesToVector, Vector, QAngle)
	VSCRIPT_GLOBAL_FUNCTION(AxisAngleToQuaternion, Quaternion, Vector, float)
	VSCRIPT_GLOBAL_FUNCTION(CalcClosestPointOnEntityOBB, Vector, HSCRIPT, Vector)
	VSCRIPT_GLOBAL_FUNCTION(CalcDistanceBetweenEntityOBB, float, HSCRIPT, HSCRIPT)
	VSCRIPT_GLOBAL_FUNCTION(CalcDistanceToLineSegment2D, float, Vector, Vector, Vector)
	VSCRIPT_GLOBAL_FUNCTION(CrossVectors, Vector, Vector, Vector)
	VSCRIPT_GLOBAL_FUNCTION(ExponentialDecay, float, float, float, float)
	VSCRIPT_GLOBAL_FUNCTION(LerpVectors, Vector, Vector, Vector, float)
	VSCRIPT_GLOBAL_FUNCTION(QSlerp, QAngle, QAngle, QAngle, float)
	VSCRIPT_GLOBAL_FUNCTION(RotateOrientation, QAngle, QAngle, QAngle)
	VSCRIPT_GLOBAL_FUNCTION(RotatePosition, Vector, Vector, QAngle, Vector)
	VSCRIPT_GLOBAL_FUNCTION(RotateQuaternionByAxisAngle, Quaternion, Quaternion, Vector, float)
	VSCRIPT_GLOBAL_FUNCTION(RotationDelta, QAngle, QAngle, QAngle)
	VSCRIPT_GLOBAL_FUNCTION(RotationDeltaAsAngularVelocity, Vector, QAngle, QAngle)
	VSCRIPT_GLOBAL_FUNCTION(SplineQuaternions, Quaternion, Quaternion, Quaternion, float)
	VSCRIPT_GLOBAL_FUNCTION(SplineVectors, Vector, Vector, Vector, float)
	VSCRIPT_GLOBAL_FUNCTION(VectorToAngles, QAngle, Vector)
	VSCRIPT_GLOBAL_FUNCTION(RandomFloat, float, float, float)
	VSCRIPT_GLOBAL_FUNCTION(RandomInt, int, int, int)

};
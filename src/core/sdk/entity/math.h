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
	DECLARE_VSCRIPT_CLASS2(Math)

	using vector = const Vector&;
	using qangle = const QAngle&;
	using quaternion = const Quaternion&;

	VSCRIPT_GLOBAL_FUNCTION(AngleDiff, float, float, float)
	VSCRIPT_GLOBAL_FUNCTION(AnglesToVector, Vector, qangle)
	VSCRIPT_GLOBAL_FUNCTION(AxisAngleToQuaternion, Quaternion, vector, float)
	VSCRIPT_GLOBAL_FUNCTION(CalcClosestPointOnEntityOBB, Vector, HSCRIPT, vector)
	VSCRIPT_GLOBAL_FUNCTION(CalcDistanceBetweenEntityOBB, float, HSCRIPT, HSCRIPT)
	VSCRIPT_GLOBAL_FUNCTION(CalcDistanceToLineSegment2D, float, vector, vector, vector)
	VSCRIPT_GLOBAL_FUNCTION(CrossVectors, Vector, vector, vector)
	VSCRIPT_GLOBAL_FUNCTION(ExponentialDecay, float, float, float, float)
	VSCRIPT_GLOBAL_FUNCTION(LerpVectors, Vector, vector, vector, float)
	VSCRIPT_GLOBAL_FUNCTION(QSlerp, QAngle, qangle, qangle, float)
	VSCRIPT_GLOBAL_FUNCTION(RotateOrientation, QAngle, qangle, qangle)
	VSCRIPT_GLOBAL_FUNCTION(RotatePosition, Vector, vector, qangle, vector)
	VSCRIPT_GLOBAL_FUNCTION(RotateQuaternionByAxisAngle, Quaternion, quaternion, vector, float)
	VSCRIPT_GLOBAL_FUNCTION(RotationDelta, QAngle, qangle, qangle)
	VSCRIPT_GLOBAL_FUNCTION(RotationDeltaAsAngularVelocity, Vector, qangle, qangle)
	VSCRIPT_GLOBAL_FUNCTION(SplineQuaternions, Quaternion, quaternion, quaternion, float)
	VSCRIPT_GLOBAL_FUNCTION(SplineVectors, Vector, vector, vector, float)
	VSCRIPT_GLOBAL_FUNCTION(VectorToAngles, QAngle, vector)
	VSCRIPT_GLOBAL_FUNCTION(RandomFloat, float, float, float)
	VSCRIPT_GLOBAL_FUNCTION(RandomInt, int, int, int)

};
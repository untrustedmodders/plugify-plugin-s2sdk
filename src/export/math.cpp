#include <core/sdk/entity/math.h>
#include <core/sdk/helpers.hpp>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

/**
 * @brief Returns angular difference in degrees
 * @param angle1 First angle in degrees
 * @param angle2 Second angle in degrees
 * @return Angular difference in degrees
 */
extern "C" PLUGIN_API float AnglesDiff(float angle1, float angle2) {
	return Math{}.AngleDiff(angle1, angle2);
}

/**
 * @brief Converts QAngle to directional Vector
 * @param angles The QAngle to convert
 * @return Directional vector
 */
extern "C" PLUGIN_API Vector AnglesToVector(const QAngle& angles) {
	return Math{}.AnglesToVector(angles);
}

/**
 * @brief Converts axis-angle representation to quaternion
 * @param axis Rotation axis (should be normalized)
 * @param angle Rotation angle in radians
 * @return Resulting quaternion
 */
extern "C" PLUGIN_API Quaternion AxisAngleToQuaternion(const Vector& axis, float angle) {
	return Math{}.AxisAngleToQuaternion(axis, angle);
}

/**
 * @brief Computes closest point on an entity's oriented bounding box (OBB)
 * @param entityHandle Handle of the entity
 * @param position Position to find closest point from
 * @return Closest point on the entity's OBB, or vec3_origin if entity is invalid
 */
extern "C" PLUGIN_API Vector CalcClosestPointOnEntityOBB(int entityHandle, const Vector& position) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return vec3_origin;
	return Math{}.CalcClosestPointOnEntityOBB(entity->GetScriptInstance(), position);
}

/**
 * @brief Computes distance between two entities' oriented bounding boxes (OBBs)
 * @param entityHandle1 Handle of the first entity
 * @param entityHandle2 Handle of the second entity
 * @return Distance between OBBs, or -1.0f if either entity is invalid
 */
extern "C" PLUGIN_API float CalcDistanceBetweenEntityOBB(int entityHandle1, int entityHandle2) {
	auto* ent1 = helpers::GetEntity(entityHandle1);
	auto* ent2 = helpers::GetEntity(entityHandle2);
	if (!ent1 || !ent2) return -1.0f;
    return Math{}.CalcDistanceBetweenEntityOBB(ent1->GetScriptInstance(), ent2->GetScriptInstance());
}

/**
 * @brief Computes shortest 2D distance from a point to a line segment
 * @param p The point
 * @param vLineA First endpoint of the line segment
 * @param vLineB Second endpoint of the line segment
 * @return Shortest 2D distance
 */
extern "C" PLUGIN_API float CalcDistanceToLineSegment2D(const Vector& p, const Vector& vLineA, const Vector& vLineB) {
	return Math{}.CalcDistanceToLineSegment2D(p, vLineA, vLineB);
}

/**
 * @brief Computes cross product of two vectors
 * @param v1 First vector
 * @param v2 Second vector
 * @return Cross product vector (v1 × v2)
 */
extern "C" PLUGIN_API Vector CrossVectors(const Vector& v1, const Vector& v2) {
	return Math{}.CrossVectors(v1, v2);
}

/**
 * @brief Smooth exponential decay function
 * @param decayTo Target value to decay towards
 * @param decayTime Time constant for decay
 * @param dt Delta time
 * @return Decay factor
 */
extern "C" PLUGIN_API float ExponentDecay(float decayTo, float decayTime, float dt) {
	return Math{}.ExponentialDecay(decayTo, decayTime, dt);
}

/**
 * @brief Linear interpolation between two vectors
 * @param start Starting vector
 * @param end Ending vector
 * @param factor Interpolation factor (0.0 to 1.0)
 * @return Interpolated vector
 */
extern "C" PLUGIN_API Vector LerpVectors(const Vector& start, const Vector& end, float factor) {
	return Math{}.LerpVectors(start, end, factor);
}

/**
 * @brief Quaternion spherical linear interpolation for angles
 * @param fromAngle Starting angle
 * @param toAngle Ending angle
 * @param time Interpolation time (0.0 to 1.0)
 * @return Interpolated angle
 */
extern "C" PLUGIN_API QAngle QSlerp(const QAngle& fromAngle, const QAngle& toAngle, float time) {
	return Math{}.QSlerp(fromAngle, toAngle, time);
}

/**
 * @brief Rotate one QAngle by another
 * @param a1 Base orientation
 * @param a2 Rotation to apply
 * @return Rotated orientation
 */
extern "C" PLUGIN_API QAngle RotateOrientation(const QAngle& a1, const QAngle& a2) {
	return Math{}.RotateOrientation(a1, a2);
}

/**
 * @brief Rotate a vector around a point by specified angle
 * @param rotationOrigin Origin point of rotation
 * @param rotationAngle Angle to rotate by
 * @param vectorToRotate Vector to be rotated
 * @return Rotated vector
 */
extern "C" PLUGIN_API Vector RotatePosition(const Vector& rotationOrigin, const QAngle& rotationAngle, const Vector& vectorToRotate) {
	return Math{}.RotatePosition(rotationOrigin, rotationAngle, vectorToRotate);
}

/**
 * @brief Rotates quaternion by axis-angle representation
 * @param q Quaternion to rotate
 * @param axis Rotation axis
 * @param angle Rotation angle in radians
 * @return Rotated quaternion
 */
extern "C" PLUGIN_API Quaternion RotateQuaternionByAxisAngle(const Quaternion& q, const Vector& axis, float angle) {
	return Math{}.RotateQuaternionByAxisAngle(q, axis, angle);
}

/**
 * @brief Finds angular delta between two QAngles
 * @param src Source angle
 * @param dest Destination angle
 * @return Delta angle from src to dest
 */
extern "C" PLUGIN_API QAngle RotationDelta(const QAngle& src, const QAngle& dest) {
	return Math{}.RotationDelta(src, dest);
}

/**
 * @brief Converts delta QAngle to angular velocity vector
 * @param a1 First angle
 * @param a2 Second angle
 * @return Angular velocity vector
 */
extern "C" PLUGIN_API Vector RotationDeltaAsAngularVelocity(const QAngle& a1, const QAngle& a2) {
	return Math{}.RotationDeltaAsAngularVelocity(a1, a2);
}

/**
 * @brief Interpolates between two quaternions using spline
 * @param q0 Starting quaternion
 * @param q1 Ending quaternion
 * @param t Interpolation parameter (0.0 to 1.0)
 * @return Interpolated quaternion
 */
extern "C" PLUGIN_API Quaternion SplineQuaternions(const Quaternion& q0, const Quaternion& q1, float t) {
	return Math{}.SplineQuaternions(q0, q1, t);
}

/**
 * @brief Interpolates between two vectors using spline
 * @param v0 Starting vector
 * @param v1 Ending vector
 * @param t Interpolation parameter (0.0 to 1.0)
 * @return Interpolated vector
 */
extern "C" PLUGIN_API Vector SplineVectors(const Vector& v0, const Vector& v1, float t) {
	return Math{}.SplineVectors(v0, v1, t);
}

/**
 * @brief Converts directional vector to QAngle (no roll)
 * @param input Direction vector
 * @return Angle representation with pitch and yaw (roll is 0)
 */
extern "C" PLUGIN_API QAngle VectorToAngles(const Vector& input) {
	return Math{}.VectorToAngles(input);
}

/**
 * @brief Returns random float between min and max
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random float in range [min, max]
 */
extern "C" PLUGIN_API float RandomFlt(float min, float max) {
	return Math{}.RandomFloat(min, max);
}

/**
 * @brief Returns random integer between min and max (inclusive)
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random integer in range [min, max]
 */
extern "C" PLUGIN_API int RandomInt(int min, int max) {
	return Math{}.RandomInt(min, max);
}

PLUGIFY_WARN_POP()

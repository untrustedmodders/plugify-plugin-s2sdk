#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/helpers.hpp>

#if 0

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

/**
 * @brief Draws an axis at a specified world-space origin.
 *
 * @param origin The origin of the axis in world space.
 * @param orientation The orientation as a quaternion.
 * @param scale The scale of the axis.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the axis.
 */
extern "C" PLUGIN_API void DebugAxis(const plg::vec3& origin, const plg::quat& orientation, float scale, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.Axis(*reinterpret_cast<const Vector*>(&origin), *reinterpret_cast<const Quaternion*>(&orientation), scale, zTest, duration);
}

/**
 * @brief Draws a world-space axis-aligned box.
 *
 * @param mins The minimum corner of the box in world space.
 * @param maxs The maximum corner of the box in world space.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the box.
 */
extern "C" PLUGIN_API void DebugBox(const plg::vec3& mins, const plg::vec3& maxs, int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.Box(*reinterpret_cast<const Vector*>(&mins), *reinterpret_cast<const Vector*>(&maxs), r, g, b, a, zTest, duration);
}

/**
 * @brief Draws an oriented box in world space.
 *
 * @param origin The box origin in world space.
 * @param mins The local-space minimum corner.
 * @param maxs The local-space maximum corner.
 * @param orientation The box orientation as a quaternion.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the box.
 */
extern "C" PLUGIN_API void DebugBoxAngles(const plg::vec3& origin, const plg::vec3& mins, const plg::vec3& maxs, const plg::quat& orientation,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.BoxAngles(*reinterpret_cast<const Vector*>(&origin),
		*reinterpret_cast<const Vector*>(&mins),
		*reinterpret_cast<const Vector*>(&maxs),
		*reinterpret_cast<const Quaternion*>(&orientation),
		r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a capsule in world space.
 *
 * @param origin The capsule origin in world space.
 * @param orientation The capsule orientation as a quaternion.
 * @param radius The capsule radius.
 * @param halfHeight Half the capsule height.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the capsule.
 */
extern "C" PLUGIN_API void DebugCapsule(const plg::vec3& origin, const plg::quat& orientation, float radius, float halfHeight,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.Capsule(*reinterpret_cast<const Vector*>(&origin),
		*reinterpret_cast<const Quaternion*>(&orientation),
		radius, halfHeight, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a circle in world space.
 *
 * @param center The center of the circle in world space.
 * @param orientation The orientation as a quaternion.
 * @param radius The circle radius.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the circle.
 */
extern "C" PLUGIN_API void DebugCircle(const plg::vec3& center, const plg::quat& orientation, float radius,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.Circle(*reinterpret_cast<const Vector*>(&center),
		*reinterpret_cast<const Quaternion*>(&orientation),
		radius, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a screen-oriented circle in world space.
 *
 * @param center The center of the circle in world space.
 * @param radius The radius of the circle.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the circle.
 */
extern "C" PLUGIN_API void DebugCircleScreenOriented(const plg::vec3& center, float radius,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.CircleScreenOriented(*reinterpret_cast<const Vector*>(&center), radius, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a wireframe cone in world space.
 *
 * @param origin The base position of the cone in world space.
 * @param direction The direction vector of the cone in world space.
 * @param radius The cone base radius.
 * @param height The cone height.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the cone.
 */
extern "C" PLUGIN_API void DebugCone(const plg::vec3& origin, const plg::vec3& direction, float radius, float height,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.Cone(*reinterpret_cast<const Vector*>(&origin),
		*reinterpret_cast<const Vector*>(&direction),
		radius, height, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a screen-aligned cross in world space.
 *
 * @param origin The center position of the cross in world space.
 * @param size The size of the cross.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the cross.
 */
extern "C" PLUGIN_API void DebugCross(const plg::vec3& origin, float size, int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.Cross(*reinterpret_cast<const Vector*>(&origin), size, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a world-aligned cross in world space.
 *
 * @param origin The center of the cross in world space.
 * @param size The size of the cross.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the cross.
 */
extern "C" PLUGIN_API void DebugCross3D(const plg::vec3& origin, float size, int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.Cross3D(*reinterpret_cast<const Vector*>(&origin), size, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws an oriented cross in world space.
 *
 * @param origin The center position of the cross.
 * @param orientation The orientation of the cross as a quaternion.
 * @param size The size of the cross.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the cross.
 */
extern "C" PLUGIN_API void DebugCross3DOriented(const plg::vec3& origin, const plg::quat& orientation, float size,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.Cross3DOriented(*reinterpret_cast<const Vector*>(&origin),
		*reinterpret_cast<const Quaternion*>(&orientation),
		size, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a dashed (tick-marked) line in world space.
 *
 * @param start The start point in world space.
 * @param end The end point in world space.
 * @param tickDist Distance between tick marks.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param nTickCount Number of tick marks.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the line.
 */
extern "C" PLUGIN_API void DebugDrawTickMarkedLine(const plg::vec3& start, const plg::vec3& end, float tickDist,
	int r, int g, int b, int a, int nTickCount, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.DrawTickMarkedLine(*reinterpret_cast<const Vector*>(&start),
		*reinterpret_cast<const Vector*>(&end),
		tickDist, r, g, b, a, nTickCount, zTest, duration);
}

/**
 * @brief Draws the attachments of an entity.
 *
 * @param entityHandle Entity handle.
 * @param size Size of attachment markers.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugEntityAttachments(int entityHandle, float size, float duration) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	CDebugOverlayScriptHelper{}.EntityAttachments(entity, size, duration);
}

/**
 * @brief Draws the local axis of an entity at its origin.
 *
 * @param entityHandle Entity handle.
 * @param scale Axis size.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugEntityAxis(int entityHandle, float scale, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.EntityAxis(entity, scale, zTest, duration);
}

/**
 * @brief Draws an entity’s world-space bounding box.
 *
 * @param entityHandle Entity handle.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugEntityBounds(int entityHandle, int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.EntityBounds(entity, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws the skeleton of a model for a given entity.
 *
 * @param entityHandle Entity handle.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugEntitySkeleton(int entityHandle, float duration) {
	CDebugOverlayScriptHelper{}.EntitySkeleton(entity, duration);
}

/**
 * @brief Draws text attached to an entity.
 *
 * @param entityHandle Entity handle.
 * @param textOffset Offset index for text position.
 * @param text Text to display.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugEntityText(int entityHandle, int textOffset, const char* text,
	int r, int g, int b, int a, float duration) {
	CDebugOverlayScriptHelper{}.EntityText(entity, textOffset, text, r, g, b, a, duration);
}

/**
 * @brief Draws a filled 2D rectangle in screen space.
 *
 * @param min The top-left corner in pixels.
 * @param max The bottom-right corner in pixels.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugFilledRect2D(const plg::vec2& min, const plg::vec2& max,
	int r, int g, int b, int a, float duration) {
	CDebugOverlayScriptHelper{}.FilledRect2D(*reinterpret_cast<const Vector2D*>(&min),
		*reinterpret_cast<const Vector2D*>(&max),
		r, g, b, a, duration);
}

/**
 * @brief Draws a horizontal arrow between two points in world space.
 *
 * @param start The start point in world space.
 * @param end The end point in world space.
 * @param width Arrow width.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugHorzArrow(const plg::vec3& start, const plg::vec3& end, float width,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.HorzArrow(*reinterpret_cast<const Vector*>(&start),
		*reinterpret_cast<const Vector*>(&end),
		width, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a 3D line between two world-space points.
 *
 * @param start The start point.
 * @param end The end point.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugLine(const plg::vec3& start, const plg::vec3& end,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.Line(*reinterpret_cast<const Vector*>(&start),
		*reinterpret_cast<const Vector*>(&end),
		r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a line between two points in screen space.
 *
 * @param start The start position in pixels.
 * @param end The end position in pixels.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugLine2D(const plg::vec2& start, const plg::vec2& end,
	int r, int g, int b, int a, float duration) {
	CDebugOverlayScriptHelper{}.Line2D(*reinterpret_cast<const Vector2D*>(&start),
		*reinterpret_cast<const Vector2D*>(&end),
		r, g, b, a, duration);
}

/**
 * @brief Pops the current debug overlay scope, ending a grouped overlay batch.
 */
extern "C" PLUGIN_API void DebugPopDebugOverlayScope() {
	CDebugOverlayScriptHelper{}.PopDebugOverlayScope();
}

/**
 * @brief Pushes a new debug overlay scope and clears all overlays with this identifier.
 *
 * @param id Overlay identifier token.
 */
extern "C" PLUGIN_API void DebugPushAndClearDebugOverlayScope(utlstringtoken id) {
	CDebugOverlayScriptHelper{}.PushAndClearDebugOverlayScope(id);
}

/**
 * @brief Pushes a new debug overlay scope without clearing.
 *
 * @param id Overlay identifier token.
 */
extern "C" PLUGIN_API void DebugPushDebugOverlayScope(utlstringtoken id) {
	CDebugOverlayScriptHelper{}.PushDebugOverlayScope(id);
}

/**
 * @brief Removes all debug overlays marked with a specific identifier.
 *
 * @param id Overlay identifier token.
 */
extern "C" PLUGIN_API void DebugRemoveAllInScope(utlstringtoken id) {
	CDebugOverlayScriptHelper{}.RemoveAllInScope(id);
}

/**
 * @brief Draws a solid cone in world space.
 *
 * @param origin Base position of the cone in world space.
 * @param direction Direction vector of the cone in world space.
 * @param radius Base radius of the cone.
 * @param height Cone height.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the cone.
 */
extern "C" PLUGIN_API void DebugSolidCone(const plg::vec3& origin, const plg::vec3& direction, float radius, float height,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.SolidCone(*reinterpret_cast<const Vector*>(&origin),
		*reinterpret_cast<const Vector*>(&direction),
		radius, height, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a wireframe sphere in world space.
 *
 * @param center Center of the sphere in world space.
 * @param radius Sphere radius.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the sphere.
 */
extern "C" PLUGIN_API void DebugSphere(const plg::vec3& center, float radius,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.Sphere(*reinterpret_cast<const Vector*>(&center),
		radius, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a swept box between two points in world space.
 *
 * @param start Start point in world space.
 * @param end End point in world space.
 * @param mins Local-space minimum bounds.
 * @param maxs Local-space maximum bounds.
 * @param orientation Orientation of the box.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param duration Duration (in seconds) to display the swept box.
 */
extern "C" PLUGIN_API void DebugSweptBox(const plg::vec3& start, const plg::vec3& end,
	const plg::vec3& mins, const plg::vec3& maxs, const plg::quat& orientation,
	int r, int g, int b, int a, float duration) {
	CDebugOverlayScriptHelper{}.SweptBox(*reinterpret_cast<const Vector*>(&start),
		*reinterpret_cast<const Vector*>(&end),
		*reinterpret_cast<const Vector*>(&mins),
		*reinterpret_cast<const Vector*>(&maxs),
		*reinterpret_cast<const Quaternion*>(&orientation),
		r, g, b, a, duration);
}

/**
 * @brief Draws 2D text in world space.
 *
 * @param origin Origin of the text in world space.
 * @param lineOffset Line offset index.
 * @param text Text to display.
 * @param scale Text scale factor.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugText(const plg::vec3& origin, int lineOffset, const char* text,
	float scale, int r, int g, int b, int a, float duration) {
	CDebugOverlayScriptHelper{}.Text(*reinterpret_cast<const Vector*>(&origin),
		lineOffset, text, scale, r, g, b, a, duration);
}

/**
 * @brief Draws a textured rectangle on screen.
 *
 * @param textureName Name of the texture to draw.
 * @param min Top-left screen-space coordinate in pixels.
 * @param max Bottom-right screen-space coordinate in pixels.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param uvMin Minimum UV coordinate.
 * @param uvMax Maximum UV coordinate.
 * @param duration Duration (in seconds) to display the texture.
 */
extern "C" PLUGIN_API void DebugTexture(const char* textureName, const plg::vec2& min, const plg::vec2& max,
	int r, int g, int b, int a, const plg::vec2& uvMin, const plg::vec2& uvMax, float duration) {
	CDebugOverlayScriptHelper{}.Texture(textureName,
		*reinterpret_cast<const Vector2D*>(&min),
		*reinterpret_cast<const Vector2D*>(&max),
		r, g, b, a,
		*reinterpret_cast<const Vector2D*>(&uvMin),
		*reinterpret_cast<const Vector2D*>(&uvMax),
		duration);
}

/**
 * @brief Draws a filled triangle in world space.
 *
 * @param v1 First vertex.
 * @param v2 Second vertex.
 * @param v3 Third vertex.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the triangle.
 */
extern "C" PLUGIN_API void DebugTriangle(const plg::vec3& v1, const plg::vec3& v2, const plg::vec3& v3,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.Triangle(*reinterpret_cast<const Vector*>(&v1),
		*reinterpret_cast<const Vector*>(&v2),
		*reinterpret_cast<const Vector*>(&v3),
		r, g, b, a, zTest, duration);
}

/**
 * @brief Draws 3D text in world space with orientation.
 *
 * @param origin Origin position in world space.
 * @param orientation Orientation of the text.
 * @param text Text to display.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the text.
 */
extern "C" PLUGIN_API void DebugVectorText3D(const plg::vec3& origin, const plg::quat& orientation, const char* text,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.VectorText3D(*reinterpret_cast<const Vector*>(&origin),
		*reinterpret_cast<const Quaternion*>(&orientation),
		text, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a vertical arrow between two points in world space.
 *
 * @param start Start point in world space.
 * @param end End point in world space.
 * @param width Arrow width.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugVertArrow(const plg::vec3& start, const plg::vec3& end, float width,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.VertArrow(*reinterpret_cast<const Vector*>(&start),
		*reinterpret_cast<const Vector*>(&end),
		width, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a yaw-based directional arrow in world space.
 *
 * @param origin Origin position in world space.
 * @param yaw Yaw angle in degrees.
 * @param length Length of the arrow.
 * @param width Arrow width.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the arrow.
 */
extern "C" PLUGIN_API void DebugYawArrow(const plg::vec3& origin, float yaw, float length, float width,
	int r, int g, int b, int a, bool zTest, float duration) {
	CDebugOverlayScriptHelper{}.YawArrow(*reinterpret_cast<const Vector*>(&origin),
		yaw, length, width, r, g, b, a, zTest, duration);
}

PLUGIFY_WARN_POP()

#endif
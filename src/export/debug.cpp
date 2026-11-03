#include <core/sdk/entity/debug.h>
#include <core/sdk/helpers.hpp>

PLUGIFY_WARN_PUSH()

PLUGIFY_LINKAGE()

/**
 * @brief Triggers a breakpoint in the debugger.
 */
extern "C" PLUGIN_API void DebugBreak() {
	Debug{}.DebugBreak();
}

/**
 * @brief Draws a debug overlay box.
 *
 * @param center Center of the box in world space.
 * @param mins Minimum bounds relative to the center.
 * @param maxs Maximum bounds relative to the center.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha (transparency) value.
 * @param duration Duration (in seconds) to display the box.
 */
extern "C" PLUGIN_API void DebugDrawBox(const plg::vec3& center, const plg::vec3& mins, const plg::vec3& maxs,
			 int r, int g, int b, int a, float duration) {
	Debug{}.DebugDrawBox(std::bit_cast<Vector>(center),
						 std::bit_cast<Vector>(mins),
						 std::bit_cast<Vector>(maxs),
						 r, g, b, a, duration);
}

/**
 * @brief Draws a debug box oriented in the direction of a forward vector.
 *
 * @param center Center of the box.
 * @param mins Minimum bounds.
 * @param maxs Maximum bounds.
 * @param forward Forward direction vector.
 * @param color RGB color vector.
 * @param alpha Alpha transparency.
 * @param duration Duration (in seconds) to display the box.
 */
extern "C" PLUGIN_API void DebugDrawBoxDirection(const plg::vec3& center, const plg::vec3& mins, const plg::vec3& maxs,
					  const plg::vec3& forward, const plg::vec3& color, float alpha, float duration) {
	Debug{}.DebugDrawBoxDirection(std::bit_cast<Vector>(center),
								  std::bit_cast<Vector>(mins),
								  std::bit_cast<Vector>(maxs),
								  std::bit_cast<Vector>(forward),
								  std::bit_cast<Vector>(color),
								  alpha, duration);
}

/**
 * @brief Draws a debug circle.
 *
 * @param center Center of the circle.
 * @param color RGB color vector.
 * @param alpha Alpha transparency.
 * @param radius Circle radius.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the circle.
 */
extern "C" PLUGIN_API void DebugDrawCircle(const plg::vec3& center, const plg::vec3& color,
				float alpha, float radius, bool zTest, float duration) {
	Debug{}.DebugDrawCircle(std::bit_cast<Vector>(center),
							std::bit_cast<Vector>(color),
							alpha, radius, zTest, duration);
}

/**
 * @brief Clears all debug overlays.
 */
extern "C" PLUGIN_API void DebugDrawClear() {
	Debug{}.DebugDrawClear();
}

/**
 * @brief Draws a debug overlay line.
 *
 * @param origin Start point of the line.
 * @param target End point of the line.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the line.
 */
extern "C" PLUGIN_API void DebugDrawLine(const plg::vec3& origin, const plg::vec3& target,
			  int r, int g, int b, bool zTest, float duration) {
	Debug{}.DebugDrawLine(std::bit_cast<Vector>(origin),
						  std::bit_cast<Vector>(target),
						  r, g, b, zTest, duration);
}

/**
 * @brief Draws a debug line using a color vector.
 *
 * @param start Start point of the line.
 * @param end End point of the line.
 * @param color RGB color vector.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the line.
 */
extern "C" PLUGIN_API void DebugDrawLine_vCol(const plg::vec3& start, const plg::vec3& end,
				   const plg::vec3& color, bool zTest, float duration) {
	Debug{}.DebugDrawLine_vCol(std::bit_cast<Vector>(start),
							   std::bit_cast<Vector>(end),
							   std::bit_cast<Vector>(color),
							   zTest, duration);
}

/**
 * @brief Draws text at a specified screen position with line offset.
 *
 * @param x X coordinate in screen space.
 * @param y Y coordinate in screen space.
 * @param lineOffset Line offset value.
 * @param text The text string to display.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha transparency value.
 * @param duration Duration (in seconds) to display the text.
 */
extern "C" PLUGIN_API void DebugDrawScreenTextLine(float x, float y, int lineOffset, const plg::string& text,
						int r, int g, int b, int a, float duration) {
	Debug{}.DebugDrawScreenTextLine(x, y, lineOffset, text.c_str(), r, g, b, a, duration);
}

/**
 * @brief Draws a debug sphere.
 *
 * @param center Center of the sphere.
 * @param color RGB color vector.
 * @param alpha Alpha transparency.
 * @param radius Radius of the sphere.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the sphere.
 */
extern "C" PLUGIN_API void DebugDrawSphere(const plg::vec3& center, const plg::vec3& color,
				float alpha, float radius, bool zTest, float duration) {
	Debug{}.DebugDrawSphere(std::bit_cast<Vector>(center),
							std::bit_cast<Vector>(color),
							alpha, radius, zTest, duration);
}

/**
 * @brief Draws text in 3D space.
 *
 * @param origin World-space position to draw the text at.
 * @param text The text string to display.
 * @param viewCheck If true, only draws when visible to camera.
 * @param duration Duration (in seconds) to display the text.
 */
extern "C" PLUGIN_API void DebugDrawText(const plg::vec3& origin, const plg::string& text,
			  bool viewCheck, float duration) {
	Debug{}.DebugDrawText(std::bit_cast<Vector>(origin), text.c_str(), viewCheck, duration);
}

/**
 * @brief Draws styled debug text on screen.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param lineOffset Line offset value.
 * @param text Text string.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @param a Alpha transparency.
 * @param duration Duration (in seconds) to display the text.
 * @param font Font name.
 * @param size Font size.
 * @param bold Whether text should be bold.
 */
extern "C" PLUGIN_API void DebugScreenTextPretty(float x, float y, int lineOffset, const plg::string& text,
					  int r, int g, int b, int a, float duration, const plg::string& font, int size, bool bold) {
	Debug{}.DebugScreenTextPretty(x, y, lineOffset, text.c_str(), r, g, b, a, duration, font.c_str(), size, bold);
}

/**
 * @brief Performs an assertion and logs a message if the assertion fails.
 *
 * @param assertion Boolean value to test.
 * @param message Message to display if the assertion fails.
 */
extern "C" PLUGIN_API void DebugScriptAssert(bool assertion, const plg::string& message) {
	Debug{}.DoScriptAssert(assertion, message.c_str());
}

namespace {
	CDebugOverlayScriptHelper* GetDebugOverlay() {
		auto* overlay = static_cast<CDebugOverlayScriptHelper*>(vscript::GetInstance(CDebugOverlayScriptHelper::m_className));
		if (!overlay) {
			plg::print(LS_WARNING, "Debug overlay helper is not available yet\n");
		}
		return overlay;
	}

	// The game reads quaternions with aligned SSE loads
	struct AlignedQuaternion {
		alignas(16) Quaternion value;
	};
}// namespace

/**
 * @brief Draws an axis at a specified world-space origin.
 *
 * @param origin The origin of the axis in world space.
 * @param orientation The orientation as a quaternion.
 * @param scale The scale of the axis.
 * @param zTest Whether to perform depth testing.
 * @param duration Duration (in seconds) to display the axis.
 */
extern "C" PLUGIN_API void DebugAxis(const plg::vec3& origin, const plg::vec4& orientation, float scale, bool zTest, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Axis(std::bit_cast<Vector>(origin), AlignedQuaternion{std::bit_cast<Quaternion>(orientation)}.value, scale, zTest, duration);
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Box(std::bit_cast<Vector>(mins), std::bit_cast<Vector>(maxs), r, g, b, a, zTest, duration);
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
extern "C" PLUGIN_API void DebugBoxAngles(const plg::vec3& origin, const plg::vec3& mins, const plg::vec3& maxs, const plg::vec4& orientation,
	int r, int g, int b, int a, bool zTest, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->BoxAngles(std::bit_cast<Vector>(origin),
		std::bit_cast<Vector>(mins),
		std::bit_cast<Vector>(maxs),
		AlignedQuaternion{std::bit_cast<Quaternion>(orientation)}.value,
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
extern "C" PLUGIN_API void DebugCapsule(const plg::vec3& origin, const plg::vec4& orientation, float radius, float halfHeight,
	int r, int g, int b, int a, bool zTest, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Capsule(std::bit_cast<Vector>(origin),
		AlignedQuaternion{std::bit_cast<Quaternion>(orientation)}.value,
		radius, halfHeight, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a circle in world space.
 *
 * Not displayed on clients of a dedicated server: the server does not send this primitive.
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
extern "C" PLUGIN_API void DebugCircle(const plg::vec3& center, const plg::vec4& orientation, float radius,
	int r, int g, int b, int a, bool zTest, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Circle(std::bit_cast<Vector>(center),
		AlignedQuaternion{std::bit_cast<Quaternion>(orientation)}.value,
		radius, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a screen-oriented circle in world space.
 *
 * Not displayed on clients of a dedicated server: the server does not send this primitive.
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->CircleScreenOriented(std::bit_cast<Vector>(center), radius, r, g, b, a, zTest, duration);
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Cone(std::bit_cast<Vector>(origin),
		std::bit_cast<Vector>(direction),
		radius, height, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a screen-aligned cross in world space.
 *
 * Not displayed on clients of a dedicated server: the server does not send this primitive.
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Cross(std::bit_cast<Vector>(origin), size, r, g, b, a, zTest, duration);
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Cross3D(std::bit_cast<Vector>(origin), size, r, g, b, a, zTest, duration);
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
extern "C" PLUGIN_API void DebugCross3DOriented(const plg::vec3& origin, const plg::vec4& orientation, float size,
	int r, int g, int b, int a, bool zTest, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Cross3DOriented(std::bit_cast<Vector>(origin),
		AlignedQuaternion{std::bit_cast<Quaternion>(orientation)}.value,
		size, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a dashed (tick-marked) line in world space.
 *
 * Not displayed on clients of a dedicated server: the server does not send this primitive.
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
	int r, int g, int b, int a, int tickCount, bool zTest, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->DrawTickMarkedLine(std::bit_cast<Vector>(start),
		std::bit_cast<Vector>(end),
		tickDist, r, g, b, a, tickCount, zTest, duration);
}

/**
 * @brief Draws the attachments of an entity.
 *
 * @param entityHandle Entity handle.
 * @param size Size of attachment markers.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugEntityAttachments(int entityHandle, float size, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	if (!helpers::GetEntity(entityHandle)) return;
	overlay->EntityAttachments(entityHandle, size, duration);
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->EntityAxis(entityHandle, scale, zTest, duration);
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->EntityBounds(entityHandle, r, g, b, a, zTest, duration);
}

/**
 * @brief Draws the skeleton of a model for a given entity.
 *
 * Not displayed on clients of a dedicated server: the server does not send this primitive.
 *
 * @param entityHandle Entity handle.
 * @param duration Duration (in seconds) to display.
 */
extern "C" PLUGIN_API void DebugEntitySkeleton(int entityHandle, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->EntitySkeleton(entityHandle, duration);
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
extern "C" PLUGIN_API void DebugEntityText(int entityHandle, int textOffset, const plg::string& text,
	int r, int g, int b, int a, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->EntityText(entityHandle, textOffset, text.c_str(), r, g, b, a, duration);
}

/**
 * @brief Draws a filled 2D rectangle in screen space.
 *
 * Not displayed on clients of a dedicated server: the server does not send this primitive.
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->FilledRect2D(std::bit_cast<Vector2D>(min),
		std::bit_cast<Vector2D>(max),
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->HorzArrow(std::bit_cast<Vector>(start),
		std::bit_cast<Vector>(end),
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Line(std::bit_cast<Vector>(start),
		std::bit_cast<Vector>(end),
		r, g, b, a, zTest, duration);
}

/**
 * @brief Draws a line between two points in screen space.
 *
 * Not displayed on clients of a dedicated server: the server does not send this primitive.
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Line2D(std::bit_cast<Vector2D>(start),
		std::bit_cast<Vector2D>(end),
		r, g, b, a, duration);
}

/**
 * @brief Pops the current debug overlay scope, ending a grouped overlay batch.
 */
extern "C" PLUGIN_API void DebugPopDebugOverlayScope() {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->PopDebugOverlayScope();
}

/**
 * @brief Pushes a new debug overlay scope and clears all overlays with this identifier.
 *
 * @param name Overlay scope name.
 */
extern "C" PLUGIN_API void DebugPushAndClearDebugOverlayScope(const plg::string& name) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->PushAndClearDebugOverlayScope(CUtlStringToken(std::string_view(name)));
}

/**
 * @brief Pushes a new debug overlay scope without clearing.
 *
 * @param name Overlay scope name.
 */
extern "C" PLUGIN_API void DebugPushDebugOverlayScope(const plg::string& name) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->PushDebugOverlayScope(CUtlStringToken(std::string_view(name)));
}

/**
 * @brief Removes all debug overlays marked with a specific identifier.
 *
 * @param name Overlay scope name.
 */
extern "C" PLUGIN_API void DebugRemoveAllInScope(const plg::string& name) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->RemoveAllInScope(CUtlStringToken(std::string_view(name)));
}

/**
 * @brief Draws a solid cone in world space.
 *
 * Not displayed on clients of a dedicated server: the server does not send this primitive.
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->SolidCone(std::bit_cast<Vector>(origin),
		std::bit_cast<Vector>(direction),
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Sphere(std::bit_cast<Vector>(center),
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
	const plg::vec3& mins, const plg::vec3& maxs, const plg::vec4& orientation,
	int r, int g, int b, int a, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->SweptBox(std::bit_cast<Vector>(start),
		std::bit_cast<Vector>(end),
		std::bit_cast<Vector>(mins),
		std::bit_cast<Vector>(maxs),
		AlignedQuaternion{std::bit_cast<Quaternion>(orientation)}.value,
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
extern "C" PLUGIN_API void DebugText(const plg::vec3& origin, int lineOffset, const plg::string& text,
	float scale, int r, int g, int b, int a, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Text(std::bit_cast<Vector>(origin),
		lineOffset, text.c_str(), scale, r, g, b, a, duration);
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
extern "C" PLUGIN_API void DebugTexture(const plg::string& textureName, const plg::vec2& min, const plg::vec2& max,
	int r, int g, int b, int a, const plg::vec2& uvMin, const plg::vec2& uvMax, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Texture(textureName.c_str(),
		std::bit_cast<Vector2D>(min),
		std::bit_cast<Vector2D>(max),
		r, g, b, a,
		std::bit_cast<Vector2D>(uvMin),
		std::bit_cast<Vector2D>(uvMax),
		duration);
}

/**
 * @brief Draws a filled triangle in world space.
 *
 * Not displayed on clients of a dedicated server: the server does not send this primitive.
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->Triangle(std::bit_cast<Vector>(v1),
		std::bit_cast<Vector>(v2),
		std::bit_cast<Vector>(v3),
		r, g, b, a, zTest, duration);
}

/**
 * @brief Draws 3D text in world space with orientation.
 *
 * Not displayed on clients of a dedicated server: the server does not send this primitive.
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
extern "C" PLUGIN_API void DebugVectorText3D(const plg::vec3& origin, const plg::vec4& orientation, const plg::string& text,
	int r, int g, int b, int a, bool zTest, float duration) {
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->VectorText3D(std::bit_cast<Vector>(origin),
		AlignedQuaternion{std::bit_cast<Quaternion>(orientation)}.value,
		text.c_str(), r, g, b, a, zTest, duration);
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->VertArrow(std::bit_cast<Vector>(start),
		std::bit_cast<Vector>(end),
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
	auto* overlay = GetDebugOverlay();
	if (!overlay) return;
	overlay->YawArrow(std::bit_cast<Vector>(origin),
		yaw, length, width, r, g, b, a, zTest, duration);
}

PLUGIFY_WARN_POP()

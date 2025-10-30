#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/entity/trace.h>
#include <core/sdk/utils.hpp>
#include <core/sdk/helpers.hpp>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

/**
 * @brief Performs a collideable trace using the VScript-compatible table call, exposing it through C++ exports.
 *
 * @param start Trace start position (world space)
 * @param end Trace end position (world space)
 * @param entityHandle Entity handle of the collideable
 * @param outPos Output: position of impact
 * @param outFraction Output: fraction of trace completed
 * @param outHit Output: whether a hit occurred
 * @param outStartSolid Output: whether trace started inside solid
 * @param outNormal Output: surface normal at impact
 * @return True if trace hit something, false otherwise
 */
extern "C" PLUGIN_API bool TraceCollideable(
	const Vector& start,
	const Vector& end,
	int entityHandle,
	Vector& outPos,
	float& outFraction,
	bool& outHit,
	bool& outStartSolid,
	Vector& outNormal
) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return false;
	return Trace{}.VScript_TraceCollideable(start, end, entity, nullptr, nullptr, outPos, outFraction, outHit, outStartSolid, outNormal);
}

/**
 * @brief Performs a collideable trace using the VScript-compatible table call, exposing it through C++ exports.
 *
 * @param start Trace start position (world space)
 * @param end Trace end position (world space)
 * @param entityHandle Entity handle of the collideable
 * @param mins Bounding box minimums
 * @param maxs Bounding box maximums
 * @param outPos Output: position of impact
 * @param outFraction Output: fraction of trace completed
 * @param outHit Output: whether a hit occurred
 * @param outStartSolid Output: whether trace started inside solid
 * @param outNormal Output: surface normal at impact
 * @return True if trace hit something, false otherwise
 */
extern "C" PLUGIN_API bool TraceCollideable2(
	const Vector& start,
	const Vector& end,
	int entityHandle,
	const Vector* mins,
	const Vector* maxs,
	Vector& outPos,
	float& outFraction,
	bool& outHit,
	bool& outStartSolid,
	Vector& outNormal
) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return false;
	return Trace{}.VScript_TraceCollideable(start, end, entity, mins, maxs, outPos, outFraction, outHit, outStartSolid, outNormal);
}

/**
 * @brief Performs a hull trace with specified dimensions and mask.
 *
 * @param start Trace start position
 * @param end Trace end position
 * @param min Local bounding box minimums
 * @param max Local bounding box maximums
 * @param mask Trace mask
 * @param ignoreHandle Entity handle to ignore during trace
 * @param outPos Output: position of impact
 * @param outFraction Output: fraction of trace completed
 * @param outHit Output: whether a hit occurred
 * @param outEntHit Output: handle of entity hit
 * @param outStartSolid Output: whether trace started inside solid
 * @return True if trace hit something, false otherwise
 */
extern "C" PLUGIN_API bool TraceHull(
	const Vector& start,
	const Vector& end,
	const Vector& min,
	const Vector& max,
	int mask,
	int ignoreHandle,
	Vector& outPos,
	float& outFraction,
	bool& outHit,
	int& outEntHit,
	bool& outStartSolid
) {
	auto* ignoreEntity = helpers::GetEntity(ignoreHandle);
	if (!ignoreEntity) return false;
	CEntityInstance* hitEntity = nullptr;
	bool result = Trace{}.VScript_TraceHull(start, end, min, max, mask, ignoreEntity, outPos, outFraction, outHit, hitEntity, outStartSolid);
	outEntHit = hitEntity ? hitEntity->GetRefEHandle().ToInt(): INVALID_EHANDLE_INDEX;
	return result;
}

/**
 * @brief Performs a line trace between two points.
 *
 * @param startPos Trace start position
 * @param endPos Trace end position
 * @param mask Trace mask
 * @param ignoreHandle Entity handle to ignore during trace
 * @param outPos Output: position of impact
 * @param outFraction Output: fraction of trace completed
 * @param outHit Output: whether a hit occurred
 * @param outEntHit Output: handle of entity hit
 * @param outStartSolid Output: whether trace started inside solid
 * @return True if trace hit something, false otherwise
 */
extern "C" PLUGIN_API bool TraceLine(
	const Vector& startPos,
	const Vector& endPos,
	int mask,
	int ignoreHandle,
	Vector& outPos,
	float& outFraction,
	bool& outHit,
	int& outEntHit,
	bool& outStartSolid
) {
	auto* ignoreEntity = helpers::GetEntity(ignoreHandle);
	if (!ignoreEntity) return false;
	CEntityInstance* hitEntity = nullptr;
	bool result = Trace{}.VScript_TraceLine(startPos, endPos, mask, ignoreEntity, outPos, outFraction, outHit, hitEntity, outStartSolid);
	outEntHit = hitEntity ? hitEntity->GetRefEHandle().ToInt(): INVALID_EHANDLE_INDEX;
	return result;
}

PLUGIFY_WARN_POP()

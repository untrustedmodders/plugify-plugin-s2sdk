#include <core/output_manager.hpp>
#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/entity/cbasemodelentity.h>
#include <core/sdk/utils.hpp>
#include <core/sdk/helpers.hpp>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

/**
 * @brief Applies an impulse to an entity at a specific world position.
 *
 * @param entityHandle The handle of the entity.
 * @param position The world position where the impulse will be applied.
 * @param impulse The impulse vector to apply.
 */
extern "C" PLUGIN_API void AddBodyImpulseAtPosition(int entityHandle, const plg::vec3& position, const plg::vec3& impulse) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->m_CBodyComponent->AddImpulseAtPosition(std::bit_cast<Vector>(position), std::bit_cast<Vector>(impulse));
}

/**
 * @brief Adds linear and angular velocity to the entity's physics object.
 *
 * @param entityHandle The handle of the entity.
 * @param linearVelocity The linear velocity vector to add.
 * @param angularVelocity The angular velocity vector to add.
 */
extern "C" PLUGIN_API void AddBodyVelocity(int entityHandle, const plg::vec3& linearVelocity, const plg::vec3& angularVelocity) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->m_CBodyComponent->AddVelocity(std::bit_cast<Vector>(linearVelocity), std::bit_cast<Vector>(angularVelocity));
}

/**
 * @brief Detaches the entity from its parent.
 *
 * @param entityHandle The handle of the entity.
 */
extern "C" PLUGIN_API void DetachBodyFromParent(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->m_CBodyComponent->DetachFromParent();
}

/**
 * @brief Retrieves the currently active sequence of the entity.
 *
 * @param entityHandle The handle of the entity.
 * @return The sequence ID of the active sequence, or -1 if invalid.
 */
extern "C" PLUGIN_API int GetBodySequence(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return -1;
	return entity->m_CBodyComponent->GetSequence();
}

/**
 * @brief Checks whether the entity is attached to a parent.
 *
 * @param entityHandle The handle of the entity.
 * @return True if attached to a parent, false otherwise.
 */
extern "C" PLUGIN_API bool IsBodyAttachedToParent(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return false;
	return entity->m_CBodyComponent->IsAttachedToParent();
}

/**
 * @brief Looks up a sequence ID by its name.
 *
 * @param entityHandle The handle of the entity.
 * @param name The name of the sequence.
 * @return The sequence ID, or -1 if not found.
 */
extern "C" PLUGIN_API int LookupBodySequence(int entityHandle, const plg::string& name) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return -1;
	return entity->m_CBodyComponent->LookupSequence(name.c_str());
}

/**
 * @brief Retrieves the duration of a specified sequence.
 *
 * @param entityHandle The handle of the entity.
 * @param sequenceName The name of the sequence.
 * @return The duration of the sequence in seconds, or 0 if invalid.
 */
extern "C" PLUGIN_API float SetBodySequenceDuration(int entityHandle, const plg::string& sequenceName) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return 0.0f;
	return entity->m_CBodyComponent->SequenceDuration(sequenceName.c_str());
}

/**
 * @brief Sets the angular velocity of the entity.
 *
 * @param entityHandle The handle of the entity.
 * @param angVelocity The new angular velocity vector.
 */
extern "C" PLUGIN_API void SetBodyAngularVelocity(int entityHandle, const plg::vec3& angVelocity) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->m_CBodyComponent->SetAngularVelocity(std::bit_cast<Vector>(angVelocity));
}

/**
 * @brief Sets the material group of the entity.
 *
 * @param entityHandle The handle of the entity.
 * @param materialGroup The material group token to assign.
 */
extern "C" PLUGIN_API void SetBodyMaterialGroup(int entityHandle, const plg::string& materialGroup) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->m_CBodyComponent->SetMaterialGroup(CUtlStringToken(materialGroup));
}

/**
 * @brief Sets the linear velocity of the entity.
 *
 * @param entityHandle The handle of the entity.
 * @param velocity The new velocity vector.
 */
extern "C" PLUGIN_API void SetBodyVelocity(int entityHandle, const plg::vec3& velocity) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->m_CBodyComponent->SetVelocity(std::bit_cast<Vector>(velocity));
}

PLUGIFY_WARN_POP()

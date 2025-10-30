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
 * @brief Retrieves the attachment angles of an entity.
 *
 * This function returns the pitch, yaw, and roll angles of the specified attachment.
 * If the entity is invalid, it returns a zero vector.
 *
 * @param entityHandle The handle of the entity whose attachment angles are to be retrieved.
 * @param attachmentIndex The attachment index.
 * @return A vector representing the attachment angles (pitch, yaw, roll).
 */
extern "C" PLUGIN_API plg::vec3 GetEntityAttachmentAngles(int entityHandle, int attachmentIndex) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return {};
	Vector angles = entity->GetAttachmentAngles(attachmentIndex);
	return *reinterpret_cast<const plg::vec3*>(&angles);
}

/**
 * @brief Retrieves the forward vector of an entity's attachment.
 *
 * This function returns the forward direction vector of the specified attachment.
 * If the entity is invalid, it returns a zero vector.
 *
 * @param entityHandle The handle of the entity whose attachment forward vector is to be retrieved.
 * @param attachmentIndex The attachment index.
 * @return A vector representing the forward direction of the attachment.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityAttachmentForward(int entityHandle, int attachmentIndex) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return {};
	Vector forward = entity->GetAttachmentForward(attachmentIndex);
	return *reinterpret_cast<const plg::vec3*>(&forward);
}

/**
 * @brief Retrieves the origin vector of an entity's attachment.
 *
 * This function returns the origin position of the specified attachment.
 * If the entity is invalid, it returns a zero vector.
 *
 * @param entityHandle The handle of the entity whose attachment origin is to be retrieved.
 * @param attachmentIndex The attachment index.
 * @return A vector representing the origin of the attachment.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityAttachmentOrigin(int entityHandle, int attachmentIndex) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return {};
	Vector origin = entity->GetAttachmentOrigin(attachmentIndex);
	return *reinterpret_cast<const plg::vec3*>(&origin);
}

/**
 * @brief Retrieves the material group hash of an entity.
 *
 * This function returns the hash of the entity’s current material group.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity.
 * @return The material group hash.
 */
extern "C" PLUGIN_API uint32_t GetEntityMaterialGroupHash(int entityHandle) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return {};
	return entity->GetMaterialGroupHash();
}

/**
 * @brief Retrieves the material group mask of an entity.
 *
 * This function returns the mesh group mask for the entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity.
 * @return The mesh group mask.
 */
extern "C" PLUGIN_API uint64 GetEntityMaterialGroupMask(int entityHandle) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return {};
	return entity->GetMaterialGroupMask();
}

/**
 * @brief Retrieves the model scale of an entity.
 *
 * This function returns the scale factor applied to the entity's model.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity.
 * @return The model scale factor.
 */
extern "C" PLUGIN_API float GetEntityModelScale(int entityHandle) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return {};
	return entity->GetModelScale();
}

/**
 * @brief Retrieves the render alpha of an entity.
 *
 * This function returns the alpha (transparency) value used for rendering the entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity.
 * @return The alpha modulation value.
 */
extern "C" PLUGIN_API int GetEntityRenderAlpha(int entityHandle) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return {};
	return entity->GetRenderAlpha();
}

/**
 * @brief Retrieves the render color of an entity.
 *
 * This function returns the current render color of the entity as an RGB vector.
 * If the entity is invalid, it returns a zero vector.
 *
 * @param entityHandle The handle of the entity.
 * @return A vector representing the render color (R, G, B).
 */
extern "C" PLUGIN_API plg::vec3 GetEntityRenderColor2(int entityHandle) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return {};
	Vector color = entity->GetRenderColor();
	return *reinterpret_cast<const plg::vec3*>(&color);
}

/**
 * @brief Retrieves an attachment index by name.
 *
 * This function returns the index of the specified attachment by its name.
 * If the entity is invalid or the attachment is not found, it returns -1.
 *
 * @param entityHandle The handle of the entity.
 * @param attachmentName The name of the attachment.
 * @return The attachment index, or -1 if not found.
 */
extern "C" PLUGIN_API int ScriptLookupAttachment(int entityHandle, const plg::string& attachmentName) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return -1;
	return entity->ScriptLookupAttachment(attachmentName.c_str());
}

/**
 * @brief Sets a bodygroup value by index.
 *
 * This function sets a specific bodygroup of the entity to the given value.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity.
 * @param group The bodygroup index.
 * @param value The new value to set for the bodygroup.
 */
extern "C" PLUGIN_API void SetEntityBodygroup(int entityHandle, int group, int value) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetBodygroup(group, value);
}

/**
 * @brief Sets a bodygroup value by name.
 *
 * This function sets a specific bodygroup of the entity using its name.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity.
 * @param name The bodygroup name.
 * @param value The new value to set for the bodygroup.
 */
extern "C" PLUGIN_API void SetEntityBodygroupByName(int entityHandle, const plg::string& name, int value) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetBodygroupByName(name.c_str(), value);
}

/**
 * @brief Sets the light group of an entity.
 *
 * This function assigns the entity to the specified light group.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity.
 * @param lightGroup The light group name.
 */
extern "C" PLUGIN_API void SetEntityLightGroup(int entityHandle, const plg::string& lightGroup) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetLightGroup(lightGroup.c_str());
}

/**
 * @brief Sets the material group of an entity.
 *
 * This function changes the entity’s material group.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity.
 * @param materialGroup The material group name.
 */
extern "C" PLUGIN_API void SetEntityMaterialGroup(int entityHandle, const plg::string& materialGroup) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetMaterialGroup(materialGroup.c_str());
}

/**
 * @brief Sets the material group hash of an entity.
 *
 * @param entityHandle The handle of the entity.
 * @param hash The new material group hash to set.
 */
extern "C" PLUGIN_API void SetEntityMaterialGroupHash(int entityHandle, uint32_t hash) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetMaterialGroupHash(hash);
}

/**
 * @brief Sets the material group mask of an entity.
 *
 * @param entityHandle The handle of the entity.
 * @param mask The new mesh group mask to set.
 */
extern "C" PLUGIN_API void SetEntityMaterialGroupMask(int entityHandle, uint64 mask) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetMaterialGroupMask(mask);
}

/**
 * @brief Sets the model scale of an entity.
 *
 * This function changes the scale of the entity’s model.
 *
 * @param entityHandle The handle of the entity.
 * @param scale The new scale factor.
 */
extern "C" PLUGIN_API void SetEntityModelScale(int entityHandle, float scale) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetModelScale(scale);
}

/**
 * @brief Sets the render alpha of an entity.
 *
 * This function modifies the alpha transparency of the entity for rendering.
 *
 * @param entityHandle The handle of the entity.
 * @param alpha The new alpha value (0–255).
 */
extern "C" PLUGIN_API void SetEntityRenderAlpha(int entityHandle, int alpha) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetRenderAlpha(alpha);
}

/**
 * @brief Sets the render color of an entity.
 *
 * @param entityHandle The handle of the entity.
 * @param r The red component (0–255).
 * @param g The green component (0–255).
 * @param b The blue component (0–255).
 */
extern "C" PLUGIN_API void SetEntityRenderColor2(int entityHandle, int r, int g, int b) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetRenderColor(r, g, b);
}

/**
 * @brief Sets the render mode of an entity.
 *
 * @param entityHandle The handle of the entity.
 * @param mode The new render mode value.
 */
extern "C" PLUGIN_API void SetEntityRenderMode2(int entityHandle, int mode) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetRenderMode(mode);
}

/**
 * @brief Sets a single mesh group for an entity.
 *
 * @param entityHandle The handle of the entity.
 * @param meshGroupName The name of the mesh group.
 */
extern "C" PLUGIN_API void SetEntitySingleMeshGroup(int entityHandle, const plg::string& meshGroupName) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetSingleMeshGroup(meshGroupName.c_str());
}

/**
 * @brief Sets the size (bounding box) of an entity.
 *
 * @param entityHandle The handle of the entity.
 * @param mins The minimum bounding box vector.
 * @param maxs The maximum bounding box vector.
 */
extern "C" PLUGIN_API void SetEntitySize(int entityHandle, const plg::vec3& mins, const plg::vec3& maxs) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetSize(*reinterpret_cast<const Vector*>(&mins), *reinterpret_cast<const Vector*>(&maxs));
}

/**
 * @brief Sets the skin of an entity.
 *
 * @param entityHandle The handle of the entity.
 * @param skin The new skin index.
 */
extern "C" PLUGIN_API void SetEntitySkin(int entityHandle, int skin) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetSkin(skin);
}

PLUGIFY_WARN_POP()

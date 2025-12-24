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
 * @brief Converts an entity index into an entity pointer.
 *
 * This function retrieves a pointer to the entity corresponding to the given
 * entity index. It is essential for accessing entity properties and methods
 * using the entity index.
 *
 * @param entityIndex The index of the entity to convert.
 * @return A pointer to the entity instance, or nullptr if the entity does not exist.
 */
extern "C" PLUGIN_API void* EntIndexToEntPointer(int entityIndex) {
	return g_pGameEntitySystem->GetEntityInstance(CEntityIndex(entityIndex));
}

/**
 * @brief Retrieves the entity index from an entity pointer.
 *
 * This function returns the index associated with a given entity pointer,
 * allowing for the identification of the entity's position in the entity list.
 *
 * @param entity A pointer to the entity whose index is to be retrieved.
 * @return The index of the entity, or -1 if the entity is nullptr.
 */
extern "C" PLUGIN_API int EntPointerToEntIndex(CEntityInstance* entity) {
	if (!g_pGameEntitySystem->IsEntityPtr(entity)) {
		return -1;
	}

	return entity->GetRefEHandle().GetEntryIndex();
}

/**
 * @brief Converts an entity pointer into an entity handle.
 *
 * This function creates and returns an entity handle from a given entity pointer.
 * If the entity pointer is null, it returns an invalid entity handle index.
 *
 * @param entity A pointer to the entity to convert.
 * @return The entity handle as an integer, or INVALID_EHANDLE_INDEX if the entity is nullptr.
 */
extern "C" PLUGIN_API int EntPointerToEntHandle(CEntityInstance* entity) {
	if (!g_pGameEntitySystem->IsEntityPtr(entity)) {
		return INVALID_EHANDLE_INDEX;
	}

	return entity->GetRefEHandle().ToInt();
}

/**
 * @brief Retrieves the entity pointer from an entity handle.
 *
 * This function retrieves the entity pointer corresponding to the given entity handle.
 * If the handle is invalid, it returns nullptr.
 *
 * @param entityHandle The entity handle to convert.
 * @return A pointer to the entity instance, or nullptr if the handle is invalid.
 */
extern "C" PLUGIN_API void* EntHandleToEntPointer(int entityHandle) {
	CEntityHandle handle(entityHandle);
	if (!handle.IsValid()) {
		return nullptr;
	}

	return g_pGameEntitySystem->GetEntityInstance(handle);
}

/**
 * @brief Converts an entity index into an entity handle.
 *
 * This function converts a given entity index into its corresponding entity handle.
 * If the entity index is invalid, it returns -1.
 *
 * @param entityIndex The index of the entity to convert.
 * @return The entity handle as an integer, or -1 if the entity index is invalid.
 */
extern "C" PLUGIN_API int EntIndexToEntHandle(int entityIndex) {
	CBaseEntity* entity = static_cast<CBaseEntity*>(g_pGameEntitySystem->GetEntityInstance(CEntityIndex(entityIndex)));
	if (!entity) {
		return INVALID_EHANDLE_INDEX;
	}

	return entity->GetRefEHandle().ToInt();
}

/**
 * @brief Retrieves the entity index from an entity handle.
 *
 * This function retrieves the entity index corresponding to the provided entity handle.
 * If the handle is invalid or does not correspond to an entity, it returns -1.
 *
 * @param entityHandle The entity handle from which to retrieve the index.
 * @return The index of the entity, or -1 if the handle is invalid.
 */
extern "C" PLUGIN_API int EntHandleToEntIndex(int entityHandle) {
	CEntityHandle handle(entityHandle);
	if (!handle.IsValid()) {
		return -1;
	}

	CBaseEntity* entity = static_cast<CBaseEntity*>(g_pGameEntitySystem->GetEntityInstance(handle));
	if (!entity) {
		return -1;
	}

	return entity->entindex();
}

/**
 * @brief Checks if the provided entity handle is valid.
 *
 * This function checks whether the entity handle points to a valid entity in the
 * entity system. It returns true if valid, and false otherwise.
 *
 * @param entityHandle The entity handle to check.
 * @return True if the entity handle is valid, false otherwise.
 */
extern "C" PLUGIN_API bool IsValidEntHandle(int entityHandle) {
	CEntityHandle handle(entityHandle);
	if (!handle.IsValid()) {
		return false;
	}

	return g_pGameEntitySystem->GetEntityInstance(handle) != nullptr;
}

/**
 * @brief Checks if the provided entity pointer is valid.
 *
 * This function checks whether the entity pointer points to a valid entity in the
 * entity system. It returns true if valid, and false otherwise.
 *
 * @param entity The entity pointer to check.
 * @return True if the entity pointer is valid, false otherwise.
 */
extern "C" PLUGIN_API bool IsValidEntPointer(CEntityInstance* entity) {
	return g_pGameEntitySystem->IsEntityPtr(entity);
}

/**
 * @brief Retrieves the pointer to the first active entity.
 *
 * This function returns a pointer to the first active entity in the entity system,
 * which can be useful for iterating through active entities.
 *
 * @return A handle to the first active entity.
 */
extern "C" PLUGIN_API int GetFirstActiveEntity() {
	auto* first = g_pGameEntitySystem->m_EntityList.m_pFirstActiveEntity;
	return first ? first->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Retrieves the previous active entity.
 *
 * @param entity Current entity.
 * @return int32 Handle to the previous entity.
 */
extern "C" PLUGIN_API int GetPrevActiveEntity(int entityHandle) {
	auto* entity = g_pGameEntitySystem->GetEntityIdentity(CEntityHandle(entityHandle));
	return entity && entity->m_pPrev ? entity->m_pPrev->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Retrieves the next active entity.
 *
 * @param entity Current entity.
 * @return int32 Handle to the next entity.
 */
extern "C" PLUGIN_API int GetNextActiveEntity(int entityHandle) {
	auto* entity = g_pGameEntitySystem->GetEntityIdentity(CEntityHandle(entityHandle));
	return entity && entity->m_pNext ? entity->m_pNext->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Adds an entity output hook on a specified entity class name.
 *
 * This function hooks a specified output for a given entity class name. The provided
 * callback will be triggered when the output is fired.
 *
 * @param classname The class name of the entity to hook the output for.
 * @param output The output event name to hook.
 * @param callback The callback function to invoke when the output is fired.
 * @param mode Whether the hook was in post mode (after processing) or pre mode (before processing).
 * @return True if the hook was successfully added, false otherwise.
 */
extern "C" PLUGIN_API bool HookEntityOutput(const plg::string& classname, const plg::string& output, EntityListenerCallback callback, HookMode mode) {
	return g_EntityOutputManager.HookEntityOutput(classname, output, callback, mode);
}

/**
 * @brief Removes an entity output hook.
 *
 * This function unhooks a previously set output for a given entity class name.
 *
 * @param classname The class name of the entity from which to unhook the output.
 * @param output The output event name to unhook.
 * @param callback The callback function that was previously hooked.
 * @param mode Whether the hook was in post mode (after processing) or pre mode (before processing).
* @return True if the hook was successfully removed, false otherwise.
 */
extern "C" PLUGIN_API bool UnhookEntityOutput(const plg::string& classname, const plg::string& output, EntityListenerCallback callback, HookMode mode) {
	return g_EntityOutputManager.UnhookEntityOutput(classname, output, callback, mode);
}

////////////////////////

/**
 * @brief Finds an entity by classname with iteration.
 *
 * @param startFrom The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
 * @param classname The class name to search for.
 * @return The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
 */
extern "C" PLUGIN_API int FindEntityByClassname(int startFrom, const plg::string& classname) {
	CEntityInstance* startEnt = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(startFrom));
	CEntityInstance* found = entities::FindEntityByClassName(startEnt, classname.c_str());
	return found ? found->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Finds the nearest entity by classname to a point.
 *
 * @param classname The class name to search for.
 * @param origin The center point to search around.
 * @param maxRadius Maximum search radius.
 * @return The handle of the nearest entity, or INVALID_EHANDLE_INDEX if none found.
 */
extern "C" PLUGIN_API int FindEntityByClassnameNearest(const plg::string& classname, const plg::vec3& origin, float maxRadius) {
	CEntityInstance* found = entities::FindEntityByClassNameNearest(classname.c_str(), *reinterpret_cast<const Vector*>(&origin), maxRadius);
	return found ? found->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Finds an entity by classname within a radius with iteration.
 *
 * @param startFrom The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
 * @param classname The class name to search for.
 * @param origin The center of the search sphere.
 * @param radius The search radius.
 * @return The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
 */
extern "C" PLUGIN_API int FindEntityByClassnameWithin(int startFrom, const plg::string& classname, const plg::vec3& origin, float radius) {
	CEntityInstance* startEnt = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(startFrom));
	CEntityInstance* found = entities::FindEntityByClassNameWithin(startEnt, classname.c_str(), *reinterpret_cast<const Vector*>(&origin), radius);
	return found ? found->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Finds an entity by name with iteration.
 *
 * @param startFrom The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
 * @param name The targetname to search for.
 * @return The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
 */
extern "C" PLUGIN_API int FindEntityByName(int startFrom, const plg::string& name) {
	CEntityInstance* startEnt = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(startFrom));
	CEntityInstance* found = entities::FindEntityByName(startEnt, name.c_str());
	return found ? found->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Finds the nearest entity by name to a point.
 *
 * @param name The targetname to search for.
 * @param origin The point to search around.
 * @param maxRadius Maximum search radius.
 * @return The handle of the nearest entity, or INVALID_EHANDLE_INDEX if none found.
 */
extern "C" PLUGIN_API int FindEntityByNameNearest(const plg::string& name, const plg::vec3& origin, float maxRadius) {
	CEntityInstance* found = entities::FindByNameNearest(name.c_str(), *reinterpret_cast<const Vector*>(&origin), maxRadius);
	return found ? found->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Finds an entity by name within a radius with iteration.
 *
 * @param startFrom The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
 * @param name The targetname to search for.
 * @param origin The center of the search sphere.
 * @param radius The search radius.
 * @return The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
 */
extern "C" PLUGIN_API int FindEntityByNameWithin(int startFrom, const plg::string& name, const plg::vec3& origin, float radius) {
	CEntityInstance* startEnt = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(startFrom));
	CEntityInstance* found = entities::FindByNameWithin(startEnt, name.c_str(), *reinterpret_cast<const Vector*>(&origin), radius);
	return found ? found->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Finds an entity by targetname with iteration.
 *
 * @param startFrom The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
 * @param name The targetname to search for.
 * @return The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
 */
extern "C" PLUGIN_API int FindEntityByTarget(int startFrom, const plg::string& name) {
	CEntityInstance* startEnt = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(startFrom));
	CEntityInstance* found = entities::FindByTarget(startEnt, name.c_str());
	return found ? found->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Finds an entity within a sphere with iteration.
 *
 * @param startFrom The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
 * @param origin The center of the search sphere.
 * @param radius The search radius.
 * @return The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
 */
extern "C" PLUGIN_API int FindEntityInSphere(int startFrom, const plg::vec3& origin, float radius) {
	CEntityInstance* startEnt = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(startFrom));
	CEntityInstance* found = entities::FindInSphere(startEnt, nullptr, *reinterpret_cast<const Vector*>(&origin), radius);
	return found ? found->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Creates an entity by classname.
 *
 * @param className The class name of the entity to create.
 * @return The handle of the created entity, or INVALID_EHANDLE_INDEX if creation failed.
 */
extern "C" PLUGIN_API int SpawnEntityByName(const plg::string& className) {
	CEntityInstance* ent = entities::CreateEntityByName(className.c_str());
	return ent ? ent->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Creates an entity by string name but does not spawn it.
 *
 * This function creates an entity using the specified class name. The entity
 * is not spawned until DispatchSpawn is called. If ForceEdictIndex is not -1,
 * it will use the edict by that index. If the index is invalid or there is
 * already an edict using that index, it will error out.
 *
 * @param className The class name of the entity to create.
 * @return The entity handle of the created entity, or INVALID_EHANDLE_INDEX if the entity could not be created.
 */
extern "C" PLUGIN_API int CreateEntityByName(const plg::string& className) {
	CEntityInstance* ent = addresses::CreateEntityByName(className.c_str(), -1);
	return ent ? ent->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Spawns an entity into the game.
 *
 * This function spawns the specified entity into the game world.
 * It requires a valid entity handle to function correctly.
 *
 * @param entityHandle The handle of the entity to spawn.
 */
extern "C" PLUGIN_API void DispatchSpawn(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->DispatchSpawn();
}

/**
 * @brief Spawns an entity into the game with key-value properties.
 *
 * This function spawns the specified entity into the game world.
 * It requires a valid entity handle to function correctly.
 *
 * @param entityHandle The handle of the entity to spawn.
 * @param keys A vector of keys representing the property names to set on the entity.
 * @param values A vector of values corresponding to the keys, representing the property values to set on the entity.
 */
extern "C" PLUGIN_API void DispatchSpawn2(int entityHandle, const plg::vector<plg::string>& keys, const plg::vector<plg::any>& values) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;

	if (keys.size() != values.size()) {
		plg::print(LS_WARNING, "Cannot execute 'DispatchSpawn2': Mismatch between keys and values sizes. Keys size: {}, Values size: {}.",
			static_cast<int>(keys.size()), static_cast<int>(values.size()));
		return;
	}

	entity->DispatchSpawn(keys, values);
}

/**
 * @brief Marks an entity for deletion.
 *
 * This function marks the specified entity for deletion from the game world.
 * It requires a valid entity handle to function correctly.
 *
 * @param entityHandle The handle of the entity to be deleted.
 */
extern "C" PLUGIN_API void RemoveEntity(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->Remove();
}

/**
 * @brief Checks if an entity is a player controller.
 *
 * This function determines whether the specified entity represents a player controller.
 * If the entity is invalid, it returns false.
 *
 * @param entityHandle The handle of the entity.
 * @return True if the entity is a player controller, false otherwise.
 */
extern "C" PLUGIN_API bool IsEntityPlayerController(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return false;
	return entity->IsPlayerController();
}

/**
 * @brief Checks if an entity is a player pawn.
 *
 * This function determines whether the specified entity represents a player pawn.
 * If the entity is invalid, it returns false.
 *
 * @param entityHandle The handle of the entity.
 * @return True if the entity is a player pawn, false otherwise.
 */
extern "C" PLUGIN_API bool IsEntityPlayerPawn(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return false;
	return entity->IsPlayerPawn();
}

///

/**
 * @brief Retrieves the class name of an entity.
 *
 * This function gets the class name of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose class name is to be retrieved.
 * @return A string where the class name will be stored.
 */
extern "C" PLUGIN_API plg::string GetEntityClassname(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->GetClassname();
}

/**
 * @brief Retrieves the name of an entity.
 *
 * This function gets the name of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose name is to be retrieved.
 * @retrun A string where the entity name will be stored.
 */
extern "C" PLUGIN_API plg::string GetEntityName(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->GetName();
}

/**
 * @brief Sets the name of an entity.
 *
 * This function updates the name of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose name is to be set.
 * @param name The new name to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityName(int entityHandle, const plg::string& name) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetEntityName(name.c_str());
}

/**
 * @brief Retrieves the movement type of an entity.
 *
 * This function returns the movement type of the specified entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity whose movement type is to be retrieved.
 * @return The movement type of the entity, or 0 if the entity is invalid.
 */
extern "C" PLUGIN_API MoveType_t GetEntityMoveType(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->m_MoveType;
}

/**
 * @brief Sets the movement type of an entity.
 *
 * This function updates the movement type of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose movement type is to be set.
 * @param moveType The new movement type to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityMoveType(int entityHandle, MoveType_t moveType) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetMoveType(moveType);
}

/**
 * @brief Retrieves the gravity scale of an entity.
 *
 * This function returns the gravity scale of the specified entity.
 * If the entity is invalid, it returns 0.0f.
 *
 * @param entityHandle The handle of the entity whose gravity scale is to be retrieved.
 * @return The gravity scale of the entity, or 0.0f if the entity is invalid.
 */
extern "C" PLUGIN_API float GetEntityGravity(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->m_flGravityScale;
}

/**
 * @brief Sets the gravity scale of an entity.
 *
 * This function updates the gravity scale of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose gravity scale is to be set.
 * @param gravity The new gravity scale to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityGravity(int entityHandle, float gravity) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetGravity(gravity);
}

/**
 * @brief Retrieves the flags of an entity.
 *
 * This function returns the flags of the specified entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity whose flags are to be retrieved.
 * @return The flags of the entity, or 0 if the entity is invalid.
 */
extern "C" PLUGIN_API int GetEntityFlags(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return *entity->m_fFlags;
}

/**
 * @brief Sets the flags of an entity.
 *
 * This function updates the flags of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose flags are to be set.
 * @param flags The new flags to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityFlags(int entityHandle, int flags) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->m_fFlags = static_cast<uint32>(flags);
}

/**
 * @brief Retrieves the render color of an entity.
 *
 * This function gets the render color of the specified entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity whose render color is to be retrieved.
 * @return The raw color value of the entity's render color, or 0 if the entity is invalid.
 */
extern "C" PLUGIN_API int GetEntityRenderColor(int entityHandle) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return {};
	Color color = entity->m_clrRender;
	return *reinterpret_cast<const int*>(&color);
}

/**
 * @brief Sets the render color of an entity.
 *
 * This function updates the render color of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose render color is to be set.
 * @param color The new raw color value to set for the entity's render color.
 */
extern "C" PLUGIN_API void SetEntityRenderColor(int entityHandle, int color) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->m_clrRender = *reinterpret_cast<Color*>(&color);
}

/**
 * @brief Retrieves the render mode of an entity.
 *
 * This function gets the render mode of the specified entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity whose render mode is to be retrieved.
 * @return The render mode of the entity, or 0 if the entity is invalid.
 */
extern "C" PLUGIN_API RenderMode_t GetEntityRenderMode(int entityHandle) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return {};
	return static_cast<RenderMode_t>(*entity->m_nRenderMode);
}

/**
 * @brief Sets the render mode of an entity.
 *
 * This function updates the render mode of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose render mode is to be set.
 * @param renderMode The new render mode to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityRenderMode(int entityHandle, RenderMode_t renderMode) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetRenderMode(renderMode);
}

/**
 * @brief Retrieves the mass of an entity.
 *
 * This function returns the current mass of the specified entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity whose mass is to be retrieved.
 * @return The mass of the entity, or 0 if the entity is invalid.
 */
extern "C" PLUGIN_API int GetEntityMass(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->GetMass();
}

/**
 * @brief Sets the mass of an entity.
 *
 * This function updates the mass of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose mass is to be set.
 * @param mass The new mass value to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityMass(int entityHandle, int mass) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetMass(mass);
}

/**
 * @brief Retrieves the friction of an entity.
 *
 * This function returns the current friction of the specified entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity whose friction is to be retrieved.
 * @return The friction of the entity, or 0 if the entity is invalid.
 */
extern "C" PLUGIN_API float GetEntityFriction(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->m_flFriction;
}

/**
 * @brief Sets the friction of an entity.
 *
 * This function updates the friction of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose friction is to be set.
 * @param friction The new friction value to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityFriction(int entityHandle, float friction) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetFriction(friction);
}

/**
 * @brief Retrieves the health of an entity.
 *
 * This function returns the current health of the specified entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity whose health is to be retrieved.
 * @return The health of the entity, or 0 if the entity is invalid.
 */
extern "C" PLUGIN_API int GetEntityHealth(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->GetHealth();
}

/**
 * @brief Sets the health of an entity.
 *
 * This function updates the health of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose health is to be set.
 * @param health The new health value to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityHealth(int entityHandle, int health) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetHealth(health);
}

/**
 * @brief Retrieves the max health of an entity.
 *
 * This function returns the current max health of the specified entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity whose max health is to be retrieved.
 * @return The max health of the entity, or 0 if the entity is invalid.
 */
extern "C" PLUGIN_API int GetEntityMaxHealth(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->GetMaxHealth();
}

/**
 * @brief Sets the max health of an entity.
 *
 * This function updates the max health of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose max health is to be set.
 * @param maxHealth The new max health value to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityMaxHealth(int entityHandle, int maxHealth) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetMaxHealth(maxHealth);
}

/**
 * @brief Retrieves the team number of an entity.
 *
 * This function returns the team number of the specified entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity whose team number is to be retrieved.
 * @return The team number of the entity, or 0 if the entity is invalid.
 */
extern "C" PLUGIN_API CSTeam GetEntityTeam(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->GetTeam();
}

/**
 * @brief Sets the team number of an entity.
 *
 * This function updates the team number of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose team number is to be set.
 * @param team The new team number to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityTeam(int entityHandle, CSTeam team) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetTeam(team);
}

/**
 * @brief Retrieves the owner of an entity.
 *
 * This function returns the entity handle of the owner for the specified entity.
 * If the entity is invalid, it returns -1.
 *
 * @param entityHandle The handle of the entity whose owner is to be retrieved.
 * @return The handle of the owner entity, or INVALID_EHANDLE_INDEX if the entity is invalid.
 */
extern "C" PLUGIN_API int GetEntityOwner(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	auto owner = entity->GetOwner();
	auto instance = reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(owner));
	return instance != nullptr ? instance->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Sets the owner of an entity.
 *
 * This function updates the owner of the specified entity.
 * If either the entity or the new owner is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose owner is to be set.
 * @param ownerHandle The handle of the new owner entity.
 */
extern "C" PLUGIN_API void SetEntityOwner(int entityHandle, int ownerHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	auto* owner = helpers::GetEntity(ownerHandle);
	if (!owner) return;
	entity->SetOwner(owner->GetScriptInstance());
}

/**
 * @brief Retrieves the parent of an entity.
 *
 * This function returns the entity handle of the parent for the specified entity.
 * If the entity is invalid, it returns -1.
 *
 * @param entityHandle The handle of the entity whose parent is to be retrieved.
 * @return The handle of the parent entity, or INVALID_EHANDLE_INDEX if the entity is invalid.
 */
extern "C" PLUGIN_API int GetEntityParent(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	auto parent = entity->GetMoveParent();
	auto instance = reinterpret_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(parent));
	return instance != nullptr ? instance->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Sets the parent of an entity.
 *
 * This function updates the parent of the specified entity.
 * If either the entity or the new parent is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose parent is to be set.
 * @param parentHandle The handle of the new parent entity.
 * @param attachmentName The name of the entity's attachment.
 */
extern "C" PLUGIN_API void SetEntityParent(int entityHandle, int parentHandle, const plg::string& attachmentName) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	auto* parent = helpers::GetEntity(parentHandle);
	if (!parent) return;
	entity->SetParent(parent->GetScriptInstance(), attachmentName.c_str());
}

/**
 * @brief Retrieves the absolute origin of an entity.
 *
 * This function gets the absolute position of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose absolute origin is to be retrieved.
 * @return A vector where the absolute origin will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityAbsOrigin(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	const Vector& vec = entity->GetAbsOrigin();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Sets the absolute origin of an entity.
 *
 * This function updates the absolute position of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose absolute origin is to be set.
 * @param origin The new absolute origin to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityAbsOrigin(int entityHandle, const plg::vec3& origin) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetAbsOrigin(*reinterpret_cast<const Vector*>(&origin));
}

/**
 * @brief Retrieves the absolute scale of an entity.
 *
 * This function gets the absolute position of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose absolute scale is to be retrieved.
 * @return A vector where the absolute scale will be stored.
 */
extern "C" PLUGIN_API float GetEntityAbsScale(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->GetAbsScale();
}

/**
 * @brief Sets the absolute scale of an entity.
 *
 * This function updates the absolute position of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose absolute scale is to be set.
 * @param scale The new absolute scale to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityAbsScale(int entityHandle, float scale) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetAbsScale(scale);
}

/**
 * @brief Retrieves the angular rotation of an entity.
 *
 * This function gets the angular rotation of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose angular rotation is to be retrieved.
 * @return A QAngle where the angular rotation will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityAbsAngles(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	const QAngle& ang = entity->GetAngles();
	return *reinterpret_cast<const plg::vec3*>(&ang);
}

/**
 * @brief Sets the angular rotation of an entity.
 *
 * This function updates the angular rotation of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose angular rotation is to be set.
 * @param angle The new angular rotation to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityAbsAngles(int entityHandle, const QAngle& angle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetAbsAngles(angle.x, angle.y, angle.z);
}

/**
 * @brief Retrieves the local origin of an entity.
 *
 * This function gets the local position of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose local origin is to be retrieved.
 * @return A vector where the local origin will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityLocalOrigin(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	const Vector& vec = entity->GetLocalOrigin();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Sets the local origin of an entity.
 *
 * This function updates the local position of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose local origin is to be set.
 * @param origin The new local origin to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityLocalOrigin(int entityHandle, const plg::vec3& origin) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetLocalOrigin(*reinterpret_cast<const Vector*>(&origin));
}

/**
 * @brief Retrieves the local scale of an entity.
 *
 * This function gets the local position of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose local scale is to be retrieved.
 * @return A vector where the local scale will be stored.
 */
extern "C" PLUGIN_API float GetEntityLocalScale(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->GetLocalScale();
}

/**
 * @brief Sets the local scale of an entity.
 *
 * This function updates the local position of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose local scale is to be set.
 * @param scale The new local scale to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityLocalScale(int entityHandle, float scale) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetLocalScale(scale);
}

/**
 * @brief Retrieves the angular rotation of an entity.
 *
 * This function gets the angular rotation of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose angular rotation is to be retrieved.
 * @return A QAngle where the angular rotation will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityLocalAngles(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	const QAngle& ang = entity->GetAngles();
	return *reinterpret_cast<const plg::vec3*>(&ang);
}

/**
 * @brief Sets the angular rotation of an entity.
 *
 * This function updates the angular rotation of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose angular rotation is to be set.
 * @param angle The new angular rotation to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityLocalAngles(int entityHandle, const QAngle& angle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetLocalAngles(angle.x, angle.y, angle.z);
}

/**
 * @brief Retrieves the absolute velocity of an entity.
 *
 * This function gets the absolute velocity of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose absolute velocity is to be retrieved.
 * @return A vector where the absolute velocity will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityAbsVelocity(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	const Vector& vec = entity->GetVelocity();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Sets the absolute velocity of an entity.
 *
 * This function updates the absolute velocity of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose absolute velocity is to be set.
 * @param velocity The new absolute velocity to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityAbsVelocity(int entityHandle, const plg::vec3& velocity) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetVelocity(*reinterpret_cast<const Vector*>(&velocity));
}

/**
 * @brief Retrieves the base velocity of an entity.
 *
 * This function gets the base velocity of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose base velocity is to be retrieved.
 * @return A vector where the base velocity will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityBaseVelocity(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	const Vector& vec = entity->GetBaseVelocity();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Retrieves the local angular velocity of an entity.
 *
 * This function gets the local angular velocity of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose local angular velocity is to be retrieved.
 * @return A vector where the local angular velocity will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityLocalAngVelocity(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	const QAngle& ang = entity->GetLocalAngularVelocity();
	return *reinterpret_cast<const plg::vec3*>(&ang);
}

/**
 * @brief Retrieves the angular velocity of an entity.
 *
 * This function gets the angular velocity of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose angular velocity is to be retrieved.
 * @return A vector where the angular velocity will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityAngVelocity(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	const Vector& vec = entity->GetAngularVelocity();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}
/**
 * @brief Sets the angular velocity of an entity.
 *
 * This function updates the angular velocity of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose angular velocity is to be set.
 * @param velocity The new angular velocity to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityAngVelocity(int entityHandle, const plg::vec3& velocity) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetAngularVelocity(velocity.x, velocity.y, velocity.z);
}

/**
 * @brief Retrieves the local velocity of an entity.
 *
 * This function gets the local velocity of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose local velocity is to be retrieved.
 * @return A vector where the local velocity will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityLocalVelocity(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	const Vector& vec = entity->GetLocalVelocity();
	return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
* @brief Retrieves the angular rotation of an entity.
 *
 * This function gets the angular rotation of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose angular rotation is to be retrieved.
 * @return A vector where the angular rotation will be stored.
 */
extern "C" PLUGIN_API plg::vec3 GetEntityAngRotation(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	const QAngle& ang = entity->m_CBodyComponent->m_pSceneNode->m_angRotation;
	return *reinterpret_cast<const plg::vec3*>(&ang);
}
/**
 * @brief Sets the angular rotation of an entity.
 *
 * This function updates the angular rotation of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose angular rotation is to be set.
 * @param rotation The new angular rotation to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityAngRotation(int entityHandle, const plg::vec3& rotation) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->m_CBodyComponent->m_pSceneNode->m_angRotation = *reinterpret_cast<const QAngle*>(&rotation);
}

/**
 * @brief Returns the input Vector transformed from entity to world space.
 *
 * Transforms a point from the entity's local coordinate space to world coordinate space.
 *
 * @param entityHandle The handle of the entity
 * @param point Point in entity local space to transform
 * @return The point transformed to world space coordinates
 */
extern "C" PLUGIN_API plg::vec3 TransformPointEntityToWorld(int entityHandle, const plg::vec3& point) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const Vector& pos = entity->TransformPointEntityToWorld(*reinterpret_cast<const Vector*>(&point));
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Returns the input Vector transformed from world to entity space.
 *
 * Transforms a point from world coordinate space to the entity's local coordinate space.
 *
 * @param entityHandle The handle of the entity
 * @param point Point in world space to transform
 * @return The point transformed to entity local space coordinates
 */
extern "C" PLUGIN_API plg::vec3 TransformPointWorldToEntity(int entityHandle, const plg::vec3& point) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const Vector& pos = entity->TransformPointWorldToEntity(*reinterpret_cast<const Vector*>(&point));
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get vector to eye position - absolute coords.
 *
 * Returns the position of the entity's eyes in world space coordinates.
 *
 * @param entityHandle The handle of the entity
 * @return Eye position in absolute/world coordinates
 */
extern "C" PLUGIN_API plg::vec3 GetEntityEyePosition(int entityHandle) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const Vector& pos = entity->EyePosition();
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get the qangles that this entity is looking at.
 *
 * Returns the eye angles (pitch, yaw, roll) representing the direction
 * the entity is looking.
 *
 * @param entityHandle The handle of the entity
 * @return Eye angles as a vector (pitch, yaw, roll)
 */
extern "C" PLUGIN_API plg::vec3 GetEntityEyeAngles(int entityHandle) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const QAngle& ang = entity->EyeAngles();
    return *reinterpret_cast<const plg::vec3*>(&ang);
}

/**
 * @brief Sets the forward velocity of an entity.
 *
 * This function updates the forward velocity of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose forward velocity is to be set.
 * @param forward The new forward velocity to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityForwardVector(int entityHandle, const plg::vec3& forward) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->SetForwardVector(*reinterpret_cast<const Vector*>(&forward));
}

/**
 * @brief Get the forward vector of the entity.
 *
 * @param entityHandle The handle of the entity to query
 * @return Forward-facing direction vector of the entity
 */
extern "C" PLUGIN_API plg::vec3 GetEntityForwardVector(int entityHandle) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const Vector& pos = entity->GetForwardVector();
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get the left vector of the entity.
 *
 * @param entityHandle The handle of the entity to query
 * @return Left-facing direction vector of the entity (aligned with the y axis)
 */
extern "C" PLUGIN_API plg::vec3 GetEntityLeftVector(int entityHandle) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const Vector& pos = entity->GetLeftVector();
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get the right vector of the entity.
 *
 * @warning This produces a left-handed coordinate system. Use GetLeftVector instead
 *          (which is aligned with the y axis of the entity).
 *
 * @param entityHandle The handle of the entity to query
 * @return Right-facing direction vector of the entity
 */
extern "C" PLUGIN_API plg::vec3 GetEntityRightVector(int entityHandle) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const Vector& pos = entity->GetRightVector();
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get the up vector of the entity.
 *
 * @param entityHandle The handle of the entity to query
 * @return Up-facing direction vector of the entity
 */
extern "C" PLUGIN_API plg::vec3 GetEntityUpVector(int entityHandle) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const Vector& pos = entity->GetUpVector();
    return *reinterpret_cast<const plg::vec3*>(&pos);
}

/**
 * @brief Get the entity-to-world transformation matrix.
 *
 * Returns the complete transformation matrix that converts coordinates from
 * entity local space to world space.
 *
 * @param entityHandle The handle of the entity to query
 * @return 4x4 transformation matrix representing entity's position, rotation, and scale in world space
 */
extern "C" PLUGIN_API plg::mat4x4 GetEntityTransform(int entityHandle) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const matrix3x4_t& mat = entity->m_CBodyComponent->m_pSceneNode->EntityToWorldTransform();
    return *reinterpret_cast<const plg::mat4x4*>(&mat);
}

/**
 * @brief Retrieves the model name of an entity.
 *
 * This function gets the model name of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose model name is to be retrieved.
 * @return A string where the model name will be stored.
 */
extern "C" PLUGIN_API plg::string GetEntityModel(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->GetModelName();
}

/**
 * @brief Sets the model name of an entity.
 *
 * This function updates the model name of the specified entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity whose model name is to be set.
 * @param model The new model name to set for the entity.
 */
extern "C" PLUGIN_API void SetEntityModel(int entityHandle, const plg::string& model) {
	auto* entity = helpers::GetEntity<CBaseModelEntity>(entityHandle);
	if (!entity) return;
	entity->SetModel(model.c_str());
}

/**
 * @brief Retrieves the water level of an entity.
 *
 * This function returns the water level of the specified entity.
 * If the entity is invalid, it returns 0.0f.
 *
 * @param entityHandle The handle of the entity whose water level is to be retrieved.
 * @return The water level of the entity, or 0.0f if the entity is invalid.
 */
extern "C" PLUGIN_API float GetEntityWaterLevel(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->m_flWaterLevel;
}

/**
 * @brief Retrieves the ground entity of an entity.
 *
 * This function returns the handle of the ground entity for the specified entity.
 * If the entity is invalid, it returns -1.
 *
 * @param entityHandle The handle of the entity whose ground entity is to be retrieved.
 * @return The handle of the ground entity, or INVALID_EHANDLE_INDEX if the entity is invalid.
 */
extern "C" PLUGIN_API int GetEntityGroundEntity(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};

	CBaseEntity* ground = *entity->m_hGroundEntity;
	if (!ground) {
		return INVALID_EHANDLE_INDEX;
	}

	return ground->GetRefEHandle().ToInt();
}

/**
 * @brief Retrieves the effects of an entity.
 *
 * This function returns the effect flags of the specified entity.
 * If the entity is invalid, it returns 0.
 *
 * @param entityHandle The handle of the entity whose effects are to be retrieved.
 * @return The effect flags of the entity, or 0 if the entity is invalid.
 */
extern "C" PLUGIN_API int GetEntityEffects(int entityHandle) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	return entity->m_fEffects;
}

/**
 * @brief Adds the render effect flag to an entity.
 *
 * @param entityHandle The handle of the entity to modify
 * @param effects Render effect flags to add
 */
extern "C" PLUGIN_API void AddEntityEffects(int entityHandle, int effects) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return;
    entity->AddEffects(effects);
}

/**
 * @brief Removes the render effect flag from an entity.
 *
 * @param entityHandle The handle of the entity to modify
 * @param effects Render effect flags to remove
 */
extern "C" PLUGIN_API void RemoveEntityEffects(int entityHandle, int effects) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return;
    entity->RemoveEffects(effects);
}

/**
 * @brief Get a vector containing max bounds, centered on object.
 *
 * @param entityHandle The handle of the entity to query
 * @return Vector containing the maximum bounds of the entity's bounding box
 */
extern "C" PLUGIN_API plg::vec3 GetEntityBoundingMaxs(int entityHandle) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const Vector& vec = entity->GetBoundingMaxs();
    return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Get a vector containing min bounds, centered on object.
 *
 * @param entityHandle The handle of the entity to query
 * @return Vector containing the minimum bounds of the entity's bounding box
 */
extern "C" PLUGIN_API plg::vec3 GetEntityBoundingMins(int entityHandle) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const Vector& vec = entity->GetBoundingMins();
    return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Get vector to center of object - absolute coords.
 *
 * @param entityHandle The handle of the entity to query
 * @return Vector pointing to the center of the entity in absolute/world coordinates
 */
extern "C" PLUGIN_API plg::vec3 GetEntityCenter(int entityHandle) {
    auto* entity = helpers::GetEntity(entityHandle);
    if (!entity) return {};
    const Vector& vec = entity->GetCenter();
    return *reinterpret_cast<const plg::vec3*>(&vec);
}

/**
 * @brief Teleports an entity to a specified location and orientation.
 *
 * This function teleports the specified entity to the given absolute position,
 * with an optional new orientation and velocity. If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity to teleport.
 * @param origin A pointer to a Vector representing the new absolute position. Use nan vector to not set.
 * @param angles A pointer to a QAngle representing the new orientation. Use nan vector to not set.
 * @param velocity A pointer to a Vector representing the new velocity. Use nan vector to not set.
 */
extern "C" PLUGIN_API void TeleportEntity(int entityHandle, const Vector& origin, const QAngle& angles, const Vector& velocity) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->Teleport(origin, angles, velocity);
}

/**
 * @brief Apply an absolute velocity impulse to an entity.
 *
 * Applies an instantaneous change to the entity's velocity in world space.
 *
 * @param entityHandle The handle of the entity to apply impulse to
 * @param vecImpulse Velocity impulse vector to apply
 */
extern "C" PLUGIN_API void ApplyAbsVelocityImpulseToEntity(int entityHandle, const plg::vec3& vecImpulse) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->ApplyAbsVelocityImpulse(*reinterpret_cast<const Vector*>(&vecImpulse));
}

/**
 * @brief Apply a local angular velocity impulse to an entity.
 *
 * Applies an instantaneous change to the entity's rotational velocity in local space.
 *
 * @param entityHandle The handle of the entity to apply impulse to
 * @param angImpulse Angular velocity impulse vector to apply
 */
extern "C" PLUGIN_API void ApplyLocalAngularVelocityImpulseToEntity(int entityHandle, const plg::vec3& angImpulse) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->ApplyLocalAngularVelocityImpulse(*reinterpret_cast<const Vector*>(&angImpulse));
}

/**
 * @brief Invokes a named input method on a specified entity.
 *
 * This function triggers an input action on an entity, allowing dynamic interaction
 * between game objects or entities within the system. The input can be initiated
 * by another entity (activator) or by a specific caller entity.
 *
 * @param entityHandle The handle of the target entity that will receive the input.
 * @param inputName    The name of the input action to invoke.
 * @param activatorHandle The handle of the entity that initiated the sequence of actions.
 * @param callerHandle The handle of the entity sending this event.
 * @param value        The value associated with the input action.
 * @param type         The type or classification of the value.
 * @param outputId      An identifier for tracking the output of this operation.
 */
extern "C" PLUGIN_API void AcceptEntityInput(int entityHandle, const plg::string& inputName, int activatorHandle, int callerHandle, const plg::any& value, FieldType type, int outputId) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	CEntityInstance* activator = activatorHandle != INVALID_EHANDLE_INDEX ? g_pGameEntitySystem->GetEntityInstance(CEntityHandle(callerHandle)) : nullptr;
	CEntityInstance* caller = callerHandle != INVALID_EHANDLE_INDEX ? g_pGameEntitySystem->GetEntityInstance(CEntityHandle(callerHandle)) : nullptr;
	variant_t variant = helpers::GetVariant(value, type);
	entity->AcceptInput(inputName.c_str(), variant, activator, caller, outputId);
}

/**
 * @brief Connects a script function to an entity output.
 *
 * This function will call the specified function on this entity when the output fires.
 *
 * @param entityHandle The handle of the entity.
 * @param output The name of the output to connect to.
 * @param functionName The name of the script function to call.
 */
extern "C" PLUGIN_API void ConnectEntityOutput(int entityHandle, const plg::string& output, const plg::string& functionName) {
	auto* entity = helpers::GetEntity<CEntityInstance2>(entityHandle);
	if (!entity) return;
	entity->ConnectOutput(output.c_str(), functionName.c_str());
}

/**
 * @brief Disconnects a script function from an entity output.
 *
 * This removes the function from being called when the output fires.
 *
 * @param entityHandle The handle of the entity.
 * @param output The name of the output.
 * @param functionName The name of the script function to disconnect.
 */
extern "C" PLUGIN_API void DisconnectEntityOutput(int entityHandle, const plg::string& output, const plg::string& functionName) {
	auto* entity = helpers::GetEntity<CEntityInstance2>(entityHandle);
	if (!entity) return;
	entity->DisconnectOutput(output.c_str(), functionName.c_str());
}

/**
 * @brief Disconnects a script function from an I/O event on a different entity.
 *
 * @param entityHandle The handle of the calling entity.
 * @param output The name of the output.
 * @param functionName The function name to disconnect.
 * @param targetHandle The handle of the entity whose output is being disconnected.
 */
extern "C" PLUGIN_API void DisconnectEntityRedirectedOutput(int entityHandle, const plg::string& output, const plg::string& functionName, int targetHandle) {
	auto* entity = helpers::GetEntity<CEntityInstance2>(entityHandle);
	if (!entity) return;
	auto* target = helpers::GetEntity(targetHandle);
	if (!target) return;
	entity->DisconnectRedirectedOutput(output.c_str(), functionName.c_str(), target->GetScriptInstance());
}

/**
 * @brief Fires an entity output.
 *
 * Calls all connected functions for the given output.
 *
 * @param entityHandle The handle of the entity firing the output.
 * @param outputName The name of the output to fire.
 * @param activatorHandle The entity activating the output.
* @param callerHandle The entity that called the output.
 * @param value The value associated with the input action.
 * @param type The type or classification of the value.
 * @param delay Delay in seconds before firing the output.
 */
extern "C" PLUGIN_API void FireEntityOutput(int entityHandle, const plg::string& outputName, int activatorHandle, int callerHandle, const plg::any& value, FieldType type, float delay) {
	auto* entity = helpers::GetEntity<CEntityInstance2>(entityHandle);
	if (!entity) return;
	CEntityInstance* activator = activatorHandle != INVALID_EHANDLE_INDEX ? g_pGameEntitySystem->GetEntityInstance(CEntityHandle(callerHandle)) : nullptr;
	CEntityInstance* caller = callerHandle != INVALID_EHANDLE_INDEX ? g_pGameEntitySystem->GetEntityInstance(CEntityHandle(callerHandle)) : nullptr;
	variant_t variant = helpers::GetVariant(value, type);
	entity->FireOutput(outputName.c_str(), activator ? activator->GetScriptInstance() : nullptr, caller ? caller->GetScriptInstance() : nullptr, variant, delay);
}

/**
 * @brief Redirects an entity output to call a function on another entity.
 *
 * @param entityHandle The handle of the entity whose output is being redirected.
 * @param output The name of the output to redirect.
 * @param functionName The function name to call on the target entity.
 * @param targetHandle The handle of the entity that will receive the output call.
 */
extern "C" PLUGIN_API void RedirectEntityOutput(int entityHandle, const plg::string& output, const plg::string& functionName, int targetHandle) {
	auto* entity = helpers::GetEntity<CEntityInstance2>(entityHandle);
	if (!entity) return;
	auto* target = helpers::GetEntity(targetHandle);
	if (!target) return;
	entity->RedirectOutput(output.c_str(), functionName.c_str(), target->GetScriptInstance());
}

/**
 * @brief Makes an entity follow another entity with optional bone merging.
 *
 * @param entityHandle The handle of the entity that will follow
 * @param attachmentHandle The handle of the entity to follow
 * @param boneMerge If true, bones will be merged between entities
 */
extern "C" PLUGIN_API void FollowEntity(int entityHandle, int attachmentHandle, bool boneMerge) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	auto* attach = helpers::GetEntity(attachmentHandle);
	if (!attach) return;
	entity->FollowEntity(attach->GetScriptInstance(), boneMerge);
}

/**
 * @brief Makes an entity follow another entity and merge with a specific bone or attachment.
 *
 * @param entityHandle The handle of the entity that will follow
 * @param attachmentHandle The handle of the entity to follow
 * @param boneOrAttachName Name of the bone or attachment point to merge with
 */
extern "C" PLUGIN_API void FollowEntityMerge(int entityHandle, int attachmentHandle, const plg::string& boneOrAttachName) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	auto* attach = helpers::GetEntity(attachmentHandle);
	if (!attach) return;
	entity->FollowEntity(attach->GetScriptInstance(), boneOrAttachName.c_str());
}

/**
 * @brief Apply damage to an entity.
 *
 * Creates a damage info object and applies damage to the specified entity.
 *
 * @param entityHandle The handle of the entity receiving damage
 * @param inflictorHandle The handle of the entity inflicting damage (e.g., projectile)
 * @param attackerHandle The handle of the attacking entity
 * @param force Direction and magnitude of force to apply
 * @param hitPos Position where the damage hit occurred
 * @param damage Amount of damage to apply
 * @param damageTypes Bitfield of damage type flags
 * @return Amount of damage actually applied to the entity
 */
extern "C" PLUGIN_API int TakeEntityDamage(int entityHandle, int inflictorHandle, int attackerHandle, const plg::vec3& force, const plg::vec3& hitPos, float damage, DamageTypes_t damageTypes) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return {};
	auto* inflictor = helpers::GetEntity(inflictorHandle);
	if (!inflictor) return {};
	auto* attacker = helpers::GetEntity(attackerHandle);
	if (!attacker) return {};
	HSCRIPT takeDamageInfo = CTakeDamage{}.CreateDamageInfo(inflictor->GetScriptInstance(), attacker->GetScriptInstance(), *reinterpret_cast<const Vector*>(&force), *reinterpret_cast<const Vector*>(&hitPos), damage, damageTypes);
	int applied = entity->TakeDamage(takeDamageInfo);
	CTakeDamage{}.DestroyDamageInfo(takeDamageInfo);
	return applied;
}

/**
 * @brief Retrieves a float attribute value from an entity.
 *
 * This function returns the float value of the specified entity attribute.
 * If the attribute does not exist or the entity is invalid, the provided default value is returned.
 *
 * @param entityHandle The handle of the entity.
 * @param name The name of the attribute.
 * @param defaultValue The default value to return if the attribute does not exist.
 * @return The float value of the attribute, or the default value if missing or invalid.
 */
extern "C" PLUGIN_API float GetEntityAttributeFloatValue(int entityHandle, const plg::string& name, float defaultValue) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return defaultValue;
	return entity->Attribute_GetFloatValue(name.c_str(), defaultValue);
}

/**
 * @brief Retrieves an integer attribute value from an entity.
 *
 * This function returns the integer value of the specified entity attribute.
 * If the attribute does not exist or the entity is invalid, the provided default value is returned.
 *
 * @param entityHandle The handle of the entity.
 * @param name The name of the attribute.
 * @param defaultValue The default value to return if the attribute does not exist.
 * @return The integer value of the attribute, or the default value if missing or invalid.
 */
extern "C" PLUGIN_API int GetEntityAttributeIntValue(int entityHandle, const plg::string& name, int defaultValue) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return defaultValue;
	return entity->Attribute_GetIntValue(name.c_str(), defaultValue);
}

/**
 * @brief Sets a float attribute value on an entity.
 *
 * This function sets the value of the specified float attribute for the entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity.
 * @param name The name of the attribute.
 * @param value The float value to assign to the attribute.
 */
extern "C" PLUGIN_API void SetEntityAttributeFloatValue(int entityHandle, const plg::string& name, float value) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->Attribute_SetFloatValue(name.c_str(), value);
}

/**
 * @brief Sets an integer attribute value on an entity.
 *
 * This function sets the value of the specified integer attribute for the entity.
 * If the entity is invalid, the function does nothing.
 *
 * @param entityHandle The handle of the entity.
 * @param name The name of the attribute.
 * @param value The integer value to assign to the attribute.
 */
extern "C" PLUGIN_API void SetEntityAttributeIntValue(int entityHandle, const plg::string& name, int value) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->Attribute_SetIntValue(name.c_str(), value);
}

/**
 * @brief Deletes an attribute from an entity.
 *
 * This function removes the specified attribute from the entity.
 * If the entity is invalid or the attribute does not exist, the function does nothing.
 *
 * @param entityHandle The handle of the entity.
 * @param name The name of the attribute to delete.
 */
extern "C" PLUGIN_API void DeleteEntityAttribute(int entityHandle, const plg::string& name) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return;
	entity->DeleteAttribute(name.c_str());
}

/**
 * @brief Checks if an entity has a specific attribute.
 *
 * This function determines whether the entity contains the specified attribute.
 * If the entity is invalid, it returns false.
 *
 * @param entityHandle The handle of the entity.
 * @param name The name of the attribute to check.
 * @return True if the attribute exists, false otherwise.
 */
extern "C" PLUGIN_API bool HasEntityAttribute(int entityHandle, const plg::string& name) {
	auto* entity = helpers::GetEntity(entityHandle);
	if (!entity) return false;
	return entity->HasAttribute(name.c_str());
}

//

PLUGIFY_WARN_POP()

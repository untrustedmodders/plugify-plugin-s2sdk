#include <core/event_manager.hpp>
#include <entity2/entitysystem.h>
#include <core/sdk/utils.hpp>
#include <core/sdk/helpers.hpp>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

/**
 * @brief Creates a hook for when a game event is fired.
 * @param name The name of the event to hook.
 * @param pCallback The callback function to call when the event is fired.
 * @param mode Whether the hook was in post mode (after processing) or pre mode (before processing).
 * @return An integer indicating the result of the hook operation.
 */
extern "C" PLUGIN_API EventHookError HookEvent(const plg::string& name, EventListenerCallback pCallback, HookMode mode) {
	return g_EventManager.HookEvent(name, pCallback, mode);
}

/**
 * @brief Removes a hook for when a game event is fired.
 * @param name The name of the event to unhook.
 * @param pCallback The callback function to remove.
 * @param mode Whether the hook was in post mode (after processing) or pre mode (before processing).
 * @return An integer indicating the result of the unhook operation.
 */
extern "C" PLUGIN_API EventHookError UnhookEvent(const plg::string& name, EventListenerCallback pCallback, HookMode mode) {
	return g_EventManager.UnhookEvent(name, pCallback, mode);
}

/**
 * @brief Creates a game event to be fired later.
 * @param name The name of the event to create.
 * @param force A boolean indicating whether to force the creation of the event.
 * @return A pointer to the created IGameEvent object.
 */
extern "C" PLUGIN_API IGameEvent* CreateEvent(const plg::string& name, bool force) {
	return g_EventManager.CreateEvent(name, force);
}

/**
 * @brief Fires a game event.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param bDontBroadcast A boolean indicating whether to broadcast the event.
 */
extern "C" PLUGIN_API void FireEvent(IGameEvent* event, bool bDontBroadcast) {
	if (event) g_EventManager.FireEvent(event, bDontBroadcast);
}

/**
 * @brief Fires a game event to a specific client.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param playerSlot The index of the player's slot to fire the event to.
 */
extern "C" PLUGIN_API void FireEventToClient(IGameEvent* event, int playerSlot) {
	if (event) g_EventManager.FireEventToClient(event, playerSlot);
}

/**
 * @brief Cancels a previously created game event that has not been fired.
 * @param event A pointer to the IGameEvent object of the event to cancel.
 */
extern "C" PLUGIN_API void CancelCreatedEvent(IGameEvent* event) {
	if (event) g_EventManager.CancelCreatedEvent(event);
}

/**
 * @brief Retrieves the boolean value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the boolean value.
 * @return The boolean value associated with the key.
 */
extern "C" PLUGIN_API bool GetEventBool(IGameEvent* event, const plg::string& key) {
	return event ? event->GetBool(GameEventKeySymbol_t::Make(key)) : false;
}

/**
 * @brief Retrieves the float value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the float value.
 * @return The float value associated with the key.
 */
extern "C" PLUGIN_API float GetEventFloat(IGameEvent* event, const plg::string& key) {
	return event ? event->GetFloat(GameEventKeySymbol_t::Make(key)) : 0;
}

/**
 * @brief Retrieves the integer value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the integer value.
 * @return The integer value associated with the key.
 */
extern "C" PLUGIN_API int GetEventInt(IGameEvent* event, const plg::string& key) {
	return event ? event->GetInt(GameEventKeySymbol_t::Make(key)) : 0;
}

/**
 * @brief Retrieves the long integer value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the long integer value.
 * @return The long integer value associated with the key.
 */
extern "C" PLUGIN_API uint64_t GetEventUInt64(IGameEvent* event, const plg::string& key) {
	return event ? event->GetUint64(GameEventKeySymbol_t::Make(key)) : 0;
}

/**
 * @brief Retrieves the string value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the string value.
 * @return A string where the result will be stored.
 */
extern "C" PLUGIN_API plg::string GetEventString(IGameEvent* event, const plg::string& key) {
	return event ? event->GetString(GameEventKeySymbol_t::Make(key)) : "";
}

/**
 * @brief Retrieves the pointer value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the pointer value.
 * @return The pointer value associated with the key.
 */
extern "C" PLUGIN_API void* GetEventPtr(IGameEvent* event, const plg::string& key) {
	return event ? event->GetPtr(GameEventKeySymbol_t::Make(key)) : nullptr;
}

/**
 * @brief Retrieves the player controller address of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the player controller address.
 * @return A pointer to the player controller associated with the key.
 */
extern "C" PLUGIN_API CEntityInstance* GetEventPlayerController(IGameEvent* event, const plg::string& key) {
	return event ? event->GetPlayerController(GameEventKeySymbol_t::Make(key)) : nullptr;
}

/**
 * @brief Retrieves the player index of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the player index.
 * @return The player index associated with the key.
 */
extern "C" PLUGIN_API int GetEventPlayerIndex(IGameEvent* event, const plg::string& key) {
	return event ? event->GetPlayerSlot(GameEventKeySymbol_t::Make(key)).Get() : INVALID_PLAYER_SLOT_INDEX;
}

/**
 * @brief Retrieves the player slot of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the player index.
 * @return The player slot associated with the key.
 */
extern "C" PLUGIN_API int GetEventPlayerSlot(IGameEvent* event, const plg::string& key) {
	return event ? event->GetPlayerSlot(GameEventKeySymbol_t::Make(key)).Get() : INVALID_PLAYER_SLOT_INDEX;
}

/**
 * @brief Retrieves the player pawn address of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the player pawn address.
 * @return A pointer to the player pawn associated with the key.
 */
extern "C" PLUGIN_API CEntityInstance* GetEventPlayerPawn(IGameEvent* event, const plg::string& key) {
	return event ? event->GetPlayerPawn(GameEventKeySymbol_t::Make(key)) : nullptr;
}

/**
 * @brief Retrieves the entity address of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the entity address.
 * @return A pointer to the entity associated with the key.
 */
extern "C" PLUGIN_API CEntityInstance* GetEventEntity(IGameEvent* event, const plg::string& key) {
	return event ? event->GetEntity(GameEventKeySymbol_t::Make(key)) : nullptr;
}

/**
 * @brief Retrieves the entity index of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the entity index.
 * @return The entity index associated with the key.
 */
extern "C" PLUGIN_API int GetEventEntityIndex(IGameEvent* event, const plg::string& key) {
	return event ? event->GetEntityIndex(GameEventKeySymbol_t::Make(key)).Get() : -1;
}

/**
 * @brief Retrieves the entity handle of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to retrieve the entity handle.
 * @return The entity handle associated with the key.
 */
extern "C" PLUGIN_API int GetEventEntityHandle(IGameEvent* event, const plg::string& key) {
	return event ? event->GetEHandle(GameEventKeySymbol_t::Make(key)).ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Retrieves the name of a game event.
 * @param event A pointer to the IGameEvent object containing event data.
 * @return A string where the result will be stored.
 */
extern "C" PLUGIN_API plg::string GetEventName(IGameEvent* event) {
	return event ? event->GetName() : "";
}

/**
 * @brief Sets the boolean value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the boolean value.
 * @param value The boolean value to set.
 */
extern "C" PLUGIN_API void SetEventBool(IGameEvent* event, const plg::string& key, bool value) {
	if (event) event->SetBool(GameEventKeySymbol_t::Make(key), value);
}

/**
 * @brief Sets the floating point value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the float value.
 * @param value The float value to set.
 */
extern "C" PLUGIN_API void SetEventFloat(IGameEvent* event, const plg::string& key, float value) {
	if (event) event->SetFloat(GameEventKeySymbol_t::Make(key), value);
}

/**
 * @brief Sets the integer value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the integer value.
 * @param value The integer value to set.
 */
extern "C" PLUGIN_API void SetEventInt(IGameEvent* event, const plg::string& key, int value) {
	if (event) event->SetInt(GameEventKeySymbol_t::Make(key), value);
}

/**
 * @brief Sets the long integer value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the long integer value.
 * @param value The long integer value to set.
 */
extern "C" PLUGIN_API void SetEventUInt64(IGameEvent* event, const plg::string& key, uint64_t value) {
	if (event) event->SetUint64(GameEventKeySymbol_t::Make(key), value);
}

/**
 * @brief Sets the string value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the string value.
 * @param value The string value to set.
 */
extern "C" PLUGIN_API void SetEventString(IGameEvent* event, const plg::string& key, const plg::string& value) {
	if (event) event->SetString(GameEventKeySymbol_t::Make(key), value.c_str());
}

/**
 * @brief Sets the pointer value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the pointer value.
 * @param value The pointer value to set.
 */
extern "C" PLUGIN_API void SetEventPtr(IGameEvent* event, const plg::string& key, void* value) {
	if (event) event->SetPtr(GameEventKeySymbol_t::Make(key), value);
}

/**
 * @brief Sets the player controller address of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the player controller address.
 * @param value A handle to the player controller to set.
 */
extern "C" PLUGIN_API void SetEventPlayerController(IGameEvent* event, const plg::string& key, int value) {
	auto* entity = helpers::GetEntity(value);
	if (!entity) return;
	if (event) event->SetPlayer(GameEventKeySymbol_t::Make(key), entity);
}

/**
 * @brief Sets the player index value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the player index value.
 * @param value The player index value to set.
 */
extern "C" PLUGIN_API void SetEventPlayerIndex(IGameEvent* event, const plg::string& key, int value) {
	if (event) event->SetPlayer(GameEventKeySymbol_t::Make(key), CPlayerSlot(value));
}

/**
 * @brief Sets the player slot value of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the player index value.
 * @param value The player slot value to set.
 */
extern "C" PLUGIN_API void SetEventPlayerSlot(IGameEvent* event, const plg::string& key, int value) {
	if (event) event->SetPlayer(GameEventKeySymbol_t::Make(key), CPlayerSlot(value));
}

/**
 * @brief Sets the entity address of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the entity address.
 * @param value A handle to the entity to set.
 */
extern "C" PLUGIN_API void SetEventEntity(IGameEvent* event, const plg::string& key, int value) {
	auto* entity = helpers::GetEntity(value);
	if (!entity) return;
	if (event) event->SetEntity(GameEventKeySymbol_t::Make(key), entity);
}

/**
 * @brief Sets the entity index of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the entity index.
 * @param value The entity index value to set.
 */
extern "C" PLUGIN_API void SetEventEntityIndex(IGameEvent* event, const plg::string& key, int value) {
	if (event) event->SetEntity(GameEventKeySymbol_t::Make(key), CEntityIndex(value));
}

/**
 * @brief Sets the entity handle of a game event's key.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param key The key for which to set the entity handle.
 * @param value The entity handle value to set.
 */
extern "C" PLUGIN_API void SetEventEntityHandle(IGameEvent* event, const plg::string& key, int value) {
	if (event) event->SetEntity(GameEventKeySymbol_t::Make(key), CEntityHandle(value).Get());
}

/**
 * @brief Sets whether an event's broadcasting will be disabled or not.
 * @param event A pointer to the IGameEvent object containing event data.
 * @param dontBroadcast A boolean indicating whether to disable broadcasting.
 */
extern "C" PLUGIN_API void SetEventBroadcast(IGameEvent* event, bool dontBroadcast) {
	if (event) event->SetBool("dont_broadcast", dontBroadcast);
}

/**
 * @brief Load game event descriptions from a file (e.g., "resource/gameevents.res").
 * @param path The path to the file containing event descriptions.
 * @param searchAll A boolean indicating whether to search all paths for the file.
 * @return An integer indicating the result of the loading operation.
 */
extern "C" PLUGIN_API int LoadEventsFromFile(const plg::string& path, bool searchAll) {
	return g_pGameEventManager->LoadEventsFromFile(path.c_str(), searchAll);
}

PLUGIFY_WARN_POP()

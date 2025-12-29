#include <core/game_config.hpp>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

/**
 * @brief Closes a game configuration file.
 *
 * This function closes the specified game configuration file.
 *
 * @param id An id to the GameConfig object to be closed.
 */
extern "C" PLUGIN_API void CloseGameConfigFile(uint32_t id) {
	g_GameConfigManager.UnloadConfig(id);
}

/**
 * @brief Loads a game configuration file.
 *
 * This function loads a game configuration file from the specified file path.
 *
 * @param paths The paths to the game configuration file to be loaded.
 * @return A handle to the loaded CGameConfig object, or nullptr if loading fails.
 */
extern "C" PLUGIN_API uint32_t LoadGameConfigFile(const plg::vector<plg::string>& paths) {
	auto config = g_GameConfigManager.LoadConfig({ paths });
	if (!config) {
		plg::print(LS_WARNING, "Could not load config: [{}] - {}\n", plg::join(paths, ", "), config.error());
		return 0;
	}
	return *config;
}

/**
 * @brief Retrieves a patch associated with the game configuration.
 *
 * This function retrieves a patch by name from the specified game configuration
 * and stores it in the provided output string.
 *
 * @param id An id to the GameConfig object from which to retrieve the patch.
 * @param name The name of the patch to be retrieved.
 * @return A string where the patch will be stored.
 */
extern "C" PLUGIN_API plg::string GetGameConfigPatch(uint32_t id, const plg::string& name) {
	if (auto gameConfig = g_GameConfigManager.GetConfig(id)) {
		if (auto patch = gameConfig->GetPatch(name)) {
			return *patch;
		} else {
			plg::print(LS_WARNING, "Could not get patch: {} - {}\n", name, patch.error());
		}
	} else {
		plg::print(LS_WARNING, "Could not get find config: {} - {}\n", id, name);
	}
	return {};
}

/**
 * @brief Retrieves the offset associated with a name from the game configuration.
 *
 * This function retrieves the offset of the specified name from the game configuration.
 *
 * @param id An id to the GameConfig object from which to retrieve the offset.
 * @param name The name whose offset is to be retrieved.
 * @return The offset associated with the specified name.
 */
extern "C" PLUGIN_API int GetGameConfigOffset(uint32_t id, const plg::string& name) {
	if (auto gameConfig = g_GameConfigManager.GetConfig(id)) {
		if (auto offset = gameConfig->GetOffset(name)) {
			return *offset;
		} else {
			plg::print(LS_WARNING, "Could not get offset: {} - {}\n", name, offset.error());
		}
	} else {
		plg::print(LS_WARNING, "Could not get find config: {} - {}\n", id, name);
	}
	return -1;
}

/**
 * @brief Retrieves the address associated with a name from the game configuration.
 *
 * This function retrieves the address of the specified name from the game configuration.
 *
 * @param id An id to the GameConfig object from which to retrieve the address.
 * @param name The name whose address is to be retrieved.
 * @return A pointer to the address associated with the specified name.
 */
extern "C" PLUGIN_API void* GetGameConfigAddress(uint32_t id, const plg::string& name) {
	if (auto gameConfig = g_GameConfigManager.GetConfig(id)) {
		if (auto address = gameConfig->GetAddress(name)) {
			return *address;
		} else {
			plg::print(LS_WARNING, "Could not get address: {} - {}\n", name, address.error());
		}
	} else {
		plg::print(LS_WARNING, "Could not get find config: {} - {}\n", id, name);
	}
	return nullptr;
}

/**
 * @brief Retrieves the vtable associated with a name from the game configuration.
 *
 * This function retrieves the vtable of the specified name from the game configuration.
 *
 * @param id An id to the GameConfig object from which to retrieve the vtable.
 * @param name The name whose vtable is to be retrieved.
 * @return A pointer to the vtable associated with the specified name.
 */
extern "C" PLUGIN_API void* GetGameConfigVTable(uint32_t id, const plg::string& name) {
	if (auto gameConfig = g_GameConfigManager.GetConfig(id)) {
		if (auto vtable = gameConfig->GetVTable(name)) {
			return *vtable;
		} else {
			plg::print(LS_WARNING, "Could not get vtable: {} - {}\n", name, vtable.error());
		}
	} else {
		plg::print(LS_WARNING, "Could not get find config: {} - {}\n", id, name);
	}
	return nullptr;
}

/**
 * @brief Retrieves the signature associated with a name from the game configuration.
 *
 * This function resolves and retrieves the signature of the specified name from the game configuration.
 *
 * @param id An id to the GameConfig object from which to retrieve the signature.
 * @param name The name whose signature is to be resolved and retrieved.
 * @return A pointer to the signature associated with the specified name.
 */
extern "C" PLUGIN_API void* GetGameConfigSignature(uint32_t id, const plg::string& name) {
	if (auto gameConfig = g_GameConfigManager.GetConfig(id)) {
		if (auto signature = gameConfig->GetSignature(name)) {
			return *signature;
		} else {
			plg::print(LS_WARNING, "Could not get signature: {} - {}\n", name, signature.error());
		}
	} else {
		plg::print(LS_WARNING, "Could not get find config: {} - {}\n", id, name);
	}
	return nullptr;
}

/**
 * @brief Retrieves a patch by scanning all loaded game configurations.
 *
 * This function scans all loaded game configurations and retrieves the first
 * patch that matches the specified name.
 *
 * @param name The name of the patch to be retrieved.
 * @return A string containing the patch, or an empty string if not found.
 */
extern "C" PLUGIN_API plg::string GetGameConfigPatchAll(const plg::string& name) {
	for (auto* gameConfig : g_GameConfigManager.GetConfigs()) {
		if (auto patch = gameConfig->GetPatch(name)) {
			return *patch;
		}
	}
	plg::print(LS_WARNING, "Could not find patch: {} in any loaded config\n", name);
	return {};
}

/**
 * @brief Retrieves an offset by scanning all loaded game configurations.
 *
 * This function scans all loaded game configurations and retrieves the first
 * offset that matches the specified name.
 *
 * @param name The name whose offset is to be retrieved.
 * @return The offset associated with the specified name, or -1 if not found.
 */
extern "C" PLUGIN_API int GetGameConfigOffsetAll(const plg::string& name) {
	for (auto* gameConfig : g_GameConfigManager.GetConfigs()) {
		if (auto offset = gameConfig->GetOffset(name)) {
			return *offset;
		}
	}
	plg::print(LS_WARNING, "Could not find offset: {} in any loaded config\n", name);
	return -1;
}

/**
 * @brief Retrieves an address by scanning all loaded game configurations.
 *
 * This function scans all loaded game configurations and retrieves the first
 * address that matches the specified name.
 *
 * @param name The name whose address is to be retrieved.
 * @return A pointer to the address associated with the specified name, or nullptr if not found.
 */
extern "C" PLUGIN_API void* GetGameConfigAddressAll(const plg::string& name) {
	for (auto* gameConfig : g_GameConfigManager.GetConfigs()) {
		if (auto address = gameConfig->GetAddress(name)) {
			return *address;
		}
	}
	plg::print(LS_WARNING, "Could not find address: {} in any loaded config\n", name);
	return nullptr;
}

/**
 * @brief Retrieves a vtable by scanning all loaded game configurations.
 *
 * This function scans all loaded game configurations and retrieves the first
 * vtable that matches the specified name.
 *
 * @param name The name whose vtable is to be retrieved.
 * @return A pointer to the vtable associated with the specified name, or nullptr if not found.
 */
extern "C" PLUGIN_API void* GetGameConfigVTableAll(const plg::string& name) {
	for (auto* gameConfig : g_GameConfigManager.GetConfigs()) {
		if (auto vtable = gameConfig->GetVTable(name)) {
			return *vtable;
		}
	}
	plg::print(LS_WARNING, "Could not find vtable: {} in any loaded config\n", name);
	return nullptr;
}

/**
 * @brief Retrieves a signature by scanning all loaded game configurations.
 *
 * This function scans all loaded game configurations and resolves the first
 * signature that matches the specified name.
 *
 * @param name The name whose signature is to be resolved and retrieved.
 * @return A pointer to the signature associated with the specified name, or nullptr if not found.
 */
extern "C" PLUGIN_API void* GetGameConfigSignatureAll(const plg::string& name) {
	for (auto* gameConfig : g_GameConfigManager.GetConfigs()) {
		if (auto signature = gameConfig->GetSignature(name)) {
			return *signature;
		}
	}
	plg::print(LS_WARNING, "Could not find signature: {} in any loaded config\n", name);
	return nullptr;
}

PLUGIFY_WARN_POP()

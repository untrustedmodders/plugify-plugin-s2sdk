#include <core/multi_addon_manager.hpp>

PLUGIFY_WARN_PUSH()

PLUGIFY_LINKAGE()

/**
 * @brief Adds a workshop addon that is mounted on the server and sent to all clients.
 *
 * Same as adding the addon to s2_extra_addons. If the addon is not downloaded yet,
 * the server downloads it and reloads the map.
 *
 * @param addon The workshop ID of the addon.
 * @param refresh If true, the addon is mounted right away, otherwise on the next map load.
 * @return True if the addon was added, false if it is already in the list.
 */
extern "C" PLUGIN_API bool AddServerAddon(uint64_t addon, bool refresh) {
#if defined (CS2)
	return g_MultiAddonManager.AddAddon(addon, refresh);
#else
	return false;
#endif
}

/**
 * @brief Removes a workshop addon from the server addons.
 *
 * @param addon The workshop ID of the addon.
 * @param refresh If true, the addon is unmounted right away, otherwise on the next map load.
 * @return True if the addon was removed, false if it is not in the list.
 */
extern "C" PLUGIN_API bool RemoveServerAddon(uint64_t addon, bool refresh) {
#if defined (CS2)
	return g_MultiAddonManager.RemoveAddon(addon, refresh);
#else
	return false;
#endif
}

/**
 * @brief Retrieves the workshop addons that are mounted on the server and sent to all clients.
 *
 * @return An array of workshop IDs from s2_extra_addons.
 */
extern "C" PLUGIN_API plg::vector<uint64_t> GetServerAddons() {
#if defined (CS2)
	const auto& addons = g_MultiAddonManager.m_extraAddons;
	return { addons.begin(), addons.end() };
#else
	return {};
#endif
}

/**
 * @brief Adds a workshop addon that is mounted on the server but sent only to the clients passed to SendAddon.
 *
 * Same as adding the addon to s2_private_addons. If the addon is not downloaded yet,
 * the server downloads it and reloads the map.
 *
 * @param addon The workshop ID of the addon.
 * @param refresh If true, the addon is mounted right away, otherwise on the next map load.
 * @return True if the addon was added, false if it is already in the list.
 */
extern "C" PLUGIN_API bool AddPrivateAddon(uint64_t addon, bool refresh) {
#if defined (CS2)
	return g_MultiAddonManager.AddPrivateAddon(addon, refresh);
#else
	return false;
#endif
}

/**
 * @brief Removes a workshop addon from the private addons.
 *
 * @param addon The workshop ID of the addon.
 * @param refresh If true, the addon is unmounted right away, otherwise on the next map load.
 * @return True if the addon was removed, false if it is not in the list.
 */
extern "C" PLUGIN_API bool RemovePrivateAddon(uint64_t addon, bool refresh) {
#if defined (CS2)
	return g_MultiAddonManager.RemovePrivateAddon(addon, refresh);
#else
	return false;
#endif
}

/**
 * @brief Retrieves the workshop addons that are mounted on the server but sent only to selected clients.
 *
 * @return An array of workshop IDs from s2_private_addons.
 */
extern "C" PLUGIN_API plg::vector<uint64_t> GetPrivateAddons() {
#if defined (CS2)
	const auto& addons = g_MultiAddonManager.m_privateAddons;
	return { addons.begin(), addons.end() };
#else
	return {};
#endif
}

/**
 * @brief Adds a workshop addon that is sent to all clients but not mounted on the server.
 *
 * Same as adding the addon to s2_client_extra_addons. Use it only for content the server
 * never loads itself, e.g. replacements of the base game files.
 *
 * @param addon The workshop ID of the addon.
 * @param refresh If true, connected clients that do not have the addon yet are told to download it right away.
 * @return True if the addon was added, false if it is already in the list.
 */
extern "C" PLUGIN_API bool AddClientOnlyAddon(uint64_t addon, bool refresh) {
#if defined (CS2)
	return g_MultiAddonManager.AddClientAddon(addon, 0, refresh);
#else
	return false;
#endif
}

/**
 * @brief Removes a workshop addon from the client only addons.
 *
 * Clients that already mounted the addon keep it until they reconnect.
 *
 * @param addon The workshop ID of the addon.
 * @return True if the addon was removed, false if it is not in the list.
 */
extern "C" PLUGIN_API bool RemoveClientOnlyAddon(uint64_t addon) {
#if defined (CS2)
	return g_MultiAddonManager.RemoveClientAddon(addon, 0);
#else
	return false;
#endif
}

/**
 * @brief Retrieves the workshop addons that are sent to all clients but not mounted on the server.
 *
 * @return An array of workshop IDs from s2_client_extra_addons.
 */
extern "C" PLUGIN_API plg::vector<uint64_t> GetClientOnlyAddons() {
#if defined (CS2)
	const auto& addons = g_MultiAddonManager.m_globalClientAddons;
	return { addons.begin(), addons.end() };
#else
	return {};
#endif
}

/**
 * @brief Sends a workshop addon to a specific client.
 *
 * Use it for private addons, or for client only addons that only this client should get.
 * The addon stays sent to this SteamID64 across reconnects and map changes until it is removed
 * with RemoveSentAddon or ClearSentAddons, or the server restarts, so it is enough to call this once,
 * e.g. when the client gets the rights for it.
 *
 * @param steamId The SteamID64 of the client.
 * @param addon The workshop ID of the addon.
 * @param refresh If true and the client is connected, it is told to download the addon right away, otherwise it gets the addon on the next connect.
 * @return True if the addon was sent, false if the client already has it or the SteamID64 is 0.
 */
extern "C" PLUGIN_API bool SendAddon(uint64_t steamId, uint64_t addon, bool refresh) {
#if defined (CS2)
	if (!steamId)
		return false;

	return g_MultiAddonManager.AddClientAddon(addon, steamId, refresh);
#else
	return false;
#endif
}

/**
 * @brief Removes a workshop addon sent to a specific client.
 *
 * The client keeps the mounted addon until it reconnects.
 *
 * @param steamId The SteamID64 of the client.
 * @param addon The workshop ID of the addon.
 * @return True if the addon was removed, false if it was not sent to the client.
 */
extern "C" PLUGIN_API bool RemoveSentAddon(uint64_t steamId, uint64_t addon) {
#if defined (CS2)
	if (!steamId)
		return false;

	return g_MultiAddonManager.RemoveClientAddon(addon, steamId);
#else
	return false;
#endif
}

/**
 * @brief Removes all workshop addons sent to a specific client.
 *
 * The client keeps the mounted addons until it reconnects.
 *
 * @param steamId The SteamID64 of the client.
 */
extern "C" PLUGIN_API void ClearSentAddons(uint64_t steamId) {
#if defined (CS2)
	if (!steamId)
		return;

	g_MultiAddonManager.ClearClientAddons(steamId);
#endif
}

/**
 * @brief Retrieves the workshop addons sent to a specific client.
 *
 * @param steamId The SteamID64 of the client.
 * @return An array of workshop IDs sent with SendAddon or s2_sent_addons.
 */
extern "C" PLUGIN_API plg::vector<uint64_t> GetSentAddons(uint64_t steamId) {
#if defined (CS2)
	if (!steamId)
		return {};

	auto addons = g_MultiAddonManager.GetSentAddons(steamId);
	return { addons.begin(), addons.end() };
#else
	return {};
#endif
}

/**
 * @brief Retrieves all workshop addons a client downloads and mounts when it connects.
 *
 * Includes the current workshop map, the server addons, the client only addons
 * and the addons sent to this client.
 *
 * @param steamId The SteamID64 of the client.
 * @return An array of workshop IDs in the order the client mounts them.
 */
extern "C" PLUGIN_API plg::vector<uint64_t> GetClientAddons(uint64_t steamId) {
#if defined (CS2)
	auto addons = g_MultiAddonManager.GetClientAddons(steamId);
	return { addons.begin(), addons.end() };
#else
	return {};
#endif
}

/**
 * @brief Retrieves the workshop ID of the current map.
 *
 * @return The workshop ID of the current map, or 0 if the current map is not a workshop map.
 */
extern "C" PLUGIN_API uint64_t GetCurrentWorkshopMap() {
#if defined (CS2)
	return g_MultiAddonManager.GetCurrentWorkshopMap();
#else
	return 0;
#endif
}

PLUGIFY_WARN_POP()

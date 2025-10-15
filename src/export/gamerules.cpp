#include <plugin_export.h>

#include <core/sdk/entity/cgamerules.h>
#include <core/sdk/entity/cteam.h>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

extern CBaseGameRulesProxy* g_pGameRulesProxy;
extern CBaseGameRules* g_pGameRules;

/**
 * @brief Retrieves the pointer to the current game rules proxy entity instance.
 *
 * This function provides access to the global game rules object,
 * which contains various methods and properties defining the
 * rules and logic for the game.
 *
 * @return A pointer to the game rules proxy entity instance.
 */
extern "C" PLUGIN_API void* GetGameRulesProxy() {
	return g_pGameRulesProxy;
}

/**
 * @brief Retrieves the pointer to the current game rules instance.
 *
 * This function provides access to the global game rules object,
 * which contains various methods and properties defining the
 * rules and logic for the game.
 *
 * @return A pointer to the game rules object.
 */
extern "C" PLUGIN_API void* GetGameRules() {
	return g_pGameRules;
}

/**
 * @brief Retrieves the team manager instance for a specified team.
 *
 * This function looks up the internal team manager object associated with the
 * given team identifier. If the specified team cannot be found in the global
 * team manager registry, a warning is logged and a null pointer is returned.
 *
 * @param team The numeric identifier of the team.
 * @return A pointer to the corresponding CTeam instance, or nullptr if the team was not found.
 */
extern "C" PLUGIN_API CTeam* GetGameTeamManager(int team) {
	auto it = g_pTeamManagers.find(team);
	if (it == g_pTeamManagers.end()) {
		plg::print(LS_WARNING, "Failed to find \"{}\" team\n", team);
		return nullptr;
	}

	return std::get<CTeam*>(*it);
}

/**
 * @brief Retrieves the current score of a specified team.
 *
 * This function looks up the team manager associated with the provided team ID
 * and returns its current score. If the team cannot be found, a warning is logged
 * and -1 is returned.
 *
 * @param team The numeric identifier of the team.
 * @return The current score of the team, or -1 if the team could not be found.
 */
extern "C" PLUGIN_API int GetGameTeamScore(int team) {
	auto it = g_pTeamManagers.find(team);
	if (it == g_pTeamManagers.end()) {
		plg::print(LS_WARNING, "Failed to find \"{}\" team\n", team);
		return -1;
	}

	return std::get<CTeam*>(*it)->m_iScore;
}

/**
 * @brief Retrieves the number of players on a specified team.
 *
 * This function queries the active game rules instance to determine how many
 * players currently belong to the specified team (Terrorists, Counter-Terrorists,
 * or Spectators). Returns -1 if the game rules object is not yet available.
 *
 * @param team The numeric identifier of the team (e.g., CS_TEAM_T, CS_TEAM_CT, CS_TEAM_SPECTATOR).
 * @return The number of players on the team, or -1 if game rules are unavailable.
 */
extern "C" PLUGIN_API int GetGamePlayerCount(int team) {
	if (g_pGameRules == nullptr) {
		plg::print(LS_WARNING, "cs_gamerules not instantiated yet.\n");
		return -1;
	}

	switch (team) {
		case CS_TEAM_T:
			return g_pGameRules->m_iNumTerrorist;
		case CS_TEAM_CT:
			return g_pGameRules->m_iNumCT;
		case CS_TEAM_SPECTATOR:
			return g_pGameRules->m_iSpectatorSlotCount;
		default:
			return 0;
	}
}

/**
 * @brief Returns the total number of rounds played in the current match.
 *
 * This function queries the game rules object to retrieve the total count
 * of rounds that have been completed since the match started. If the game
 * rules object has not been created yet, a warning is logged and -1 is returned.
 *
 * @return The total number of rounds played, or -1 if the game rules are unavailable.
 */
extern "C" PLUGIN_API int GetGameTotalRoundsPlayed() {
	if (g_pGameRules == nullptr) {
		plg::print(LS_WARNING, "cs_gamerules not instantiated yet.\n");
		return -1;
	}

	return g_pGameRules->m_totalRoundsPlayed;
}

/**
 * @brief Forces the round to end with a specified reason and delay.
 *
 * This function allows the caller to end the current round, specifying
 * both the reason for the termination and a delay (in seconds) before the
 * next round begins. If the game rules object is not yet available, the
 * request is ignored and a warning is logged.
 *
 * @param delay Time in seconds to wait before the next round starts.
 * @param reason The reason for ending the round, as defined by the CSRoundEndReason enum.
 */
extern "C" PLUGIN_API void TerminateRound(float delay, CSRoundEndReason reason) {
	if (g_pGameRules == nullptr) {
		plg::print(LS_WARNING, "cs_gamerules not instantiated yet.\n");
		return;
	}

	g_pGameRules->TerminateRound(delay, reason);
}

PLUGIFY_WARN_POP()

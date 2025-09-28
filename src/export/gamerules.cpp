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
 *
 * @param team
 * @return
 */
extern "C" PLUGIN_API int GetGameTeamScore(int team) {
	auto it = g_pTeamManagers.find(team);
	if (it == g_pTeamManagers.end()) {
		S2_LOGF(LS_WARNING,  "Failed to find \"{}\" team\n", team);
		return -1;
	}

	return std::get<CTeam*>(*it)->m_iScore;
}

/**
 *
 * @param team
 * @return
 */
extern "C" PLUGIN_API int GetGamePlayerCount(int team) {
	if (g_pGameRules == nullptr) {
		S2_LOG(LS_WARNING, "cs_gamerules not instantiated yet.\n");
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
 *
 * @return
 */
extern "C" PLUGIN_API int GetGameTotalRoundsPlayed() {
	if (g_pGameRules == nullptr) {
		S2_LOG(LS_WARNING, "cs_gamerules not instantiated yet.\n");
		return -1;
	}

	return g_pGameRules->m_totalRoundsPlayed;
}

/**
 * @brief Forces the round to end with a specified reason and delay.
 *
 * This function allows the caller to end the current round, specifying
 * the reason for the termination and the time delay before a new round begins.
 *
 * @param delay Time (in seconds) to delay before the next round starts.
 * @param reason The reason for ending the round, defined by the CSRoundEndReason enum.
 */
extern "C" PLUGIN_API void TerminateRound(float delay, CSRoundEndReason reason) {
	if (g_pGameRules == nullptr) {
		S2_LOG(LS_WARNING, "cs_gamerules not instantiated yet.\n");
		return;
	}

	g_pGameRules->TerminateRound(delay, reason);
}

PLUGIFY_WARN_POP()

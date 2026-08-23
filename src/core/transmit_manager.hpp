#pragma once
#include <iservernetworkable.h>

class TransmitManager {
	TransmitManager() = default;
	~TransmitManager() = default;
	NONCOPYABLE(TransmitManager)

	static TransmitManager instance;
public:
	static auto& Instance() noexcept {
		return instance;
	}

	void OnCheckTransmit(const plg::vector<CCheckTransmitInfo*>& transmitList);

	// Force a just-spawned player pawn to transmit for the next CheckTransmit pass,
	// so the client can build its scene node before it may be hidden again. Hiding a
	// pawn on the spawn tick crashes nearby clients.
	void MarkRecentlySpawned(int entHandle);

	void HideEntities(int playerSlot, std::span<const int> entHandles);
	void ShowEntities(int playerSlot, std::span<const int> entHandles);

	void HideEntityFromOtherPlayers(int playerSlot, int entHandle);
	void ShowEntityToOtherPlayers(int playerSlot, int entHandle);

	void RoundStart();
	plg::vector<int> GetHiddenEntities(int playerSlot);

private:
	plg::flat_hash_map<int, plg::flat_hash_set<int>> m_playerHiddenEntities;
	// Pawns that spawned since the last CheckTransmit pass; shown for one tick.
	plg::flat_hash_set<int> m_recentlySpawned;
	//std::mutex m_mutex;
};
inline TransmitManager& g_TransmitManager = TransmitManager::Instance();

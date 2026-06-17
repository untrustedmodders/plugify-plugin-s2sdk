#pragma once
#include <iservernetworkable.h>

class TransmitManager {
public:
	static auto& Instance() {
		static TransmitManager instance;
		return instance;
	}

	bool ShouldHide(int32_t playerSlot, CEntityInstance* entity) const;

	void HideEntities(int32_t playerSlot, std::span<const int32_t> entHandles);
	void ShowEntities(int32_t playerSlot, std::span<const int32_t> entHandles);

	void HideEntityFromOtherPlayers(int32_t playerSlot, int32_t entHandle);
	void ShowEntityToOtherPlayers(int32_t playerSlot, int32_t entHandle);

	void RoundStart();
	plg::vector<int32_t> GetHiddenEntities(int32_t playerSlot);

private:
	plg::flat_hash_map<int32_t, plg::flat_hash_set<int32_t>> m_playerHiddenEntities;
};
inline TransmitManager& g_TransmitManager = TransmitManager::Instance();

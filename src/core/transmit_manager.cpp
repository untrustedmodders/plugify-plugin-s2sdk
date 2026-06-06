#include "transmit_manager.hpp"

#include "event_listener.hpp"
#include "globals.hpp"
#include "sdk/helpers.hpp"

void TransmitManger::OnCheckTransmit(const plg::vector<CCheckTransmitInfo*>& transmitList) {
	if (m_playerHiddenEntities.empty()) {
		return;
	}

	for (auto* info : transmitList) {
		if (!info || !info->m_pTransmitEntity) {
			continue;
		}

		auto it = m_playerHiddenEntities.find(info->m_nPlayerSlot);
		if (it == m_playerHiddenEntities.end()) {
			continue;
		}

		for (const int32_t handle : it->second) {
			auto* entity = helpers::GetEntity(handle);
			if (!entity) {
				continue;
			}
			info->m_pTransmitEntity->Clear(entity->entindex());
		}
	}
}

void TransmitManger::RoundStart() {
	m_playerHiddenEntities.clear();
}

void TransmitManger::HideEntities(int32_t playerSlot, std::span<const int32_t> entHandles) {
	auto& hidden = m_playerHiddenEntities[playerSlot];
	for (const int32_t handle : entHandles) {
		hidden.insert(handle);
	}
}

void TransmitManger::ShowEntities(int32_t playerSlot, std::span<const int32_t> entHandles) {
	auto it = m_playerHiddenEntities.find(playerSlot);
	if (it == m_playerHiddenEntities.end()) {
		return;
	}

	auto& hidden = it->second;
	for (const int32_t handle : entHandles) {
		hidden.erase(handle);
	}

	if (hidden.empty()) {
		m_playerHiddenEntities.erase(it);
	}
}

plg::vector<int32_t> TransmitManger::GetHiddenEntities(int32_t playerSlot) {
	auto it = m_playerHiddenEntities.find(playerSlot);
	if (it == m_playerHiddenEntities.end()) {
		return {};
	}

	return plg::vector<int32_t>(it->second.begin(), it->second.end());
}

void TransmitManger::HideEntityFromOtherPlayers(int32_t playerSlot, int32_t entHandle) {
	for (int32_t slot = 0; slot < MaxPlayers; ++slot) {
		if (slot == playerSlot) {
			continue;
		}

		m_playerHiddenEntities[slot].insert(entHandle);
	}
}

void TransmitManger::ShowEntityToOtherPlayers(int32_t playerSlot, int32_t entHandle) {
	for (int32_t slot = 0; slot < MaxPlayers; ++slot) {
		if (slot == playerSlot) {
			continue;
		}

		auto it = m_playerHiddenEntities.find(slot);
		if (it == m_playerHiddenEntities.end()) {
			continue;
		}

		it->second.erase(entHandle);
		if (it->second.empty()) {
			m_playerHiddenEntities.erase(it);
		}
	}
}
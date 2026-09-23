#include "transmit_manager.hpp"

#include "event_listener.hpp"
#include "globals.hpp"
#include "sdk/helpers.hpp"

TransmitManager TransmitManager::instance;

void TransmitManager::OnCheckTransmit(const plg::vector<CCheckTransmitInfo*>& transmitList) {
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

		for (const int handle : it->second) {
			auto* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(handle));
			if (!entity) {
				continue;
			}

			info->m_pTransmitEntity->Clear(entity->GetEntityIndex());

			// Mark the hidden entity as "exists but not transmitted" (dont_transmit list).
			// Without it the client's reconcile pass deletes the entity, while the server
			// still counts it as held and later sends a delta for it -- the client dies with
			// "CopyExistingEntity: missing client entity". Same fix as CS2Fixes 2a7db489.
			if (info->m_pTransmitNonPlayers) {
				info->m_pTransmitNonPlayers->Set(entity->GetEntityIndex());
			}
		}
	}
}

void TransmitManager::RoundStart() {
	m_playerHiddenEntities.clear();
}

void TransmitManager::HideEntities(int playerSlot, std::span<const int> entHandles) {
	auto& hidden = m_playerHiddenEntities[playerSlot];
	for (const int handle : entHandles) {
		hidden.insert(handle);
	}
}

void TransmitManager::ShowEntities(int playerSlot, std::span<const int> entHandles) {
	auto it = m_playerHiddenEntities.find(playerSlot);
	if (it == m_playerHiddenEntities.end()) {
		return;
	}

	auto& hidden = it->second;
	for (const int handle : entHandles) {
		hidden.erase(handle);
	}

	if (hidden.empty()) {
		m_playerHiddenEntities.erase(it);
	}
}

plg::vector<int> TransmitManager::GetHiddenEntities(int playerSlot) {
	auto it = m_playerHiddenEntities.find(playerSlot);
	if (it == m_playerHiddenEntities.end()) {
		return {};
	}

	return plg::vector<int>(it->second.begin(), it->second.end());
}

void TransmitManager::HideEntityFromOtherPlayers(int playerSlot, int entHandle) {
	for (int slot = 0; slot < MaxPlayers; ++slot) {
		if (slot == playerSlot) {
			continue;
		}

		m_playerHiddenEntities[slot].insert(entHandle);
	}
}

void TransmitManager::ShowEntityToOtherPlayers(int playerSlot, int entHandle) {
	for (int slot = 0; slot < MaxPlayers; ++slot) {
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
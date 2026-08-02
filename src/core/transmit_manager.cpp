#include "transmit_manager.hpp"

#include "event_listener.hpp"
#include "globals.hpp"
#include "sdk/helpers.hpp"

TransmitManager TransmitManager::instance;

void TransmitManager::OnCheckTransmit(const plg::vector<CCheckTransmitInfo*>& transmitList) {
	if (m_playerHiddenEntities.empty()) {
		m_recentlySpawned.clear();
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
			// A just-spawned pawn must transmit for a tick so the client can build
			// its scene node before it is hidden again; hiding it on the spawn tick
			// crashes nearby clients.
			if (m_recentlySpawned.contains(handle)) {
				continue;
			}

			auto* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(handle));
			if (!entity) {
				continue;
			}

			// Reveal a dead player pawn: keeping it hidden while the client builds
			// the death ragdoll races the CharacterDecalRenderer and crashes nearby
			// clients. Hiding resumes on respawn (with the one-tick spawn grace).
			auto* baseEntity = static_cast<CBaseEntity*>(entity);
			if (baseEntity->IsPlayerPawn() && baseEntity->m_lifeState != LIFE_ALIVE) {
				continue;
			}

			info->m_pTransmitEntity->Clear(entity->GetEntityIndex());
		}
	}

	// Grace consumed for this tick; hidden pawns resume hiding next pass.
	m_recentlySpawned.clear();
}

void TransmitManager::MarkRecentlySpawned(int32_t entHandle) {
	m_recentlySpawned.insert(entHandle);
}

void TransmitManager::RoundStart() {
	m_playerHiddenEntities.clear();
	m_recentlySpawned.clear();
}

void TransmitManager::HideEntities(int32_t playerSlot, std::span<const int32_t> entHandles) {
	auto& hidden = m_playerHiddenEntities[playerSlot];
	for (const int32_t handle : entHandles) {
		hidden.insert(handle);
	}
}

void TransmitManager::ShowEntities(int32_t playerSlot, std::span<const int32_t> entHandles) {
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

plg::vector<int32_t> TransmitManager::GetHiddenEntities(int32_t playerSlot) {
	auto it = m_playerHiddenEntities.find(playerSlot);
	if (it == m_playerHiddenEntities.end()) {
		return {};
	}

	return plg::vector<int32_t>(it->second.begin(), it->second.end());
}

void TransmitManager::HideEntityFromOtherPlayers(int32_t playerSlot, int32_t entHandle) {
	for (int32_t slot = 0; slot < MaxPlayers; ++slot) {
		if (slot == playerSlot) {
			continue;
		}

		m_playerHiddenEntities[slot].insert(entHandle);
	}
}

void TransmitManager::ShowEntityToOtherPlayers(int32_t playerSlot, int32_t entHandle) {
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
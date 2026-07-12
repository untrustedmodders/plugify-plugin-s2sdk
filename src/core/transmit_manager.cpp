#include "transmit_manager.hpp"

#include "game_config.hpp"
#include "globals.hpp"
#include "hook_manager.hpp"
#include "sdk/entity/cbaseentity.h"
#include "sdk/helpers.hpp"

TransmitManager TransmitManager::instance;

namespace {
	constexpr std::string_view kSetTransmitName = "CBaseEntity::SetTransmit";
	using SetTransmitFn = void(CBaseEntity*, CCheckTransmitInfo*, bool);
}

polyhook::ResultType TransmitManager::OnSetTransmit(polyhook::HookHandle, polyhook::ParametersHandle params, int, polyhook::ReturnHandle, polyhook::CallbackType) {
	auto& self = Instance();
	if (self.m_playerHiddenEntities.empty()) {
		return polyhook::ResultType::Ignored;
	}

	auto* entity = polyhook::GetArgument<CBaseEntity*>(params, 0);
	auto* info = polyhook::GetArgument<CCheckTransmitInfo*>(params, 1);
	if (!entity || !info) {
		return polyhook::ResultType::Ignored;
	}

	auto it = self.m_playerHiddenEntities.find(info->m_nPlayerSlot.Get());
	if (it == self.m_playerHiddenEntities.end()) {
		return polyhook::ResultType::Ignored;
	}

	const int32_t handle = entity->GetRefEHandle().ToInt();
	if (!it->second.contains(handle)) {
		return polyhook::ResultType::Ignored;
	}

	// A just-spawned pawn must transmit for a tick so the client can build its
	// scene node before it is hidden again; hiding it on the spawn tick crashes
	// nearby clients.
	if (self.m_recentlySpawned.contains(handle)) {
		return polyhook::ResultType::Ignored;
	}

	return polyhook::ResultType::Supercede;
}

void TransmitManager::MarkRecentlySpawned(int32_t entHandle) {
	m_recentlySpawned.insert(entHandle);
}

void TransmitManager::ClearRecentlySpawned() {
	m_recentlySpawned.clear();
}

void TransmitManager::UpdateActiveState() {
	const bool shouldBeActive = !m_playerHiddenEntities.empty();
	if (shouldBeActive == m_active) {
		return;
	}

	if (shouldBeActive) {
		auto address = g_pGameConfig->GetAddress(kSetTransmitName);
		if (!address) {
			plg::print(LS_WARNING, "Failed to resolve CBaseEntity::SetTransmit: {}\n", address.error());
			return;
		}
		auto result = g_HookManager.AddHookDetourFunc<SetTransmitFn>(kSetTransmitName, static_cast<uintptr_t>(*address), &TransmitManager::OnSetTransmit, {polyhook::CallbackType::Pre});
		if (!result) {
			plg::print(LS_WARNING, "Failed to hook CBaseEntity::SetTransmit: {}\n", result.error());
			return;
		}
		m_active = true;
	} else {
		g_HookManager.RemoveHookDetourFunc(kSetTransmitName);
		m_active = false;
	}
}

void TransmitManager::RoundStart() {
	m_playerHiddenEntities.clear();
	m_recentlySpawned.clear();
	UpdateActiveState();
}

void TransmitManager::HideEntities(int32_t playerSlot, std::span<const int32_t> entHandles) {
	auto& hidden = m_playerHiddenEntities[playerSlot];
	for (const int32_t handle : entHandles) {
		hidden.insert(handle);
	}
	UpdateActiveState();
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
	UpdateActiveState();
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
	UpdateActiveState();
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
	UpdateActiveState();
}

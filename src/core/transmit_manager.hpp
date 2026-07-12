#pragma once
#include <iservernetworkable.h>
#include <polyhook/polyhook.hpp>

class CEntityInstance;

// Hides entities from specific players by detouring the base implementation of
// CBaseEntity::SetTransmit(CCheckTransmitInfo*, bool). That function sets the
// entity's bit in CCheckTransmitInfo::m_pTransmitEntity, and every class override
// calls into it, so a single detour covers every entity. When a hidden entity is
// asked to add itself to a recipient's transmit list, the Pre-callback supercedes
// the original so the entity is never networked to that client.
//
// The detour is installed only while at least one entity is hidden, so servers
// that never hide anything pay no cost.
class TransmitManager {
	TransmitManager() = default;
	~TransmitManager() = default;
	NONCOPYABLE(TransmitManager)

	static TransmitManager instance;
public:
	static auto& Instance() noexcept {
		return instance;
	}

	void HideEntities(int32_t playerSlot, std::span<const int32_t> entHandles);
	void ShowEntities(int32_t playerSlot, std::span<const int32_t> entHandles);

	void HideEntityFromOtherPlayers(int32_t playerSlot, int32_t entHandle);
	void ShowEntityToOtherPlayers(int32_t playerSlot, int32_t entHandle);

	void RoundStart();
	plg::vector<int32_t> GetHiddenEntities(int32_t playerSlot);

	// Force an entity (a just-spawned player pawn) to transmit until the next
	// CheckTransmit pass, so the client can build its scene node before it may be
	// hidden again. Hiding a pawn on the spawn tick crashes nearby clients.
	void MarkRecentlySpawned(int32_t entHandle);
	void ClearRecentlySpawned();

	// Drop a deleted entity from every hidden set so its handle can't linger (or be
	// matched by a reused handle), and uninstall the detour once nothing is hidden.
	void OnEntityDeleted(int32_t entHandle);

	// Pre-callback for the detoured CBaseEntity::SetTransmit.
	static polyhook::ResultType OnSetTransmit(polyhook::HookHandle hook, polyhook::ParametersHandle params, int count, polyhook::ReturnHandle ret, polyhook::CallbackType type);

private:
	// Installs/removes the SetTransmit detour based on whether anything is hidden.
	void UpdateActiveState();

	plg::flat_hash_map<int32_t, plg::flat_hash_set<int32_t>> m_playerHiddenEntities;
	// Pawns that spawned since the last CheckTransmit pass; shown for one tick.
	plg::flat_hash_set<int32_t> m_recentlySpawned;
	bool m_active = false;
	//std::mutex m_mutex;
};
inline TransmitManager& g_TransmitManager = TransmitManager::Instance();

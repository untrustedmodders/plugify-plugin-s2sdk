#include "event_listener.hpp"

plg::hybrid_vector<CGameEventListener*, 64> g_vecEventListeners;
bool g_registered = false;

void RegisterEventListeners() {
    if (!g_pGameEventManager || g_registered)
        return;

    for (const auto& event : g_vecEventListeners) {
        g_pGameEventManager->AddListener(event, event->GetEventName().data(), true);
    }

    g_registered = true;
}

void UnregisterEventListeners() {
    if (!g_pGameEventManager || !g_registered)
        return;

    for (const auto& event : g_vecEventListeners) {
        g_pGameEventManager->RemoveListener(event);
    }

    g_vecEventListeners.clear();

	g_registered = false;
}
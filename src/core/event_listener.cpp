#include "event_listener.hpp"

plg::hybrid_vector<CGameEventListener*, 64> g_vecEventListeners;

void RegisterEventListeners() {
    static bool registered = false;

    if (registered || !g_pGameEventManager)
        return;

    for (const auto& event : g_vecEventListeners) {
        g_pGameEventManager->AddListener(event, event->GetEventName(), true);
    }

    registered = true;
}

void UnregisterEventListeners() {
    if (!g_pGameEventManager)
        return;

    for (const auto& event : g_vecEventListeners) {
        g_pGameEventManager->RemoveListener(event);
    }

    g_vecEventListeners.clear();
}
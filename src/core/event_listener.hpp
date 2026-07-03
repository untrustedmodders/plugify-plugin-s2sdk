#pragma once

#include <igameevents.h>

using GameEventListenerCallback = void (*)(IGameEvent* event);

class CGameEventListener;

extern std::vector<CGameEventListener*> g_vecEventListeners;

class CGameEventListener : public IGameEventListener2 {
public:
	CGameEventListener(GameEventListenerCallback callback, std::string_view eventName) :
		m_callback(callback), m_eventName(eventName) {
		g_vecEventListeners.emplace_back(this);
	}
	~CGameEventListener() override = default;
	NONCOPYABLE(CGameEventListener)

	// FireEvent is called by EventManager if event just occured
	// KeyValue memory will be freed by manager if not needed anymore
	void FireGameEvent(IGameEvent* event) override {
		m_callback(event);
	}

	std::string_view GetEventName() const { return m_eventName; }

private:
	GameEventListenerCallback m_callback;
	std::string_view m_eventName;
};

void RegisterEventListeners();
void UnregisterEventListeners();

#define GAME_EVENT_F(_event)                                          \
	void _event##_callback(IGameEvent*);                              \
	CGameEventListener _event##_listener(_event##_callback, #_event); \
	void _event##_callback(IGameEvent* event)

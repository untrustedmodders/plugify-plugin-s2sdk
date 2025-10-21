#pragma once

#include "listener_manager.hpp"
#include <igameevents.h>

struct EventInfo {
	IGameEvent* event{};
	bool dontBroadcast{};
};

using EventListenerCallback = ResultType (*)(const plg::string& name, EventInfo* event, bool dontBroadcast);

struct EventHook {
	plg::string name;
	bool postCopy{};
	uint32_t refCount{1};
	plg::enum_map<ListenerManager<EventListenerCallback>, HookMode> callbacks;
};

enum class EventHookError {
	Okay = 0,
	InvalidEvent,
	NotActive,
	InvalidCallback,
};

class EventManager : public IGameEventListener2 {
public:
	EventManager() = default;
	~EventManager() override;

	EventHookError HookEvent(std::string_view name, EventListenerCallback callback, HookMode mode = HookMode::Post);
	EventHookError UnhookEvent(std::string_view name, EventListenerCallback callback, HookMode mode = HookMode::Post);

	EventInfo* CreateEvent(std::string_view name, bool force = false);
	void FireEvent(EventInfo* info, bool dontBroadcast);
	void FireEventToClient(EventInfo* info, CPlayerSlot slot);
	void CancelCreatedEvent(EventInfo* info);

	ResultType OnFireEvent(IGameEvent* event, bool dontBroadcast);
	ResultType OnFireEvent_Post(IGameEvent* event, bool dontBroadcast);

private:
	void FireGameEvent(IGameEvent* event) override;

private:
	plg::flat_hash_map<plg::string, std::shared_ptr<EventHook>, plg::string_hash, std::equal_to<>> m_hookMap;
	std::recursive_mutex m_mutex;
	std::stack<EventInfo*> m_freeEvents;
	std::stack<std::shared_ptr<EventHook>> m_eventStack;
	std::stack<IGameEvent*> m_eventCopies;
};

extern EventManager g_EventManager;
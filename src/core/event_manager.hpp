#pragma once

#include "listener_manager.hpp"
#include <igameevents.h>

using EventListenerCallback = ResultType (*)(const plg::string& name, IGameEvent* event, bool dontBroadcast);

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
	EventManager() = default;
	~EventManager() override;
	NONCOPYABLE(EventManager)

public:
	static auto& Instance() {
		static EventManager instance;
		return instance;
	}

	EventHookError HookEvent(std::string_view name, EventListenerCallback callback, HookMode mode = HookMode::Post);
	EventHookError UnhookEvent(std::string_view name, EventListenerCallback callback, HookMode mode = HookMode::Post);

	static IGameEvent* CreateEvent(std::string_view name, bool force = false);
	static void FireEvent(IGameEvent* event, bool dontBroadcast);
	static void FireEventToClient(IGameEvent* event, CPlayerSlot slot);
	static void CancelCreatedEvent(IGameEvent* event);

	ResultType OnFireEvent(IGameEvent* event, bool dontBroadcast);
	ResultType OnFireEvent_Post(IGameEvent* event, bool dontBroadcast);

private:
	void FireGameEvent(IGameEvent* event) override;

private:
	plg::flat_hash_map<plg::string, std::shared_ptr<EventHook>, plg::string_hash, std::equal_to<>> m_hookMap;
	std::recursive_mutex m_mutex;
	std::stack<std::shared_ptr<EventHook>> m_eventStack;
	std::stack<IGameEvent*> m_eventCopies;
};
inline EventManager& g_EventManager = EventManager::Instance();

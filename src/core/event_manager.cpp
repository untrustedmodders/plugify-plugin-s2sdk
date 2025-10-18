#include "event_manager.hpp"

EventManager::~EventManager() {
	while (!m_freeEvents.empty()) {
		delete m_freeEvents.top();
		m_freeEvents.pop();
	}

	if (!m_eventHooks.empty()) {
		g_pGameEventManager->RemoveListener(this);
	}
}

EventHookError EventManager::HookEvent(const plg::string& name, EventListenerCallback callback, HookMode mode) {
	if (!g_pGameEventManager->FindListener(this, name.c_str())) {
		if (!g_pGameEventManager->AddListener(this, name.c_str(), true)) {
			return EventHookError::InvalidEvent;
		}
	}

	if (auto eventHook = plg::find(m_eventHooks, name)) {
		eventHook->postCopy = (mode == HookMode::Post);
		++eventHook->refCount;
		return eventHook->callbacks[mode].Register(callback) ? EventHookError::Okay : EventHookError::InvalidCallback;
	} else {
		eventHook = m_eventHooks.emplace(name, std::make_shared<EventHook>(name)).first->second;
		eventHook->postCopy = (mode == HookMode::Post);
		++eventHook->refCount;
		return eventHook->callbacks[mode].Register(callback) ? EventHookError::Okay : EventHookError::InvalidCallback;
	}
}

EventHookError EventManager::UnhookEvent(const plg::string& name, EventListenerCallback callback, HookMode mode) {
	if (auto eventHook = plg::find(m_eventHooks, name)) {
		auto status = eventHook->callbacks[mode].Unregister(callback) ? EventHookError::Okay : EventHookError::InvalidCallback;
		if (eventHook->callbacks[HookMode::Pre].Empty() && eventHook->callbacks[HookMode::Post].Empty() || --eventHook->refCount == 0) {
			m_eventHooks.erase(name);
		}
		return status;
	}

	return EventHookError::Okay;
}

void EventManager::FireGameEvent(IGameEvent* event) {
}

void EventManager::FireEvent(EventInfo* info, bool dontBroadcast) {
	g_pGameEventManager->FireEvent(info->event, dontBroadcast);

	m_freeEvents.push(info);
}

EventInfo* EventManager::CreateEvent(const plg::string& name, bool force) {
	IGameEvent* event = g_pGameEventManager->CreateEvent(name.c_str(), force);

	if (event) {
		EventInfo* info;
		if (m_freeEvents.empty()) {
			info = new EventInfo{};
		} else {
			info = m_freeEvents.top();
			m_freeEvents.pop();
		}

		info->event = event;
		info->dontBroadcast = false;

		return info;
	}

	return nullptr;
}

void EventManager::FireEventToClient(EventInfo* info, CPlayerSlot slot) {
	IGameEventListener2* listener = addresses::GetLegacyGameEventListener(slot);

	listener->FireGameEvent(info->event);
}

void EventManager::CancelCreatedEvent(EventInfo* info) {
	g_pGameEventManager->FreeEvent(info->event);

	m_freeEvents.push(info);
}

ResultType EventManager::OnFireEvent(IGameEvent* event, const bool dontBroadcast) {
	if (!event)
		return ResultType::Continue;

	plg::string name(event->GetName());
	bool localDontBroadcast = dontBroadcast;

	auto it = m_eventHooks.find(name);
	if (it != m_eventHooks.end()) {
		auto& eventHook = it->second;
		++eventHook->refCount;
		m_eventStack.push(eventHook);

		auto& preHook = eventHook->callbacks[HookMode::Pre];
		if (!preHook.Empty()) {
			plg::print(LS_DETAILED, "Pushing event `{}` pointer: {}, dont broadcast: {}, post: {}\n", event->GetName(), static_cast<const void*>(event), dontBroadcast, false);

			EventInfo eventInfo{event, dontBroadcast};

			auto funcs = preHook.Get();
			for (const auto& func : funcs) {
				auto result = func(name, &eventInfo, dontBroadcast);
				localDontBroadcast = eventInfo.dontBroadcast;

				if (result >= ResultType::Handled) {
					// m_EventCopies.push(g_gameEventManager->DuplicateEvent(pEvent));
					g_pGameEventManager->FreeEvent(event);
					return ResultType::Handled;
				}
			}
		}

		if (eventHook->postCopy) {
			m_eventCopies.push(g_pGameEventManager->DuplicateEvent(event));
		}
	} else {
		m_eventStack.push(nullptr);
	}

	if (localDontBroadcast != dontBroadcast) {
		return ResultType::Changed;
	}

	return ResultType::Continue;
}

ResultType EventManager::OnFireEvent_Post(IGameEvent* event, bool dontBroadcast) {
	if (!event)
		return ResultType::Continue;

	auto hook = m_eventStack.top();

	if (hook != nullptr) {
		auto& postHook = hook->callbacks[HookMode::Post];
		if (!postHook.Empty()) {
			if (hook->postCopy) {
				auto eventCopy = m_eventCopies.top();
				plg::print(LS_DETAILED, "Pushing event `{}` pointer: {}, dont broadcast: {}, post: {}\n", eventCopy->GetName(), static_cast<const void*>(eventCopy), dontBroadcast, true);
				EventInfo eventInfo{eventCopy, dontBroadcast};

				postHook(hook->name, &eventInfo, dontBroadcast);

				g_pGameEventManager->FreeEvent(eventInfo.event);

				m_eventCopies.pop();
			} else {
				postHook(hook->name, nullptr, dontBroadcast);
			}
		}

		if (--hook->refCount == 0) {
			m_eventHooks.erase(hook->name);
		}
	}

	m_eventStack.pop();

	return ResultType::Continue;
}

EventManager g_EventManager;
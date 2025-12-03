#include "event_manager.hpp"

EventManager::~EventManager() {
	if (!m_hookMap.empty()) {
		g_pGameEventManager->RemoveListener(this);
	}
}

EventHookError EventManager::HookEvent(std::string_view name, EventListenerCallback callback, HookMode mode) {
	std::scoped_lock lock(m_mutex);

	if (!g_pGameEventManager->FindListener(this, name.data())) {
		if (!g_pGameEventManager->AddListener(this, name.data(), true)) {
			return EventHookError::InvalidEvent;
		}
	}

	std::shared_ptr<EventHook> hook;
	{
		auto it = m_hookMap.find(name);
		if (it != m_hookMap.end()) {
			hook = it->second;
		} else {
			hook = std::make_shared<EventHook>(name, mode == HookMode::Post);
			m_hookMap.emplace(name, hook);
		}
	}
	return hook->callbacks[mode].Register(callback) ? EventHookError::Okay : EventHookError::InvalidCallback;
}

EventHookError EventManager::UnhookEvent(std::string_view name, EventListenerCallback callback, HookMode mode) {
	std::scoped_lock lock(m_mutex);

	auto it = m_hookMap.find(name);
	if (it != m_hookMap.end()) {
		auto hook = it->second;
		auto status = hook->callbacks[mode].Unregister(callback) ? EventHookError::Okay : EventHookError::InvalidCallback;
		if ((hook->callbacks[HookMode::Pre].Empty() && hook->callbacks[HookMode::Post].Empty()) || --hook->refCount == 0) {
			m_hookMap.erase(it);
		}
		return status;
	}

	return EventHookError::Okay;
}

void EventManager::FireGameEvent(IGameEvent* event) {
}

IGameEvent* EventManager::CreateEvent(std::string_view name, bool force) {
	return g_pGameEventManager->CreateEvent(name.data(), force);
}

void EventManager::FireEvent(IGameEvent* event, bool dontBroadcast) {
	g_pGameEventManager->FireEvent(event, dontBroadcast);
}

void EventManager::FireEventToClient(IGameEvent* event, CPlayerSlot slot) {
	IGameEventListener2* listener = addresses::GetLegacyGameEventListener(slot);
	if (!listener) {
		plg::print(LS_WARNING, "Could not get player event listener\n");
		return;
	}

	listener->FireGameEvent(event);
	g_pGameEventManager->FreeEvent(event);
}

void EventManager::CancelCreatedEvent(IGameEvent* event) {
	g_pGameEventManager->FreeEvent(event);
}

ResultType EventManager::OnFireEvent(IGameEvent* event, const bool dontBroadcast) {
	if (!event)
		return ResultType::Continue;

	plg::string name(event->GetName());
	bool localDontBroadcast = dontBroadcast;

	std::scoped_lock lock(m_mutex);
	auto it = m_hookMap.find(name);
	if (it != m_hookMap.end()) {
		auto hook = it->second;
		++hook->refCount;
		m_eventStack.push(hook);

		auto& preHook = hook->callbacks[HookMode::Pre];
		if (!preHook.Empty()) {
			plg::print(LS_DETAILED, "Pushing event `{}` pointer: {}, dont broadcast: {}, post: {}\n", event->GetName(), static_cast<const void*>(event), dontBroadcast, false);

			auto funcs = preHook.Get();
			for (const auto& func : funcs) {
				auto result = func(name, event, dontBroadcast);
				localDontBroadcast = event->GetBool("dont_broadcast");

				if (result >= ResultType::Handled) {
					// m_EventCopies.push(g_gameEventManager->DuplicateEvent(pEvent));
					g_pGameEventManager->FreeEvent(event);
					return ResultType::Handled;
				}
			}
		}

		if (hook->postCopy) {
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
	if (!event || m_eventStack.empty())
		return ResultType::Continue;

	if (auto hook = m_eventStack.top()) {
		auto& postHook = hook->callbacks[HookMode::Post];
		if (!postHook.Empty()) {
			if (hook->postCopy) {
				auto eventCopy = m_eventCopies.top();
				plg::print(LS_DETAILED, "Pushing event `{}` pointer: {}, dont broadcast: {}, post: {}\n", eventCopy->GetName(), static_cast<const void*>(eventCopy), dontBroadcast, true);

				postHook(hook->name, eventCopy, dontBroadcast);

				g_pGameEventManager->FreeEvent(eventCopy);

				m_eventCopies.pop();
			} else {
				postHook(hook->name, nullptr, dontBroadcast);
			}
		}

		if (--hook->refCount == 0) {
			m_hookMap.erase(hook->name);
		}
	}

	m_eventStack.pop();

	return ResultType::Continue;
}

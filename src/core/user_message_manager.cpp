#include "user_message_manager.hpp"
#include "user_message.hpp"

bool UserMessageManager::HookUserMessage(int16_t messageId, UserMessageCallback callback, HookMode mode) {
	std::scoped_lock lock(m_mutex);

	if (messageId == 0) {
		return m_global.callbacks[mode].Register(callback);
	}

	std::shared_ptr<UserMessageHook> hook;
	{
		auto it = m_hookMap.find(messageId);
		if (it != m_hookMap.end()) {
			hook = it->second;
		} else {
			hook = std::make_shared<UserMessageHook>();
			m_hookMap.emplace(messageId, hook);
		}
	}
	return hook->callbacks[mode].Register(callback);
}

bool UserMessageManager::UnhookUserMessage(int16_t messageId, UserMessageCallback callback, HookMode mode) {
	std::scoped_lock lock(m_mutex);

	if (messageId == 0) {
		return m_global.callbacks[mode].Unregister(callback);
	}

	auto it = m_hookMap.find(messageId);
	if (it != m_hookMap.end()) {
		auto hook = it->second;
		auto status = hook->callbacks[mode].Unregister(callback);
		if (hook->callbacks[HookMode::Pre].Empty() && hook->callbacks[HookMode::Post].Empty()) {
			m_hookMap.erase(it);
		}
		return status;
	}

	return false;
}

ResultType UserMessageManager::ExecuteMessageCallbacks(INetworkMessageInternal* msgSerializable, const CNetMessage* msgData, uint64_t* clients, HookMode mode) {
	std::scoped_lock lock(m_mutex);

	UserMessage message(msgSerializable, msgData, *clients);

	int16_t messageId = message.GetMessageID();
	
	//plg::print(LS_DETAILED, "[CUserMessageManager::ExecuteMessageCallbacks][{}] Pushing user message `{}` pointer: %p\n", mode == HookMode::Pre ? "Pre" : "Post",
	//	messageId, static_cast<const void*>(msgSerializable));

	ResultType result = ResultType::Continue;

	{
		auto funcs = m_global.callbacks[mode].Get();
		for (const auto& func : funcs) {
			auto thisResult = func(&message);
			if (thisResult >= ResultType::Stop) {
				if (mode == HookMode::Pre) {
					*clients = static_cast<uint64_t>(*message.GetRecipientFilter().GetRecipients().Base());
					return ResultType::Stop;
				}
				result = thisResult;
				break;
			}

			if (thisResult >= ResultType::Handled) {
				result = thisResult;
			}
		}
	}

	auto it = m_hookMap.find(messageId);
	if (it != m_hookMap.end()) {
		auto hook = it->second;
		auto funcs = hook->callbacks[mode].Get();
		for (const auto& func : funcs) {
			auto thisResult = func(&message);
			if (thisResult >= ResultType::Handled) {
				if (mode == HookMode::Pre) {
					*clients = static_cast<uint64_t>(*message.GetRecipientFilter().GetRecipients().Base());
				}
				return thisResult;
			} else if (thisResult > result) {
				result = thisResult;
			}
		}
	}

	if (mode == HookMode::Pre) {
		*clients = static_cast<uint64_t>(*message.GetRecipientFilter().GetRecipients().Base());
	}

	return result;
}

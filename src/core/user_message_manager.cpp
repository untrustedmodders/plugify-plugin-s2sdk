#include "user_message_manager.hpp"
#include "user_message.hpp"

bool UserMessageManager::HookUserMessage(int16_t messageId, UserMessageCallback callback, HookMode mode) {
	if (messageId == 0) {
		return m_global.callbacks[mode].Register(callback);
	}

	auto it = m_hooksMap.find(messageId);
	if (it == m_hooksMap.end()) {
		auto& commandInfo = *m_hooksMap.emplace(messageId, std::make_unique<UserMessageHook>()).first->second;
		return commandInfo.callbacks[mode].Register(callback);
	} else {
		auto& commandInfo = *it->second;
		return commandInfo.callbacks[mode].Register(callback);
	}
}

bool UserMessageManager::UnhookUserMessage(int16_t messageId, UserMessageCallback callback, HookMode mode) {
	if (messageId == 0) {
		return m_global.callbacks[mode].Unregister(callback);
	}

	auto it = m_hooksMap.find(messageId);
	if (it == m_hooksMap.end()) {
		return false;
	}

	auto& commandInfo = *it->second;
	return commandInfo.callbacks[mode].Unregister(callback);
}

ResultType UserMessageManager::ExecuteMessageCallbacks(INetworkMessageInternal* msgSerializable, const CNetMessage* msgData, uint64_t* clients, HookMode mode) {
	UserMessage message(msgSerializable, msgData, *clients);

	int16_t messageID = message.GetMessageID();
	
	//plg::print(LS_DETAILED, "[CUserMessageManager::ExecuteMessageCallbacks][{}] Pushing user message `{}` pointer: %p\n", mode == HookMode::Pre ? "Pre" : "Post",  messageID, static_cast<const void*>(msgSerializable));

	ResultType result = ResultType::Continue;
	
	{
		auto funcs = m_global.callbacks[mode].AsView();
		for (const auto& func : funcs) {
			auto thisResult = func(&message);
			if (thisResult >= ResultType::Stop) {
				if (mode == HookMode::Pre) {
					*clients = *reinterpret_cast<const uint64_t *>(message.GetRecipientFilter().GetRecipients().Base());
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

	auto it = m_hooksMap.find(messageID);
	if (it != m_hooksMap.end()) {
		auto& messageHook = *it->second;

		auto funcs = messageHook.callbacks[mode].AsView();
		for (const auto& func : funcs) {
			auto thisResult = func(&message);
			if (thisResult >= ResultType::Handled) {
				if (mode == HookMode::Pre) {
					*clients = *reinterpret_cast<const uint64_t *>(message.GetRecipientFilter().GetRecipients().Base());
				}
				return thisResult;
			} else if (thisResult > result) {
				result = thisResult;
			}
		}
	}

	if (mode == HookMode::Pre) {
		*clients = *reinterpret_cast<const uint64_t *>(message.GetRecipientFilter().GetRecipients().Base());
	}
	return result;
}

UserMessageManager g_UserMessageManager;
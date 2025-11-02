#pragma once

#include "listener_manager.hpp"

#include <networksystem/netmessage.h>

class UserMessage;
using UserMessageCallback = ResultType (*)(UserMessage* message);

struct UserMessageHook {
	plg::enum_map<ListenerManager<UserMessageCallback>, HookMode> callbacks;
};

class UserMessageManager {
	UserMessageManager() = default;
	~UserMessageManager() = default;
	NONCOPYABLE(UserMessageManager)

public:
	static auto& Instance() {
		static UserMessageManager instance;
		return instance;
	}

	bool HookUserMessage(int16_t messageId, UserMessageCallback callback, HookMode mode);
	bool UnhookUserMessage(int16_t messageId, UserMessageCallback callback, HookMode mode);

	ResultType ExecuteMessageCallbacks(INetworkMessageInternal* msgSerializable, const CNetMessage* msgData, uint64_t* clients, HookMode mode);

private:
	plg::flat_hash_map<int16_t, std::shared_ptr<UserMessageHook>> m_hookMap;
	std::recursive_mutex m_mutex;
	UserMessageHook m_global;
};
inline UserMessageManager& g_UserMessageManager = UserMessageManager::Instance();
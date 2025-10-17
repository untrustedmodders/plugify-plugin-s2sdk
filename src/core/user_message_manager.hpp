#pragma once

#include "listener_manager.hpp"

#include <networksystem/netmessage.h>

class UserMessage;
using UserMessageCallback = ResultType (*)(UserMessage* message);

struct UserMessageHook {
	plg::enum_map<ListenerManager<UserMessageCallback>, HookMode> callbacks;
};

class UserMessageManager {
public:
	UserMessageManager() = default;
	~UserMessageManager() = default;

	bool HookUserMessage(int16_t messageId, UserMessageCallback callback, HookMode mode);
	bool UnhookUserMessage(int16_t messageId, UserMessageCallback callback, HookMode mode);

	ResultType ExecuteMessageCallbacks(INetworkMessageInternal* msgSerializable, const CNetMessage* msgData, uint64_t* clients, HookMode mode);

private:
	plg::parallel_flat_hash_map<int16_t, std::unique_ptr<UserMessageHook>> m_hooksMap;
	UserMessageHook m_global;
};

extern UserMessageManager g_UserMessageManager;
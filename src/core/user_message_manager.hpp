#pragma once

#include "listener_manager.hpp"

#include <networksystem/netmessage.h>

class UserMessage;
using UserMessageCallback = ResultType (*)(UserMessage* message);
inline char UserMessageStr[] = S2SDK_PACKAGE "::UserMessageListener";
using SendNetMessageHookCallback = ResultType (*)(int playerSlot, UserMessage* message);
inline char SendNetMessageHookStr[] = S2SDK_PACKAGE "::SendNetMessageHook";
class CCLCMsg_VoiceData;
using ClientVoiceDataHookCallback = ResultType (*)(int playerSlot, CCLCMsg_VoiceData* voiceData);
inline char ClientVoiceDataHookStr[] = S2SDK_PACKAGE "::ClientVoiceDataHook";

struct UserMessageHook {
	plg::enum_array<ListenerManager<UserMessageStr, UserMessageCallback>, HookMode> callbacks;
};

class UserMessageManager {
	UserMessageManager() = default;
	~UserMessageManager() = default;
	NONCOPYABLE(UserMessageManager)

	static UserMessageManager instance;
public:
	static auto& Instance() noexcept {
		return instance;
	}

	bool HookUserMessage(int16_t messageId, UserMessageCallback callback, HookMode mode);
	bool UnhookUserMessage(int16_t messageId, UserMessageCallback callback, HookMode mode);

	bool HookSendNetMessage(SendNetMessageHookCallback callback);
	bool UnhookSendNetMessage(SendNetMessageHookCallback callback);
	bool HasSendNetMessageHooks() const;
	bool HookClientVoiceData(ClientVoiceDataHookCallback callback);
	bool UnhookClientVoiceData(ClientVoiceDataHookCallback callback);
	ResultType ExecuteClientVoiceDataCallbacks(int playerSlot, CCLCMsg_VoiceData* voiceData);
	ResultType ExecuteSendNetMessageCallbacks(int playerSlot, UserMessage* message);

	ResultType ExecuteMessageCallbacks(INetworkMessageInternal* msgSerializable, const CNetMessage* msgData, uint64_t* clients, HookMode mode);

private:
	plg::flat_hash_map<int16_t, std::shared_ptr<UserMessageHook>> m_hookMap;
	std::recursive_mutex m_mutex;
	UserMessageHook m_global;
	ListenerManager<SendNetMessageHookStr, SendNetMessageHookCallback> m_sendNetMessageHooks;
	ListenerManager<ClientVoiceDataHookStr, ClientVoiceDataHookCallback> m_clientVoiceDataHooks;
};
inline UserMessageManager& g_UserMessageManager = UserMessageManager::Instance();
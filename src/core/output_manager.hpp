#pragma once

#include "listener_manager.hpp"
#include <core/sdk/utils.h>
#include <core/sdk/entity/entityio.h>

#include <entity2/entitysystem.h>

using OutputKey = std::pair<plg::string, plg::string>;
using EntityListenerCallback = ResultType (*)(int activatorHandle, int callerHandle, float delay);

struct CallbackPair {
	std::array<ListenerManager<EntityListenerCallback>, 2> callbacks;
};

class EntityOutputManager {
public:
	EntityOutputManager() = default;
	~EntityOutputManager() = default;

	bool HookEntityOutput(plg::string classname, plg::string output, EntityListenerCallback callback, HookMode mode);
	bool UnhookEntityOutput(plg::string classname, plg::string output, EntityListenerCallback callback, HookMode mode);

	ResultType FireOutputInternal(CEntityIOOutput* pThis, CEntityInstance* pActivator, CEntityInstance* pCaller, float flDelay);
	ResultType FireOutputInternal_Post(CEntityIOOutput* pThis, CEntityInstance* pActivator, CEntityInstance* pCaller, float flDelay);

private:
	plg::map<OutputKey, CallbackPair> m_hookMap;
	plg::inplace_vector<CallbackPair*, 4> m_vecCallbackPairs;
};

extern EntityOutputManager g_OutputManager;

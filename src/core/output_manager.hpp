#pragma once

#include "listener_manager.hpp"
#include <core/sdk/utils.h>
#include <core/sdk/entity/entityio.h>

#include <entity2/entitysystem.h>

using OutputKey = std::pair<plg::string, plg::string>;
using EntityListenerCallback = ResultType (*)(int activatorHandle, int callerHandle, float delay);

struct CallbackPair {
	plg::enum_map<ListenerManager<EntityListenerCallback>, HookMode> callbacks;
};

class EntityOutputManager {
public:
	EntityOutputManager() = default;
	~EntityOutputManager() = default;

	bool HookEntityOutput(plg::string classname, plg::string output, EntityListenerCallback callback, HookMode mode);
	bool UnhookEntityOutput(plg::string classname, plg::string output, EntityListenerCallback callback, HookMode mode);

	ResultType FireOutputInternal(CEntityIOOutput* self, CEntityInstance* activator, CEntityInstance* caller, float delay);
	ResultType FireOutputInternal_Post(CEntityIOOutput* self, CEntityInstance* activator, CEntityInstance* caller, float delay);

private:
	plg::parallel_flat_hash_map<OutputKey, std::unique_ptr<CallbackPair>, plg::pair_hash<plg::string, plg::string>> m_hookMap;
	plg::inplace_vector<CallbackPair*, 4> m_vecCallbackPairs;
};

extern EntityOutputManager g_OutputManager;

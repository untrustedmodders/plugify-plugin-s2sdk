#pragma once

#include "listener_manager.hpp"
#include <core/sdk/utils.hpp>
#include <core/sdk/entity/entityio.h>

#include <entity2/entitysystem.h>

using OutputKey = std::pair<plg::string, plg::string>;
using EntityListenerCallback = ResultType (*)(int activatorHandle, int callerHandle, float delay);
inline char EntityListenerStr[] = S2SDK_PACKAGE "::EntityListener";

struct EntityOutputHook {
	plg::enum_array<ListenerManager<EntityListenerStr, EntityListenerCallback>, HookMode> callbacks;
};

class EntityOutputManager {
	EntityOutputManager() = default;
	~EntityOutputManager() = default;
	NONCOPYABLE(EntityOutputManager)

	static EntityOutputManager instance;
public:
	static auto& Instance() noexcept {
		return instance;
	}

	bool HookEntityOutput(plg::string classname, plg::string output, EntityListenerCallback callback, HookMode mode);
	bool UnhookEntityOutput(plg::string classname, plg::string output, EntityListenerCallback callback, HookMode mode);

	ResultType FireOutputInternal(CEntityIOOutput* self, CEntityInstance* activator, CEntityInstance* caller, float delay);
	ResultType FireOutputInternal_Post(CEntityIOOutput* self, CEntityInstance* activator, CEntityInstance* caller, float delay);

private:
	plg::flat_hash_map<OutputKey, std::shared_ptr<EntityOutputHook>, plg::pair_hash<plg::string, plg::string>> m_hookMap;
	std::recursive_mutex m_mutex;
	std::inplace_vector<std::shared_ptr<EntityOutputHook>, 4> m_callbackHooks;
};
inline EntityOutputManager& g_EntityOutputManager = EntityOutputManager::Instance();


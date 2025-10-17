#include "output_manager.hpp"

bool EntityOutputManager::HookEntityOutput(plg::string classname, plg::string output, EntityListenerCallback callback, HookMode mode) {
	OutputKey outputKey{std::move(classname), std::move(output)};

	auto it = m_hookMap.find(outputKey);
	if (it == m_hookMap.end()) {
		auto& callbackPair = *m_hookMap.emplace(std::move(outputKey), std::make_unique<CallbackPair>()).first->second;
		return callbackPair.callbacks[mode].Register(callback);
	} else {
		auto& callbackPair = *it->second;
		return callbackPair.callbacks[mode].Register(callback);
	}
}

bool EntityOutputManager::UnhookEntityOutput(plg::string classname, plg::string output, EntityListenerCallback callback, HookMode mode) {
	OutputKey outputKey{std::move(classname), std::move(output)};

	auto it = m_hookMap.find(outputKey);
	if (it != m_hookMap.end()) {
		auto& callbackPair = *it->second;
		auto status = callbackPair.callbacks[mode].Unregister(callback);
		if (callbackPair.callbacks[HookMode::Pre].Empty() && callbackPair.callbacks[HookMode::Post].Empty()) {
			m_hookMap.erase(it);
		}
		return status;
	}

	return false;
}

ResultType EntityOutputManager::FireOutputInternal(CEntityIOOutput* self, CEntityInstance* activator, CEntityInstance* caller, float delay) {

	if (caller) {
		plg::print(LS_DETAILED, "[EntityOutputManager][FireOutputHook] - {}, {}\n", self->m_pDesc->m_pName, caller->GetClassname());

		std::array searchKeys{
				OutputKey{"*", self->m_pDesc->m_pName},
				OutputKey{"*", "*"},
				OutputKey{caller->GetClassname(), self->m_pDesc->m_pName},
				OutputKey{caller->GetClassname(), "*"}
		};

		m_vecCallbackPairs.clear();

		for (const auto& searchKey : searchKeys) {
			auto it = m_hookMap.find(searchKey);
			if (it != m_hookMap.end()) {
				m_vecCallbackPairs.emplace_back(it->second.get());
			}
		}
	} else {
		plg::print(LS_DETAILED, "[EntityOutputManager][FireOutputHook] - {}, unknown caller\n", self->m_pDesc->m_pName);
	}

	ResultType result = ResultType::Continue;

	int activatorHandle = activator != nullptr ? activator->GetEntityIndex().Get() : INVALID_EHANDLE_INDEX;
	int callerHandle = caller != nullptr ? caller->GetEntityIndex().Get() : INVALID_EHANDLE_INDEX;

	for (const auto& pCallbackPair : m_vecCallbackPairs) {
		auto funcs = pCallbackPair->callbacks[HookMode::Pre].AsView();
		for (const auto& func : funcs) {
			auto thisResult = func(activatorHandle, callerHandle, delay);
			if (thisResult >= ResultType::Stop) {
				break;
			}

			if (thisResult > result) {
				result = thisResult;
			}
		}
	}

	return result;
}

ResultType EntityOutputManager::FireOutputInternal_Post(CEntityIOOutput* self, CEntityInstance* activator, CEntityInstance* caller, float delay) {
	int activatorHandle = activator != nullptr ? activator->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
	int callerHandle = caller != nullptr ? caller->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;

	for (const auto& pCallbackPair : m_vecCallbackPairs) {
		auto& callback = pCallbackPair->callbacks[HookMode::Post];
		callback(activatorHandle, callerHandle, delay);
	}

	return ResultType::Continue;
}

EntityOutputManager g_OutputManager;
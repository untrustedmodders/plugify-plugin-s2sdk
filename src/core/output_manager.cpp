#include "output_manager.hpp"

EntityOutputManager EntityOutputManager::instance;

bool EntityOutputManager::HookEntityOutput(std::string_view classname, std::string_view output, EntityListenerCallback callback, HookMode mode) {
	std::scoped_lock lock(m_mutex);

	if (classname.empty() || output.empty()) {
		plg::print(LS_WARNING, "Class/output name empty\n");
		return false;
	}

	std::shared_ptr<EntityOutputHook> hook;
	{
		auto it = m_hookMap.find(OutputView{classname, output});
		if (it != m_hookMap.end()) {
			hook = it->second;
		} else {
			hook = std::make_shared<EntityOutputHook>();
			m_hookMap.emplace(OutputStr{classname, output}, hook);
		}
	}
	return hook->callbacks[mode].Register(callback);
}

bool EntityOutputManager::UnhookEntityOutput(std::string_view classname, std::string_view output, EntityListenerCallback callback, HookMode mode) {
	std::scoped_lock lock(m_mutex);

	if (classname.empty() || output.empty()) {
		plg::print(LS_WARNING, "Class/output name empty\n");
		return false;
	}

	auto it = m_hookMap.find(OutputView{classname, output});
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

ResultType EntityOutputManager::FireOutputInternal(CEntityIOOutput* self, CEntityInstance* activator, CEntityInstance* caller, float delay) {
	std::scoped_lock lock(m_mutex);

	if (caller) {
		plg::print(LS_DETAILED, "[EntityOutputManager][FireOutputHook] - {}, {}\n", self->m_pDesc->m_pName, caller->GetClassname());

		std::array searchKeys{
				OutputView{"*", self->m_pDesc->m_pName},
				OutputView{"*", "*"},
				OutputView{caller->GetClassname(), self->m_pDesc->m_pName},
				OutputView{caller->GetClassname(), "*"}
		};

		m_callbackHooks.clear();

		for (const auto& searchKey : searchKeys) {
			auto it = m_hookMap.find(searchKey);
			if (it != m_hookMap.end()) {
				m_callbackHooks.push_back(it->second);
			}
		}
	} else {
		plg::print(LS_DETAILED, "[EntityOutputManager][FireOutputHook] - {}, unknown caller\n", self->m_pDesc->m_pName);
	}

	ResultType result = ResultType::Continue;

	int activatorHandle = activator != nullptr ? activator->GetEntityIndex().Get() : INVALID_EHANDLE_INDEX;
	int callerHandle = caller != nullptr ? caller->GetEntityIndex().Get() : INVALID_EHANDLE_INDEX;

	for (const auto& hook : m_callbackHooks) {
		if (auto funcs = hook->callbacks[HookMode::Pre].Get()) {
			for (const auto& func : funcs->handlers) {
				auto thisResult = func(activatorHandle, callerHandle, delay);
				if (thisResult >= ResultType::Stop) {
					break;
				}

				if (thisResult > result) {
					result = thisResult;
				}
			}
		}
	}

	return result;
}

ResultType EntityOutputManager::FireOutputInternal_Post(CEntityIOOutput* self, CEntityInstance* activator, CEntityInstance* caller, float delay) {
	int activatorHandle = activator != nullptr ? activator->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;
	int callerHandle = caller != nullptr ? caller->GetRefEHandle().ToInt() : INVALID_EHANDLE_INDEX;

	std::scoped_lock lock(m_mutex);
	for (const auto& hook : m_callbackHooks) {
		auto& callback = hook->callbacks[HookMode::Post];
		callback(activatorHandle, callerHandle, delay);
	}

	return ResultType::Continue;
}

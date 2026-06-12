#pragma once

#include <polyhook/polyhook.hpp>
#include <core/sdk/utils.hpp>

#include "game_config.hpp"

class HookManager {
	HookManager() = default;
	~HookManager() = default;
	NONCOPYABLE(HookManager)

	static HookManager instance;
public:
	static auto& Instance() noexcept {
		return instance;
	}

	using Hook = polyhook::IHook;
	using Type = polyhook::CallbackType;
	using Callback = polyhook::CallbackHandler;

	template<typename Func>
	Result<Memory> AddHookVTableFunc(std::string_view name, Func func, void* ptr, const Callback& callback, std::initializer_list<Type> types, int varIndex = -1) {
		return AddHookVTableFunc<Func>(name, func, ptr, [&](const Hook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		}, varIndex);
	}

	template<typename Func>
	Result<Memory> AddHookVFuncFunc(std::string_view name, Func func, void* ptr, const Callback& callback, std::initializer_list<Type> types, int varIndex = -1) {
		return AddHookVFuncFunc<Func>(name, func, ptr, [&](const Hook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		}, varIndex);
	}

	template<typename Func>
	Result<Memory> AddHookDetourFunc(std::string_view name, const Callback& callback, std::initializer_list<Type> types, int varIndex = -1) {
		return AddHookDetourFunc<Func>(name, [&] -> Result<Memory> { return g_pGameConfig->GetSignature(name); }, [&](const Hook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		}, varIndex);
	}

	template<typename Func>
	Result<Memory> AddHookDetourFunc(std::string_view name, uintptr_t addr, const Callback& callback, std::initializer_list<Type> types, int varIndex = -1) {
		return AddHookDetourFunc<Func>(name, [&] -> Result<Memory> { return addr; }, [&](const Hook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		}, varIndex);
	}

	Result<Memory> AddHookMidFunc(std::string_view name, const Callback& callback, std::initializer_list<Type> types) {
		return AddHookMidFunc(name, [&] -> Result<Memory> { return g_pGameConfig->GetSignature(name); }, [&](const Hook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		});
	}

	Result<Memory> AddHookMidFunc(std::string_view name, uintptr_t addr, const Callback& callback, std::initializer_list<Type> types) {
		return AddHookMidFunc(name, [&] -> Result<Memory> { return addr; }, [&](const Hook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		});
	}

	bool RemoveHookDetourFunc(std::string_view name) {
		auto it = m_dhooks.find(name);
		if (it != m_dhooks.end()) {
			m_dhooks.erase(it);
			return true;
		}
		return false;
	}

	template<typename Func>
	bool RemoveHookVirtualFunc(Func func, void* ptr) {
		auto it = m_vhooks.find({(void*&) func, ptr});
		if (it != m_vhooks.end()) {
			m_vhooks.erase(it);
			return true;
		}
		return false;
	}

	void UnhookAll() {
		m_dhooks.clear();
		m_vhooks.clear();
	}

	template<typename Func>
	int GetVirtualIndex(Func func) {
		return polyhook::GetVirtualIndex((void*&) func);
	}

protected:
	template<typename Func, typename Setter>
	Result<Memory> AddHookVTableFunc(std::string_view name, Func func, void* ptr, const Setter& setter, int varIndex) {
		std::pair key{(void*&) func, ptr};

		auto it = m_vhooks.find(key);
		if (it != m_vhooks.end()) {
			setter(*it->second);
			return it->second->GetAddress();
		}

		using trait = polyhook::details::function_traits<Func>;
		auto args = trait::args();
		auto ret = trait::ret();

		auto ihook = polyhook::VTableHook2::Find(ptr, (void*&) func);
		if (ihook != nullptr) {
			setter(*ihook);
			return ihook->GetAddress();
		}

		ihook = polyhook::VTableHook2::Create(ptr, (void*&) func, ret, args, varIndex, name);
		if (ihook == nullptr) {
			return MakeError("Could not hook table function: \"{}\" - {}",  name, polyhook::GetError());
		}

		setter(*ihook);
		void* orig = ihook->GetAddress();
		m_vhooks.emplace(key, std::move(ihook));
		return orig;
	}

	template<typename Func, typename Setter>
	Result<Memory> AddHookVFuncFunc(std::string_view name, Func func, void* ptr, const Setter& setter, int varIndex) {
		std::pair key{(void*&) func, ptr};

		auto it = m_vhooks.find(key);
		if (it != m_vhooks.end()) {
			setter(*it->second);
			return it->second->GetAddress();
		}

		using trait = polyhook::details::function_traits<Func>;
		auto args = trait::args();
		auto ret = trait::ret();

		auto ihook = polyhook::VFuncHook2::Find(ptr, (void*&) func);
		if (ihook != nullptr) {
			setter(*ihook);
			return ihook->GetAddress();
		}

		ihook = polyhook::VFuncHook2::Create(ptr, (void*&) func, ret, args, varIndex, name);
		if (ihook == nullptr) {
			return MakeError("Could not hook virtual function: \"{}\" - {}",  name, polyhook::GetError());
		}

		setter(*ihook);
		void* orig = ihook->GetAddress();
		m_vhooks.emplace(key, std::move(ihook));
		return orig;
	}

	template<typename Func, typename Getter, typename Setter>
	Result<Memory> AddHookDetourFunc(std::string_view name, const Getter& getter, const Setter& setter, int varIndex) {
		auto it = m_dhooks.find(name);
		if (it != m_dhooks.end()) {
			setter(*it->second);
			return it->second->GetAddress();
		}

		auto addr = getter();
		if (!addr) {
			return MakeError("Could not hook detour function: \"{}\" - {}",  name, addr.error());
		}

		auto ihook = polyhook::DetourHook::Find(*addr);
		if (ihook != nullptr) {
			setter(*ihook);
			return ihook->GetAddress();
		}

		using trait = polyhook::details::function_traits<Func>;
		auto args = trait::args();
		auto ret = trait::ret();

		ihook = polyhook::DetourHook::Create(*addr, ret, args, varIndex, name);
		if (ihook == nullptr) {
			return MakeError("Could not hook detour function: \"{}\" - {}",  name, polyhook::GetError());
		}

		setter(*ihook);
		void* orig = ihook->GetAddress();
		m_dhooks.emplace(name, std::move(ihook));
		return orig;
	}

	template<typename Getter, typename Setter>
	Result<Memory> AddHookMidFunc(std::string_view name, const Getter& getter, const Setter& setter) {
		auto it = m_dhooks.find(name);
		if (it != m_dhooks.end()) {
			setter(*it->second);
			return it->second->GetAddress();
		}

		auto addr = getter();
		if (!addr) {
			return MakeError("Could not hook mid function: \"{}\" - {}",  name, addr.error());
		}

		auto ihook = polyhook::DetourHook::Find(*addr);
		if (ihook != nullptr) {
			setter(*ihook);
			return ihook->GetAddress();
		}

		ihook = polyhook::DetourHook::Create(*addr, name);
		if (ihook == nullptr) {
			return MakeError("Could not hook mid function: \"{}\" - {}",  name, polyhook::GetError());
		}

		setter(*ihook);
		void* orig = ihook->GetAddress();
		m_dhooks.emplace(name, std::move(ihook));
		return orig;
	}

private:
	plg::flat_hash_map<plg::string, std::unique_ptr<Hook>, plg::string_hash, std::equal_to<>> m_dhooks;
	plg::flat_hash_map<std::pair<void*, void*>, std::unique_ptr<Hook>, plg::pair_hash<void*, void*>> m_vhooks;
};
inline HookManager& g_HookManager = HookManager::Instance();

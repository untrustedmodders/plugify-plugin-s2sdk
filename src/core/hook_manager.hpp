#pragma once

#include <polyhook/polyhook.hpp>
#include <core/sdk/utils.hpp>

#include "game_config.hpp"

class HookManager {
	HookManager() = default;
	~HookManager() = default;
	NONCOPYABLE(HookManager)

public:
	static auto& Instance() {
		static HookManager instance;
		return instance;
	}

	using Callback = polyhook::CallbackHandler;
	using Type = polyhook::CallbackType;

	template<typename Func, int Var = -1>
	Result<Memory> AddHookVTableFunc(std::string_view name, Func func, void* ptr, const Callback& callback, std::initializer_list<Type> types) {
		return AddHookVTableFunc<Func>(name, func, ptr, [&](const polyhook::IHook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		}, Var);
	}

	template<typename Func, int Var = -1>
	Result<Memory> AddHookVFuncFunc(std::string_view name, Func func, void* ptr, const Callback& callback, std::initializer_list<Type> types) {
		return AddHookVFuncFunc<Func>(name, func, ptr, [&](const polyhook::IHook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		}, Var);
	}

	template<typename Func, int Var = -1>
	Result<Memory> AddHookDetourFunc(std::string_view name, const Callback& callback, std::initializer_list<Type> types) {
		return AddHookDetourFunc<Func>(name, [&] -> Result<Memory> { return g_pGameConfig->GetSignature(name); }, [&](const polyhook::IHook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		}, Var);
	}

	template<typename Func, int Var = -1>
	Result<Memory> AddHookDetourFunc(std::string_view name, uintptr_t addr, const Callback& callback, std::initializer_list<Type> types) {
		return AddHookDetourFunc<Func>(name, [&] -> Result<Memory> { return addr; }, [&](const polyhook::IHook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		}, Var);
	}

	template<int Var = -1>
	Result<Memory> AddHookMidFunc(std::string_view name, const Callback& callback, std::initializer_list<Type> types) {
		return AddHookMidFunc(name, [&] -> Result<Memory> { return g_pGameConfig->GetSignature(name); }, [&](const polyhook::IHook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		}, Var);
	}

	template<int Var = -1>
	Result<Memory> AddHookMidFunc(std::string_view name, uintptr_t addr, const Callback& callback, std::initializer_list<Type> types) {
		return AddHookMidFunc(name, [&] -> Result<Memory> { return addr; }, [&](const polyhook::IHook& hook) {
			for (const auto& type : types) hook.AddCallback(type, callback);
		}, Var);
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
	using Getter = std::function<Result<Memory>()>;
	using Setter = std::function<void(const polyhook::IHook&)>;

	template<typename Func>
	Result<Memory> AddHookVTableFunc(std::string_view name, Func func, void* ptr, const Setter& setter, [[maybe_unused]] int varIndex = -1) {
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

		ihook = polyhook::VTableHook2::Create(ptr, (void*&) func, ret, plg::vector<polyhook::DataType>(args.begin(), args.end()), varIndex);
		if (ihook == nullptr) {
			return MakeError("Could not hook table function: \"{}\"", name);
		}
		ihook->SetName(name);

		setter(*ihook);
		void* orig = ihook->GetAddress();
		m_vhooks.emplace(key, std::move(ihook));
		return orig;
	}

	template<typename Func>
	Result<Memory> AddHookVFuncFunc(std::string_view name, Func func, void* ptr, const Setter& setter, [[maybe_unused]] int varIndex = -1) {
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

		ihook = polyhook::VFuncHook2::Create(ptr, (void*&) func, ret, plg::vector<polyhook::DataType>(args.begin(), args.end()), varIndex);
		if (ihook == nullptr) {
			return MakeError("Could not hook virtual function: \"{}\"", name);
		}
		ihook->SetName(name);

		setter(*ihook);
		void* orig = ihook->GetAddress();
		m_vhooks.emplace(key, std::move(ihook));
		return orig;
	}

	template<typename Func>
	Result<Memory> AddHookDetourFunc(std::string_view name, const Getter& getter, const Setter& setter, [[maybe_unused]] int varIndex = -1) {
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

		ihook = polyhook::DetourHook::Create(*addr, ret, plg::vector<polyhook::DataType>(args.begin(), args.end()), varIndex);
		if (ihook == nullptr) {
			return MakeError("Could not hook detour function: \"{}\"",  name);
		}
		ihook->SetName(name);

		setter(*ihook);
		void* orig = ihook->GetAddress();
		m_dhooks.emplace(name, std::move(ihook));
		return orig;
	}

	Result<Memory> AddHookMidFunc(std::string_view name, const Getter& getter, const Setter& setter, [[maybe_unused]] int varIndex = -1) {
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

		ihook = polyhook::DetourHook::Create(*addr);
		if (ihook == nullptr) {
			return MakeError("Could not hook mid function: \"{}\"",  name);
		}
		ihook->SetName(name);

		setter(*ihook);
		void* orig = ihook->GetAddress();
		m_dhooks.emplace(name, std::move(ihook));
		return orig;
	}

private:
	plg::flat_hash_map<plg::string, std::unique_ptr<polyhook::IHook>, plg::string_hash, std::equal_to<>> m_dhooks;
	plg::flat_hash_map<std::pair<void*, void*>, std::unique_ptr<polyhook::IHook>, plg::pair_hash<void*, void*>> m_vhooks;
};
inline HookManager& g_HookManager = HookManager::Instance();

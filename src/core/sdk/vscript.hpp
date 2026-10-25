#pragma once

#include <vscript/ivscript.h>

#include <algorithm>
#include <cstring>
#include <string_view>
#include <type_traits>
#include <utility>

struct VScriptBinding {
	void* funcaddr = nullptr;    // free function or non-virtual member
	ptrdiff_t vtable_index = -1; // virtual member (Itanium ABI only)

	explicit operator bool() const { return funcaddr != nullptr || vtable_index >= 0; }

	void* Resolve(void* instance) const {
		return funcaddr ? funcaddr : (*static_cast<void***>(instance))[vtable_index];
	}
};

namespace vscript {
	void RegisterFunction(ScriptFunctionBinding_t* scriptFunction);
	void RegisterScriptClass(ScriptClassDesc_t* classDesc, void* instance = nullptr);

	VScriptBinding GetBinding(std::string_view className, std::string_view functionName, bool reportMissing = true);

	template<size_t N>
	struct Name {
		char str[N]{};
		constexpr Name(const char (&s)[N]) { std::copy_n(s, N, str); }
	};

	// One cache per (class, function). Not cached while missing: bindings show up once the VM registers them on map load
	template<typename Class, Name FunctionName>
	const VScriptBinding& Binding() {
		static VScriptBinding binding;
		static bool reported = false;
		if (!binding) {
			binding = GetBinding(Class::m_className, FunctionName.str, !std::exchange(reported, true));
		}
		return binding;
	}

	struct Global {
		static constexpr const char m_className[] = "";
	};

	template<typename Ret>
	Ret Fail() {
		if constexpr (std::is_reference_v<Ret>) {
			static std::remove_cvref_t<Ret> fallback{};
			return fallback;
		} else if constexpr (!std::is_void_v<Ret>) {
			return Ret{};
		}
	}

	struct Dummy {};

	template<typename Class, Name FunctionName, typename Ret, typename... Args>
	Ret CallMember(Class* instance, std::type_identity_t<Args>... args) {
		const auto& binding = Binding<Class, FunctionName>();
		if (!binding) [[unlikely]] {
			return Fail<Ret>();
		}
		// Call through a real member pointer: MSVC x64 passes the hidden return pointer after `this` for members,
		// but before it for free functions, so Ret(*)(void*, Args...) breaks class-type returns (Vector, QAngle)
		Ret (Dummy::*method)(Args...);
		void* func = binding.Resolve(instance);
		std::memset(&method, 0, sizeof(method));
		std::memcpy(&method, &func, sizeof(func));
		return (reinterpret_cast<Dummy*>(instance)->*method)(args...);
	}

	template<Name FunctionName, typename Ret, typename... Args>
	Ret CallGlobal(std::type_identity_t<Args>... args) {
		const auto& binding = Binding<Global, FunctionName>();
		if (!binding) [[unlikely]] {
			return Fail<Ret>();
		}
		return reinterpret_cast<Ret (*)(Args...)>(binding.funcaddr)(args...);
	}
}// namespace vscript

#define VSCRIPT_MEMBER_FUNCTION(name, ret, ...) \
	template<typename... Ts> \
	ret name(Ts&&... args) { \
		return vscript::CallMember<std::remove_pointer_t<decltype(this)>, #name, ret __VA_OPT__(,) __VA_ARGS__>(this, std::forward<Ts>(args)...); \
	}

#define VSCRIPT_GLOBAL_FUNCTION(name, ret, ...) \
	template<typename... Ts> \
	static ret name(Ts&&... args) { \
		return vscript::CallGlobal<#name, ret __VA_OPT__(,) __VA_ARGS__>(std::forward<Ts>(args)...); \
	}

#define DECLARE_VSCRIPT_CLASS(ClassName) \
	public: \
		static constexpr const char m_className[] = #ClassName;

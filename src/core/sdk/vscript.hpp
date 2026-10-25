#pragma once

#include <vscript/ivscript.h>

#include <bit>
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

	// Kept as a static member next to its wrapper, so debuggers show the names and the resolved address.
	// Not cached while missing: bindings show up once the VM registers them on map load
	template<const char* ClassName, const char* FunctionName>
	struct Binding {
		const char* className = ClassName;
		const char* functionName = FunctionName;
		VScriptBinding resolved;
		bool reported = false;

		const VScriptBinding& Get() {
			if (!resolved) {
				resolved = GetBinding(className, functionName, !std::exchange(reported, true));
			}
			return resolved;
		}
	};

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

	template<typename Ret, typename... Args>
	Ret CallMember(const VScriptBinding& binding, void* instance, std::type_identity_t<Args>... args) {
		if (!binding) [[unlikely]] {
			return Fail<Ret>();
		}
		// Call through a real member pointer: MSVC x64 passes the hidden return pointer after `this` for members,
		// but before it for free functions, so Ret(*)(void*, Args...) breaks class-type returns (Vector, QAngle)
		Ret (Dummy::*method)(Args...){};
		void* func = binding.Resolve(instance);
		method = std::bit_cast<decltype(method)>(func);
		return (reinterpret_cast<Dummy*>(instance)->*method)(args...);
	}

	template<typename Ret, typename... Args>
	Ret CallGlobal(const VScriptBinding& binding, std::type_identity_t<Args>... args) {
		if (!binding) [[unlikely]] {
			return Fail<Ret>();
		}
		return reinterpret_cast<Ret (*)(Args...)>(binding.funcaddr)(args...);
	}
}// namespace vscript

#define VSCRIPT_MEMBER_FUNCTION(name, ret, ...) \
private: \
	static constexpr const char name##_str[] = #name; \
	static inline vscript::Binding<m_className, name##_str> name##_binding; \
public: \
	template<typename... Ts> \
	ret name(Ts&&... args) { \
		return vscript::CallMember<ret __VA_OPT__(,) __VA_ARGS__>(name##_binding.Get(), this, std::forward<Ts>(args)...); \
	}

#define VSCRIPT_GLOBAL_FUNCTION(name, ret, ...) \
private: \
	static constexpr const char name##_str[] = #name; \
	static inline vscript::Binding<vscript::Global::m_className, name##_str> name##_binding; \
public: \
	template<typename... Ts> \
	static ret name(Ts&&... args) { \
		return vscript::CallGlobal<ret __VA_OPT__(,) __VA_ARGS__>(name##_binding.Get(), std::forward<Ts>(args)...); \
	}

#define DECLARE_VSCRIPT_CLASS(ClassName) \
	public: \
		static constexpr const char m_className[] = #ClassName;

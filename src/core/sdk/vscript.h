#pragma once

#include <vscript/ivscript.h>

struct VScriptBinding {
	void* funcadr;
	intptr_t is_virtual;
	intptr_t vtable_index;
	intptr_t delta;
};

namespace vscript {
	void RegisterFunction(ScriptFunctionBinding_t* pScriptFunction);
	void RegisterScriptClass(ScriptClassDesc_t* pClassDesc);
	VScriptBinding GetFunctionBinding(std::string_view functionName);
	VScriptBinding GetFunctionBinding(std::string_view className, std::string_view functionName);
}// namespace schema

template<
	typename ThisClass,
	char const* FunctionName,
	size_t(*MemberOffset)(),
	typename Ret, typename... Args
>
class VScriptMemberRawFunction {
public:
	Ret operator()(Args&&... args) {
		static VScriptBinding binding = vscript::GetFunctionBinding(ThisClass::m_className, FunctionName);
		void* instance = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) - MemberOffset());
#if S2SDK_PLATFORM_WINDOWS
		// MSVC: Direct member function pointer call
		using MemberFunc = Ret(__thiscall*)(void*, Args...);
		auto func = reinterpret_cast<MemberFunc>(binding.funcadr);
		return func(instance, std::forward<Args>(args)...);
#elif S2SDK_PLATFORM_LINUX
		// GCC: Handle member function pointer structure
		using MemberFunc = Ret(*)(void*, Args...);
		if (binding.is_virtual) {
			// Virtual function - vtable lookup
			void** vtable = *static_cast<void***>(instance);
			auto func = reinterpret_cast<MemberFunc>(vtable[binding.vtable_index]);
			return func(instance + binding.delta, std::forward<Args>(args)...);
		} else {
			// Non-virtual function - direct call
			auto func = reinterpret_cast<MemberFunc>(binding.funcadr);
			return func(instance, std::forward<Args>(args)...);
		}
#else
#	error "Unsupported compiler"
#endif
	}
};

template<const char* FunctionName, typename Ret, typename... Args>
class VScriptRawFunction {
public:
	static Ret operator()(Args&&... args) {
		static VScriptBinding binding = vscript::GetFunctionBinding(FunctionName);
#if S2SDK_PLATFORM_WINDOWS
		using Func = Ret(__cdecl*)(Args...);
		auto fn = reinterpret_cast<Func>(binding.funcadr);
		return fn(std::forward<Args>(args)...);
#elif S2SDK_PLATFORM_LINUX
		using Func = Ret(*)(Args...);
		auto fn = reinterpret_cast<Func>(binding.funcadr);
		return fn(std::forward<Args>(args)...);
#else
#   error "Unsupported compiler"
#endif
	}
};

#define VSCRIPT_RAW_FUNCTION(name, ret, ...) \
private: \
	static constexpr const char name##_str[] = #name; \
	static constexpr size_t name##_offset() { return offsetof(ThisClass, name); }; \
public: \
    PLUGIFY_NO_UNIQUE_ADDRESS VScriptMemberRawFunction<ThisClass, name##_str, name##_offset, ret __VA_OPT__(,) __VA_ARGS__> name;

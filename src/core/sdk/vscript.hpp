#pragma once

#include <vscript/ivscript.h>

struct VScriptBinding {
	void* funcadr;
	size_t vtable_index;
	intptr_t delta;
	ScriptFunctionBinding_t* binding;
};

struct VScriptFunction {
	ScriptFunctionBinding_t* binding;
	void* instance;
};

using VScriptBindingMap = plg::flat_hash_map<plg::string, VScriptBinding, plg::string_hash, std::equal_to<>>;
struct VScriptClass {
	VScriptBindingMap functions;
	ScriptClassDesc_t* descriptor;
	void* instance;
};

using VScriptClassMap = plg::flat_hash_map<plg::string, VScriptClass, plg::string_hash, std::equal_to<>>;
namespace vscript {
	void RegisterFunction(ScriptFunctionBinding_t* pScriptFunction);
	void RegisterScriptClass(ScriptClassDesc_t* pClassDesc, void* pInstance = nullptr);

	VScriptBinding GetBinding(std::string_view functionName);
	VScriptBinding GetBinding(std::string_view className, std::string_view functionName);
}// namespace schema

template<
	typename ThisClass,
	char const* FunctionName,
	size_t(*MemberOffset)(),
	typename Ret, typename... Args
>
class VScriptMemberFunction {
public:
	Ret operator()(Args... args) {
		static VScriptBinding binding = vscript::GetBinding(ThisClass::m_className, FunctionName);
		void* instance = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) - MemberOffset());
#if defined(_MSC_VER)
		// MSVC: Direct member function pointer call
		using MemberFunc = Ret(__thiscall*)(void*, Args...);
		auto func = reinterpret_cast<MemberFunc>(binding.funcadr);
		return func(instance, std::forward<Args>(args)...);
#elif defined(__GNUC__) || defined(__clang__)
		// GCC: Handle member function pointer structure
		using MemberFunc = Ret(*)(void*, Args...);
		if (binding.vtable_index != -1) {
			// Virtual function - vtable lookup
			void** vtable = *static_cast<void***>(instance);
			auto func = reinterpret_cast<MemberFunc>(vtable[binding.vtable_index]);
			return func(instance/* + binding.delta*/, std::forward<Args>(args)...);
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

template<
	typename ThisClass,
	char const* FunctionName,
	typename Ret, typename... Args
>
class VScriptGlobalFunction {
public:
	Ret operator()(Args... args) {
		static VScriptBinding binding = vscript::GetBinding(ThisClass::m_className, FunctionName);
#if defined(_MSC_VER)
		using Func = Ret(__cdecl*)(Args...);
		auto fn = reinterpret_cast<Func>(binding.funcadr);
		return fn(std::forward<Args>(args)...);
#elif defined(__GNUC__) || defined(__clang__)
		using Func = Ret(*)(Args...);
		auto fn = reinterpret_cast<Func>(binding.funcadr);
		return fn(std::forward<Args>(args)...);
#else
#   error "Unsupported compiler"
#endif
	}
};

#define VSCRIPT_GLOBAL_FUNCTION(name, ret, ...) \
private: \
	static constexpr const char name##_str[] = #name; \
public: \
    PLUGIFY_NO_UNIQUE_ADDRESS VScriptGlobalFunction<ThisClass, name##_str, ret __VA_OPT__(,) __VA_ARGS__> name;

#define VSCRIPT_MEMBER_FUNCTION(name, ret, ...) \
private: \
	static constexpr const char name##_str[] = #name; \
	static constexpr size_t name##_offset() { return offsetof(ThisClass, name); }; \
public: \
    PLUGIFY_NO_UNIQUE_ADDRESS VScriptMemberFunction<ThisClass, name##_str, name##_offset, ret __VA_OPT__(,) __VA_ARGS__> name;

#define DECLARE_VSCRIPT_CLASS(ClassName) \
	public: \
		using ThisClass = ClassName; \
		static constexpr const char m_className[] = #ClassName; \

#define DECLARE_VSCRIPT_CLASS2(ClassName) \
	public: \
		using ThisClass = ClassName; \
		static constexpr const char m_className[] = ""; \


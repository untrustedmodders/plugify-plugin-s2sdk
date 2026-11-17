#pragma once

#include <vscript/ivscript.h>

#include "virtual.hpp"

struct VScriptBinding {
	void* funcaddr;
	size_t vtable_index;
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
	void RegisterFunction(ScriptFunctionBinding_t* scriptFunction);
	void RegisterScriptClass(ScriptClassDesc_t* classDesc, void* instance = nullptr);

	VScriptBinding FindBinding(std::string_view className, std::string_view functionName);
	VScriptBinding FindBinding(const char* className, const char* functionName);
	void* FindInstance(std::string_view className);
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
		static VScriptBinding binding = vscript::FindBinding(ThisClass::m_className, FunctionName);
		void* instance = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) - MemberOffset());
		void* addr = binding.funcaddr ? binding.funcaddr : (*static_cast<void***>(instance))[binding.vtable_index];
		return CallMember<Ret>(addr, instance, std::forward<Args>(args)...);
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
		static VScriptBinding binding = vscript::FindBinding(ThisClass::m_className, FunctionName);
		using Func = Ret(*)(Args...);
		Func func = reinterpret_cast<Func>(binding.funcaddr);
		return func(std::forward<Args>(args)...);
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
	static PLUGIN_API size_t name##_offset() { return offsetof(ThisClass, name); }; \
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


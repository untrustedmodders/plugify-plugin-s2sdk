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
	void* m_instance;
};

using VScriptBindingMap = plg::flat_hash_map<plg::string, VScriptBinding, plg::string_hash, std::equal_to<>>;
struct VScriptClass {
	VScriptBindingMap m_functions;
	ScriptClassDesc_t* m_descriptor;
	void* m_instance;
};

using VScriptClassMap = plg::flat_hash_map<plg::string, VScriptClass, plg::string_hash, std::equal_to<>>;
namespace vscript {
	void RegisterFunction(ScriptFunctionBinding_t* pScriptFunction);
	void RegisterScriptClass(ScriptClassDesc_t* pClassDesc);
	//void RegisterInstance(ScriptClassDesc_t* pClassDesc, void* pInstance);
	//void SetValue(IScriptVM* vm, const ScriptVariant_t& value);

	VScriptBinding GetBinding(std::string_view functionName);
	VScriptBinding GetBinding(std::string_view className, std::string_view functionName);

	VScriptFunction GetFunction(std::string_view functionName);
	VScriptFunction GetFunction(std::string_view className, std::string_view functionName);

	const ScriptFunctionBinding_t* LookupFunction(std::string_view className, std::string_view functionName);
	ScriptVariant_t CallFunction(std::string_view className, std::string_view functionName, void* context, const ScriptVariant_t* args);
	ScriptVariant_t CallFunction(const ScriptFunctionBinding_t* func, void* context, const ScriptVariant_t* args);
}// namespace schema

#if 0
template<
	typename ThisClass,
	char const* FunctionName,
	typename Ret, typename... Args
>
class VScriptJitFunction {
public:
	Ret operator()(Args... args) const {
		auto [binding, instance] = vscript::GetFunction(ThisClass::m_className, FunctionName);

		ScriptVariant_t argv[sizeof...(Args)] = {};
		FillVariants(argv, std::forward<Args>(args)...);

		ScriptVariant_t ret;
		ScriptVariant_t* retPtr = (binding->m_desc.m_ReturnType != 0) ? &ret : nullptr;

		bool ok = binding->m_pfnBinding(
		    binding->m_pFunction,
		    instance,
		    argv,
		    sizeof...(Args),
		    retPtr
		);
		Assert(ok);
		return GetReturn(ret);
	}

private:
	template <size_t Index = 0, typename T, typename... Rest>
	static void FillVariants(ScriptVariant_t* argv, T&& arg, Rest&&... rest) {
		ConvertToVariant(argv[Index], std::forward<T>(arg));
		if constexpr (sizeof...(Rest) > 0) {
			FillVariants<Index + 1>(argv, std::forward<Rest>(rest)...);
		}
	}

	static void FillVariants(ScriptVariant_t* /*argv*/) {}

	template<typename T>
	static void ConvertToVariant(ScriptVariant_t& var, T&& value) {
		var = std::forward<T>(value);
	}

	static Ret GetReturn(const ScriptVariant_t& var) {
		if constexpr (std::is_void_v<Ret>) {
			return;
		} else {
			Ret ret;
			var.AssignTo<Ret>(ret);
			return ret;
		}
	}
};
#endif

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
#if S2SDK_PLATFORM_WINDOWS
		// MSVC: Direct member function pointer call
		using MemberFunc = Ret(__thiscall*)(void*, Args...);
		auto func = reinterpret_cast<MemberFunc>(binding.funcadr);
		return func(instance, std::forward<Args>(args)...);
#elif S2SDK_PLATFORM_LINUX
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

template<const char* FunctionName, typename Ret, typename... Args>
class VScriptGlobalFunction {
public:
	static Ret operator()(Args... args) {
		static VScriptBinding binding = vscript::GetBinding(FunctionName);
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

/*#define VSCRIPT_JIT_FUNCTION(name, ret, ...) \
private: \
	static constexpr const char name##_str[] = #name; \
public: \
    PLUGIFY_NO_UNIQUE_ADDRESS VScriptJitFunction<ThisClass, name##_str, ret __VA_OPT__(,) __VA_ARGS__> name;
*/

#define VSCRIPT_GLOBAL_FUNCTION(name, ret, ...) \
private: \
	static constexpr const char name##_str[] = #name; \
public: \
    PLUGIFY_NO_UNIQUE_ADDRESS VScriptGlobalFunction<name##_str, ret __VA_OPT__(,) __VA_ARGS__> name;

#define VSCRIPT_MEMBER_FUNCTION(name, ret, ...) \
private: \
	static constexpr const char name##_str[] = #name; \
	static constexpr size_t name##_offset() { return offsetof(ThisClass, name); }; \
public: \
    PLUGIFY_NO_UNIQUE_ADDRESS VScriptMemberFunction<ThisClass, name##_str, name##_offset, ret __VA_OPT__(,) __VA_ARGS__> name;

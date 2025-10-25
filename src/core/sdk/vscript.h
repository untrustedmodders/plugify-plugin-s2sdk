#pragma once

#include <vscript/ivscript.h>

struct GnuMFP {
	union {
		void* funcadr; // If vtable_index_2 is even, function pointer
		int vtable_index_2; // If vtable_index_2 is odd, vindex*2+1
	};
	int delta;
};

template<
	typename ThisClass,
	char const* FunctionName,
	size_t(*MemberOffset)(),
	typename Ret, typename... Args
>
class VScriptMemberFunction {
private:
	Ret operator()(Args&&... args) {
		static ScriptFunctionBindingStorageType_t binding = nullptr;//vscript::GetFunctionBinding(ThisClass::m_className, FunctionName);
		void* instance = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) - MemberOffset());
#if S2SDK_PLATFORM_WINDOWS
		// MSVC: Direct member function pointer call
		using MemberFunc = Ret(__thiscall*)(void*, Args...);
		auto func = reinterpret_cast<MemberFunc>(binding);
		return func(instance, std::forward<Args>(args)...);
#elif S2SDK_PLATFORM_LINUX
		// GCC: Handle member function pointer structure
		GnuMFP* mfp = reinterpret_cast<GnuMFP*>(binding);
		char* adjusted = reinterpret_cast<char*>(instance) + mfp->delta;

		if (mfp->vtable_index_2 & 1) {
			// Virtual function - vtable lookup
			int vindex = (mfp->vtable_index_2 - 1) / 2;
			void** vtable = *reinterpret_cast<void***>(adjusted);
			auto func = reinterpret_cast<MemberFunc>(vtable[vindex]);
			return func(adjusted, std::forward<Args>(args)...);
		} else {
			// Non-virtual function - direct call
			auto func = reinterpret_cast<MemberFunc>(mfp->funcadr);
			return func(adjusted, std::forward<Args>(args)...);
		}
#else
#	error "Unsupported compiler"
#endif
	}
};

#if 0
template<const char* FunctionName, typename Ret, typename... Args>
class VScriptFunction {
public:
	static Ret operator()(Args&&... args) {
		static ScriptFunctionBindingStorageType_t binding = nullptr; // vscript::GetGlobalFunctionBinding(FunctionName)
#if S2SDK_PLATFORM_WINDOWS
		using Func = Ret(__cdecl*)(Args...);
		auto fn = reinterpret_cast<Func>(binding);
		return fn(std::forward<Args>(args)...);
#elif S2SDK_PLATFORM_LINUX
		GnuMFP* mfp = reinterpret_cast<GnuMFP*>(binding);
		using Func = Ret(*)(Args...);
		auto fn = reinterpret_cast<Func>(fp->funcadr);
		return fn(std::forward<Args>(args)...);
#else
#   error "Unsupported compiler"
#endif
	}
};
#endif

#define VSCRIPT_FUNCTION(name, ret, ...) \
private: \
	static constexpr const char name##_str[] = #name; \
	static constexpr size_t name##_offset() { return offsetof(ThisClass, name); }; \
public: \
    PLUGIFY_NO_UNIQUE_ADDRESS VScriptMemberFunction<ThisClass, name##_str, name##_offset, ret __VA_OPT__(,) __VA_ARGS__> name;
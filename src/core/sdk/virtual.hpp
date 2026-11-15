#pragma once

// Calls a member function at a raw address on 'klass' using the compiler's own
// member-function calling convention (via a member function pointer).
//   MSVC x64: 'this' in RCX, hidden return pointer in RDX. A plain (__thiscall*) pointer is a free
//             function on x64 and swaps them for struct returns, so it must not be used.
//   Itanium (GCC/Clang): {ptr, adj} with an even ptr = direct call to that address, adj = 0.
template<typename Ret, typename... Args>
Ret CallMember(void* addr, void* klass, Args... args) {
	class Crutch {};
	using MemberFunc = Ret (Crutch::*)(Args...);
#if defined(_MSC_VER)
	const auto func = std::bit_cast<MemberFunc>(addr);
#elif defined(__GNUC__) || defined(__clang__)
	struct ItaniumMFP {
		uintptr_t ptr;
		intptr_t adj;
	};
	const auto func = std::bit_cast<MemberFunc>(ItaniumMFP{reinterpret_cast<uintptr_t>(addr), 0});
#else
#	error "Unsupported compiler"
#endif
	return (static_cast<Crutch*>(klass)->*func)(std::forward<Args>(args)...);
}

template<typename Ret, typename... Args>
Ret CallVirtual(auto index, void* klass, Args... args) {
	// No null checks on purpose: a null object / vtable must crash here, not be silently skipped.
	void* addr = (*static_cast<void***>(klass))[index];
	return CallMember<Ret>(addr, klass, std::forward<Args>(args)...);
}

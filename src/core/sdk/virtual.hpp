#pragma once

#define CALL_VIRTUAL(retType, idx, ...) CallVirtual<retType>(static_cast<size_t>(idx), __VA_ARGS__)

template<typename T = void*>
T GetVMethod(size_t index, void* klass) {
	if (!klass) {
		return T{};
	}

	void** table = *static_cast<void***>(klass);
	if (!table) {
		return T{};
	}

	return reinterpret_cast<T>(table[index]);
}

template<typename T, typename... Args>
T CallVirtual(size_t index, void* klass, Args... args) {
	void* addr = GetVMethod(index, klass);
	if (!addr) {
		if constexpr (std::is_void_v<T>) {
			return;
		} else {
			return T{};
		}
	}

#if defined(_MSC_VER)
	class Crutch {};
	using MemberFunc = T (Crutch::*)(Args...);
	const auto func = std::bit_cast<MemberFunc>(addr);
	return (static_cast<Crutch*>(klass)->*func)(std::forward<Args>(args)...);
#elif defined(__GNUC__) || defined(__clang__)
	// GCC/Clang (System V): 'this' is just the first argument.
	using Func = T (*)(void*, Args...);
	const auto func = reinterpret_cast<Func>(addr);
	return func(klass, std::forward<Args>(args)...);
#else
#	error "Unsupported compiler"
#endif
}

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
#if defined(_MSC_VER)
	using MemberFunc = T (__thiscall*)(void*, Args...);
#elif defined(__GNUC__) || defined(__clang__)
	using MemberFunc = T (*)(void*, Args...);
#else
#	error "Unsupported compiler"
#endif
	auto func = GetVMethod<MemberFunc>(index, klass);
	if (!func) {
		if constexpr (std::is_void_v<T>) {
			return;
		} else {
			return T{};
		}
	}

	if constexpr (std::is_void_v<T>) {
		func(klass, std::forward<Args>(args)...);
	} else {
		return func(klass, std::forward<Args>(args)...);
	}
}
#pragma once

#define CALL_VIRTUAL(retType, idx, ...) CallVirtual<retType>(static_cast<size_t>(idx), __VA_ARGS__)

template<typename T>
T MemberCast(void* ptr) {
	union {
		void* ptr;
		T func;
	} u { ptr };
	return u.func;
}

template<typename T = void*>
T GetVMethod(size_t index, void* klass) {
	if (!klass) {
		return T{};
	}

	void** table = *reinterpret_cast<void***>(klass);
	if (!table) {
		return T{};
	}

	return MemberCast<T>(table[index]);
}

class ThisCall
{
};

template<typename T, typename... Args>
T CallVirtual(size_t index, void* klass, Args... args) {
	using MemberFunc = T (ThisCall::*)(Args...);
	auto func = GetVMethod<MemberFunc>(index, klass);
	if (!func) {
		if constexpr (std::is_void_v<T>) {
			return;
		} else {
			return T{};
		}
	}

	auto instance = reinterpret_cast<ThisCall*>(klass);
	if constexpr (std::is_void_v<T>) {
		(instance->*func)(std::forward<Args>(args)...);
	} else {
		return (instance->*func)(std::forward<Args>(args)...);
	}
}

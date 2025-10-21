#pragma once

enum class HookMode : uint8 {
	Pre,
	Post,
	Count,
};

enum class ResultType {
	Continue = 0,
	Changed = 1,
	Handled = 2,
	Stop = 3,
};

struct NullMutex {
	void lock() const {}
	void unlock() const {}
	bool try_lock() const { return true; }

	void lock_shared() const noexcept {}
	void unlock_shared() const noexcept {}
	bool try_lock_shared() const noexcept { return true; }
};

template<class Sig, class MutexT = NullMutex>
class ListenerManager;

template<class Ret, class... Args, class Mutex>
class ListenerManager<Ret(*)(Args...), Mutex> {
public:
    static constexpr size_t N = 62;
	using Func = Ret(*)(Args...);
	using UniqueLock = std::unique_lock<Mutex>;
	using SharedLock = std::shared_lock<Mutex>;

	bool Register(const Func& handler, ptrdiff_t priority = 0) {
		UniqueLock lock(m_mutex);
		auto it = std::upper_bound(m_priorities.begin(), m_priorities.end(), priority,
								   [](int p, int cur){ return p > cur; });
		auto index = std::distance(m_priorities.begin(), it);
		m_handlers.insert(m_handlers.begin() + index, handler);
		m_priorities.insert(m_priorities.begin() + index, priority);
		return true;
	}

	bool Unregister(const Func& handler) {
		UniqueLock lock(m_mutex);
		auto it = std::find(m_handlers.begin(), m_handlers.end(), handler);
		if (it == m_handlers.end()) return false;
        auto index = std::distance(m_handlers.begin(), it);
		m_handlers.erase(m_handlers.begin() + index);
		m_priorities.erase(m_priorities.begin() + index);
		return true;
	}

	void operator()(Args... args) {
		plg::hybrid_vector<Func, N> funcs;
		{
			SharedLock lock(m_mutex);
			funcs = m_handlers;
		}
		for (auto& func : funcs) func(std::forward<Args>(args)...);
	}

	void Clear() {
		UniqueLock lock(m_mutex);
		m_handlers.clear();
		m_priorities.clear();
	}

	plg::hybrid_vector<Func, N> Get() const {
		SharedLock lock(m_mutex);
		return m_handlers;
	}

	bool Empty() const {
		SharedLock lock(m_mutex);
		return m_handlers.empty();
	}

private:
	alignas(std::hardware_destructive_interference_size)
	plg::hybrid_vector<Func, N> m_handlers;
	plg::hybrid_vector<int, N> m_priorities;
	PLUGIFY_NO_UNIQUE_ADDRESS mutable Mutex m_mutex;
};

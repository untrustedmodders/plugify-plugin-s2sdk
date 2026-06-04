#pragma once

/**
 * Whether to hook the message in the post mode (after processing) or pre mode (before processing).
 */
enum class HookMode : uint8_t {
	Pre, // Callback will be executed before the original function
	Post, // Callback will be executed after the original function
	Count,
};

/**
 * Enum representing the possible results of an operation.
 */
enum class ResultType {
	Continue = 0, // The action continues to be processed without interruption.
	Changed = 1, // Indicates that the action has altered the state or behavior during execution.
	Handled = 2, // The action has been successfully handled, and no further action is required.
	Stop = 3, // The action processing is halted, and no further steps will be executed.
};

struct NullMutex {
	void lock() const noexcept {}
	void unlock() const noexcept {}
	bool try_lock() const noexcept { return true; }

	void lock_shared() const noexcept {}
	void unlock_shared() const noexcept {}
	bool try_lock_shared() const noexcept { return true; }
};

template<const char* Name, class Sig, class MutexT = NullMutex>
class ListenerManager;

template<const char* Name, class Ret, class... Args, class Mutex>
class ListenerManager<Name, Ret(*)(Args...), Mutex> {
public:
	ListenerManager() = default;
	~ListenerManager() = default;
	NONCOPYABLE(ListenerManager)

    static constexpr size_t N = 62;
	using Func = Ret(*)(Args...);
	using UniqueLock = std::unique_lock<Mutex>;
	using SharedLock = std::shared_lock<Mutex>;

	bool Register(const Func& handler, int priority = 0) {
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

	auto operator()(Args... args, const plg::source_location& loc = plg::source_location::current()) {
		[[maybe_unused]] plg::Scope zone(Name, loc);

		plg::hybrid_vector<Func, N> funcs;
		{
			SharedLock lock(m_mutex);
			funcs = m_handlers;
		}

		return Dispatch(funcs, std::forward<Args>(args)...);
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

protected:
	void Dispatch(const auto& funcs, Args&&... args) requires (!std::same_as<Ret, bool>) {
		for (const auto& f : funcs)
			f(std::forward<Args>(args)...);
	}

	bool Dispatch(const auto& funcs, Args&&... args) requires (std::same_as<Ret, bool>) {
		bool result = false;

		for (const auto& f : funcs)
			result |= !f(std::forward<Args>(args)...);

		return result;
	}

private:
	plg::hybrid_vector<Func, N> m_handlers;
	plg::hybrid_vector<int, N> m_priorities;
	PLUGIFY_NO_UNIQUE_ADDRESS mutable Mutex m_mutex;
};

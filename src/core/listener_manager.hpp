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

template<const char* Name, class Sig>
class ListenerManager;

template<const char* Name, class Ret, class... Args>
class ListenerManager<Name, Ret(*)(Args...)> {
public:
	ListenerManager() = default;
	~ListenerManager() = default;
	NONCOPYABLE(ListenerManager)

	using Func = Ret(*)(Args...);

	bool Register(const Func& handler, int priority = 0) {
		std::unique_lock lock(m_mutex, std::try_to_lock);
		if (!lock.owns_lock()) {
			std::unique_lock lk(m_mut);
			m_pending.emplace_back(
				PendingOp::Mode::Add,
				priority,
				handler
			);
			return true;
		}
		return Add(handler, priority);
	}

	bool Unregister(const Func& handler) {
		std::unique_lock lock(m_mutex, std::try_to_lock);
		if (!lock.owns_lock()) {
			std::unique_lock lk(m_mut);
			m_pending.emplace_back(
				PendingOp::Mode::Remove,
				-1,
				handler
			);
			return true;
		}
		return Remove(handler);
	}

	auto operator()(Args... args, const plg::source_location& loc = plg::source_location::current()) {
		[[maybe_unused]] plg::Scope zone(Name, loc);
		[[maybe_unused]] plg::scope_guard guard = plg::make_scope_guard([&]{ ApplyPending(); });
		std::shared_lock lock(m_mutex);
		return Dispatch(m_handlers, std::forward<Args>(args)...);
	}

	void Clear() {
		std::unique_lock lock(m_mutex);
		m_handlers.clear();
		m_priorities.clear();
	}

	std::vector<Func> Get() const {
		std::shared_lock lock(m_mutex);
		return m_handlers;
	}

	bool Empty() const {
		std::shared_lock lock(m_mutex);
		return m_handlers.empty();
	}

protected:
	bool Add(const Func& handler, int priority) {
		auto it = std::ranges::upper_bound(m_priorities, priority,
								   [](int p, int cur){ return p > cur; });
		auto index = std::distance(m_priorities.begin(), it);
		m_handlers.insert(m_handlers.begin() + index, handler);
		m_priorities.insert(m_priorities.begin() + index, priority);
		return true;
	}

	bool Remove(const Func& handler) {
		auto it = std::ranges::find(m_handlers, handler);
		if (it == m_handlers.end()) return false;
		auto index = std::distance(m_handlers.begin(), it);
		m_handlers.erase(m_handlers.begin() + index);
		m_priorities.erase(m_priorities.begin() + index);
		return true;
	}

	void ApplyPending() {
		std::unique_lock lk(m_mut);

		if (m_pending.empty())
			return;

		std::unique_lock lock(m_mutex);

		for (const auto& [mode, priority, handler] : m_pending) {
			switch (mode) {
				case PendingOp::Mode::Add:
					Add(handler, priority);
					break;

				case PendingOp::Mode::Remove:
					Remove(handler);
					break;
			}
		}

		m_pending.clear();
	}

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
	std::vector<Func> m_handlers;
	std::vector<int> m_priorities;
	struct PendingOp {
		enum class Mode { Add, Remove };

		Mode mode;
		int priority;
		Func handler;
	};
	std::vector<PendingOp> m_pending;
	mutable std::shared_mutex m_mutex;
	std::mutex m_mut;
};

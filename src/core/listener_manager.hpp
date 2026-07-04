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

    struct HandlerSet {
        std::vector<Func> handlers;
        std::vector<int> priorities;
    };

    bool Register(const Func& handler, int priority = 0) {
        std::unique_lock lock(m_mutex);

        auto old = m_state;
        if (old && std::ranges::any_of(old->handlers, [&](const auto& h){ return h == handler; }))
            return false;

        auto fresh = std::make_shared<HandlerSet>(old ? *old : HandlerSet{});
        auto it = std::ranges::upper_bound(fresh->priorities, priority,
            [](int p, int cur){ return p > cur; });

        auto index = std::distance(fresh->priorities.begin(), it);
        fresh->handlers.insert(fresh->handlers.begin() + index, handler);
        fresh->priorities.insert(fresh->priorities.begin() + index, priority);

        m_state = std::move(fresh);
        return true;
    }

    bool Unregister(const Func& handler) {
        std::unique_lock lock(m_mutex);

        auto old = m_state;
        if (!old)
            return false;

        auto it = std::ranges::find(old->handlers, handler);
        if (it == old->handlers.end())
            return false; // no copy made for a no-op

        auto index = std::distance(old->handlers.begin(), it);
        auto fresh = std::make_shared<HandlerSet>(*old);
        fresh->handlers.erase(fresh->handlers.begin() + index);
        fresh->priorities.erase(fresh->priorities.begin() + index);

        m_state = std::move(fresh);
        return true;
    }

    auto operator()(Args... args, const plg::source_location& loc = plg::source_location::current()) {
        [[maybe_unused]] plg::Scope zone(Name, loc);

    	std::shared_ptr<const HandlerSet> snapshot;
	    {
        	std::shared_lock lock(m_mutex);
        	snapshot = m_state;
	    }

        if (!snapshot)
            return Dispatch(std::span<Func>{}, std::forward<Args>(args)...);
        return Dispatch(snapshot->handlers, std::forward<Args>(args)...);
    }

    void Clear() {
        std::unique_lock lock(m_mutex);
        m_state.reset();
    }

    std::shared_ptr<const HandlerSet> Get() const {
        std::shared_lock lock(m_mutex);
        return m_state;
    }

    bool Empty() const {
        std::shared_lock lock(m_mutex);
        return !m_state || m_state->handlers.empty();
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
    std::shared_ptr<const HandlerSet> m_state;
    mutable std::shared_mutex m_mutex;
};
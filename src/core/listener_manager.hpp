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

template<class Sig>
class ListenerManager;

template<class Ret, class... Args>
class ListenerManager<Ret(*)(Args...)> {
public:
    static constexpr size_t kStackSize = 62; // for 512 byte object
    using Func = Ret(*)(Args...);

    bool Register(const Func& handler, ptrdiff_t priority = 0) {
        std::unique_lock lock(m_mutex);
        auto it = std::upper_bound(m_priorities.begin(), m_priorities.end(), priority,
                                   [](int p, int cur){ return p > cur; });
        auto index = std::distance(m_priorities.begin(), it);
        m_handlers.insert(m_handlers.begin() + index, handler);
        m_priorities.insert(m_priorities.begin() + index, priority);
        return true;
    }

    bool Unregister(const Func& handler) {
        std::unique_lock lock(m_mutex);
        auto it = std::find(m_handlers.begin(), m_handlers.end(), handler);
        if (it == m_handlers.end()) return false;
        auto index = std::distance(m_handlers.begin(), it);
        m_handlers.erase(m_handlers.begin() + index);
        m_priorities.erase(m_priorities.begin() + index);
        return true;
    }

    void operator()(Args... args) {
        plg::hybrid_vector<Func, kStackSize> copy;
        {
            std::shared_lock lock(m_mutex);
            copy = m_handlers;
        }
        for (auto& f : copy) f(std::forward<Args>(args)...);
    }

    void Clear() {
        std::unique_lock lock(m_mutex);
        m_handlers.clear();
    }

    plg::hybrid_vector<Func, kStackSize> Get() const {
        std::shared_lock lock(m_mutex);
        return m_handlers;
    }

    bool Empty() const {
        std::shared_lock lock(m_mutex);
        return m_handlers.empty();
    }

private:
    plg::hybrid_vector<Func, kStackSize> m_handlers;
    plg::hybrid_vector<int, kStackSize> m_priorities;
    mutable std::shared_mutex m_mutex;
};

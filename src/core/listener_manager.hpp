#pragma once

#include <concepts>

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
    using Func = Ret(*)(Args...);

    bool Register(const Func& f, ptrdiff_t priority = 0) {
        std::scoped_lock lock(m_mutex);
        if (std::any_of(m_listeners.begin(), m_listeners.end(), [&](auto& x){ return x.first == f; }))
            return false;
        m_listeners.emplace_back(f, priority);
        std::sort(m_listeners.begin(), m_listeners.end(),
                  [](const auto& a, const auto& b){ return a.second > b.second; });
        return true;
    }

    bool Unregister(const Func& f) {
        std::scoped_lock lock(m_mutex);
        auto it = std::find_if(m_listeners.begin(), m_listeners.end(),
                               [&](auto& x){ return x.first == f; });
        if (it == m_listeners.end()) return false;
        m_listeners.erase(it);
        return true;
    }

    void operator()(Args... args) {
        std::vector<Func> copy;
        {
            std::scoped_lock lock(m_mutex);
            copy.reserve(m_listeners.size());
            for (auto& p : m_listeners) copy.push_back(p.first);
        }
        for (auto& f : copy) f(std::forward<Args>(args)...);
    }

    void Clear() {
        std::scoped_lock lock(m_mutex);
        m_listeners.clear();
    }

    std::vector<Func> Get() {
        std::scoped_lock lock(m_mutex);
        std::vector<Func> result;
        result.reserve(m_listeners.size());
        for (auto& p : m_listeners) result.push_back(p.first);
        return result;
    }

    bool Empty() const {
        std::scoped_lock lock(m_mutex);
        return m_listeners.empty();
    }

private:
    mutable std::mutex m_mutex;
    std::vector<std::pair<Func, ptrdiff_t>> m_listeners;
};

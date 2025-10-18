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

    bool Register(const Func& f) {
        std::scoped_lock lock(m_mutex);
        if (std::any_of(m_listeners.begin(), m_listeners.end(), [&](auto& x){ return x == f; }))
            return false;
        m_listeners.push_back(f);
        return true;
    }

    bool Unregister(const Func& f) {
        std::scoped_lock lock(m_mutex);
        auto it = std::find_if(m_listeners.begin(), m_listeners.end(), [&](auto& x){ return x == f; });
        if (it == m_listeners.end()) return false;
        m_listeners.erase(it);
        return true;
    }

    void operator()(Args... args) {
        std::vector<Func> copy;
        {
            std::scoped_lock lock(m_mutex);
            copy = m_listeners;
        }
        for (auto& f : copy) f(std::forward<Args>(args)...);
    }

    void Clear() {
        std::scoped_lock lock(m_mutex);
        m_listeners.clear();
    }

    std::vector<Func> Get() {
        std::scoped_lock lock(m_mutex);
        return m_listeners;
    }

    bool Empty() const {
        std::scoped_lock lock(m_mutex);
        return m_listeners.empty();
    }

private:
    mutable std::mutex m_mutex;
    std::vector<Func> m_listeners;
};
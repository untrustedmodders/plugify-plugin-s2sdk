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

template<class = void>
class ListenerManager;

template<class Ret, class... Args>
class ListenerManager<Ret (*)(Args...)> {
public:
    template<typename Callable>
        requires std::invocable<Callable, Args...> &&
                 std::convertible_to<std::invoke_result_t<Callable, Args...>, Ret>
    bool Register(Callable callable)
    {
        std::scoped_lock lock(m_mutex);
        for (auto& f : m_callables) {
            if (f == callable) {
                return false;
            }
        }

        m_callables.push_back(callable);
        return true;
    }

    template<typename Callable>
        requires std::invocable<Callable, Args...> &&
                 std::convertible_to<std::invoke_result_t<Callable, Args...>, Ret>
    bool Unregister(Callable callable)
    {
        std::scoped_lock lock(m_mutex);

        for (auto it = m_callables.begin(); it != m_callables.end(); ++it) {
            if (*it == callable) {
                m_callables.erase(it);  // no need for prev
                return true;
            }
        }
        return false;
    }

    template <typename... Params>
    void operator()(Params&&... params) const {
        std::scoped_lock lock(m_mutex);
        for (const auto& f : m_callables) {
            f(std::forward<Params>(params)...);
        }
    }

    class SafeView {
    public:
        SafeView(ListenerManager* self) : m_self(*self) {
            m_self.m_mutex.lock();
        }

        ~SafeView() {
            m_self.m_mutex.unlock();
        }

        auto begin() const { return m_self.m_callables.begin(); }
        auto end() const { return m_self.m_callables.end(); }

    private:
        ListenerManager& m_self;
    };

    SafeView AsView() {
        return { this };
    }

    void Clear() {
        std::scoped_lock lock(m_mutex);
        m_callables.clear();
    }

    bool Empty() const {
        std::scoped_lock lock(m_mutex);
        return m_callables.empty();
    }

private:
    mutable std::recursive_mutex m_mutex;
    std::list<Ret (*)(Args...)> m_callables;
};

#pragma once

#include "../CallbackEventDefinitions/CallbackEventDefinitions.h"

template <typename... Args>
class MultiSubscriberEvent {
public:
    MultiSubscriberEvent() = default;
    MultiSubscriberEvent(const MultiSubscriberEvent&) = delete;
    MultiSubscriberEvent(MultiSubscriberEvent&&) = delete;
    auto operator=(const MultiSubscriberEvent&) -> MultiSubscriberEvent& = delete;
    auto operator=(MultiSubscriberEvent&&) -> MultiSubscriberEvent& = delete;
    ~MultiSubscriberEvent() = default;

public:
#pragma region Method
    // We use two template arguments to handle the case where we register a method from the parent of the class with a child class instance
    template <class T, class U>
    auto subscribe(U* instance, void (T::*method)(Args...)) -> bool {
        auto it = std::find_if(m_functionMethodPointers.begin(), m_functionMethodPointers.end(), [instance, method](const auto& methodEvent) {
            return *methodEvent == MethodEventVarying<T, Args...>(instance, method);
        });

        if (it != m_functionMethodPointers.end())
            return false;

        m_functionMethodPointers.emplace_back(std::make_unique<MethodEventVarying<T, Args...>>(instance, method));
        return true;
    }

    template <class T, class U>
    auto unsubscribe(U* instance, void (T::*method)(Args...)) -> bool {
        auto it = std::find_if(m_functionMethodPointers.begin(), m_functionMethodPointers.end(), [instance, method](const auto& methodEvent) {
            return *methodEvent == MethodEventVarying<T, Args...>(instance, method);
        });

        if (it == m_functionMethodPointers.end())
            return false;

        m_functionMethodPointers.erase(it);
        return true;
    }

    template <class T>
    auto operator+=(const MethodEventVarying<T, Args...>& methodEvent) -> bool {
        return subscribe(methodEvent);
    }

    template <class T>
    auto operator-=(const MethodEventVarying<T, Args...>& methodEvent) -> bool {
        return unsubscribe(methodEvent);
    }

#pragma endregion

#pragma region Function
    auto subscribe(FunctionPointer<Args...> function) -> bool {
        auto it = std::find_if(m_functionMethodPointers.begin(), m_functionMethodPointers.end(), [function](const auto& methodEvent) {
            return *methodEvent == FunctionEventVarying<Args...>(function);
        });

        if (it != m_functionMethodPointers.end())
            return false;

        m_functionMethodPointers.emplace_back(std::make_unique<FunctionEventVarying<Args...>>(function));
        return true;
    }

    auto unsubscribe(FunctionPointer<Args...> function) -> bool {
        auto it = std::find_if(m_functionMethodPointers.begin(), m_functionMethodPointers.end(), [function](const auto& methodEvent) {
            return *methodEvent == FunctionEventVarying<Args...>(function);
        });

        if (it == m_functionMethodPointers.end())
            return false;

        m_functionMethodPointers.erase(it);
        return true;
    }

    auto operator+=(FunctionPointer<Args...> function) -> bool {
        return subscribe(function);
    }

    auto operator-=(FunctionPointer<Args...> function) -> bool {
        return unsubscribe(function);
    }
#pragma endregion

    [[nodiscard]] auto getSubscriberCount() const -> size_t {
        return m_functionMethodPointers.size();
    }

    void clear() {
        m_functionMethodPointers.clear();
    }

    auto trigger(Args... args) const -> void {
        for (const auto& methodFunctionPointer : m_functionMethodPointers)
            (*methodFunctionPointer)(args...);
    }

private:
    std::vector<std::unique_ptr<FunctionMethodEventVaryingBase<Args...>>> m_functionMethodPointers;
};

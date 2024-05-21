#pragma once

#include "SubscriberEventBase.h"

template <typename... Args>
class SingleSubscriberEvent final : public SubscriberEventBase<Args...>
{
public:
    SingleSubscriberEvent() = default;
    SingleSubscriberEvent(const SingleSubscriberEvent&) = delete;
    SingleSubscriberEvent(SingleSubscriberEvent&&) = delete;
    auto operator=(const SingleSubscriberEvent&) -> SingleSubscriberEvent& = delete;
    auto operator=(SingleSubscriberEvent&&) -> SingleSubscriberEvent& = delete;
    ~SingleSubscriberEvent() final = default;

public:
#pragma region Method

    // We use two template arguments to handle the case where we register a method from the parent of the class with a child class instance
    template <typename T, typename U>
    auto subscribe(U* instance, void (T::*method)(Args...)) -> bool
    {
        if (m_functionMethodPointer != nullptr)
        {
            return false;
        }

        m_functionMethodPointer = std::make_unique<MethodEventVarying<T, Args...>>(instance, method);
        return true;
    }

    template <typename T, typename U>
    auto unsubscribe(U* instance, void (T::*method)(Args...)) -> bool
    {
        if (m_functionMethodPointer == nullptr)
        {
            return false;
        }

        if (*m_functionMethodPointer != MethodEventVarying<T, Args...>(instance, method))
        {
            return false;
        }

        m_functionMethodPointer.reset();
        return true;
    }

    template <typename T>
    auto operator+=(const MethodEventVarying<T, Args...>& methodEvent) -> bool
    {
        return subscribe(methodEvent);
    }

    template <typename T>
    auto operator-=(const MethodEventVarying<T, Args...>& methodEvent) -> bool
    {
        return unsubscribe(methodEvent);
    }

    template <typename T>
    auto isRegistered(const MethodEventVarying<T, Args...>& methodEvent) const -> bool
    {
        if (const auto* methodEventVarying = dynamic_cast<const MethodEventVarying<Args...>*>(&methodEvent))
        {
            return *m_functionMethodPointer == *methodEventVarying;
        }

        return false;
    }

#pragma endregion

#pragma region Function

    auto subscribe(FunctionPointer<Args...> function) -> bool
    {
        if (m_functionMethodPointer != nullptr)
        {
            return false;
        }

        m_functionMethodPointer = std::make_unique<FunctionEventVarying<Args...>>(function);
    }

    auto unsubscribe(FunctionPointer<Args...> function) -> bool
    {
        if (m_functionMethodPointer == nullptr)
        {
            return false;
        }

        if (*m_functionMethodPointer != FunctionEventVarying<Args...>(function))
        {
            return false;
        }

        m_functionMethodPointer.reset();
        return true;
    }

    auto operator+=(FunctionPointer<Args...> function) -> bool
    {
        return subscribe(function);
    }

    auto operator-=(FunctionPointer<Args...> function) -> bool
    {
        return unsubscribe(function);
    }

    template <typename T>
    auto isRegistered(FunctionPointer<Args...> function) const -> bool
    {
        return *m_functionMethodPointer == FunctionEventVarying<Args...>(function);
    }

#pragma endregion

    [[nodiscard]] auto getHasSubscriber() const -> bool
    {
        return m_functionMethodPointer != nullptr;
    }

    void clear()
    {
        m_functionMethodPointer.reset();
    }

    void trigger(Args... args) const
    {
        (*m_functionMethodPointer)(args...);
    }

    [[nodiscard]] auto triggerSafely(Args... args) const -> bool
    {
        if (m_functionMethodPointer == nullptr)
        {
            return false;
        }

        trigger(args...);
        return true;
    }

private:
    std::unique_ptr<FunctionMethodEventVaryingBase<Args...>> m_functionMethodPointer = nullptr;
};

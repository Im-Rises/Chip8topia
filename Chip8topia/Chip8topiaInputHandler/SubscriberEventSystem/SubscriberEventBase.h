#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

template <typename... Args>
class FunctionMethodEventVaryingBase
{
public:
    FunctionMethodEventVaryingBase() = default;
    FunctionMethodEventVaryingBase(const FunctionMethodEventVaryingBase&) = default;
    FunctionMethodEventVaryingBase(FunctionMethodEventVaryingBase&&) noexcept = default;
    auto operator=(const FunctionMethodEventVaryingBase&) -> FunctionMethodEventVaryingBase& = default;
    auto operator=(FunctionMethodEventVaryingBase&&) noexcept -> FunctionMethodEventVaryingBase& = default;
    virtual ~FunctionMethodEventVaryingBase() = default;

    [[nodiscard]] virtual auto operator==(const FunctionMethodEventVaryingBase& other) const -> bool = 0;

    virtual void operator()(Args... args) const = 0;
};


template <typename... Args>
using FunctionPointer = void (*)(Args...);

template <class T, typename... Args>
class MethodEventVarying final : public FunctionMethodEventVaryingBase<Args...>
{
public:
    using MethodPointer = void (T::*)(Args...);

    MethodEventVarying(T* instance, MethodPointer method) : m_instance(instance), m_method(method) {}
    MethodEventVarying(const MethodEventVarying&) = default;
    MethodEventVarying(MethodEventVarying&&) noexcept = default;
    auto operator=(const MethodEventVarying&) -> MethodEventVarying& = default;
    auto operator=(MethodEventVarying&&) noexcept -> MethodEventVarying& = default;
    ~MethodEventVarying() = default;

    [[nodiscard]] auto operator==(const FunctionMethodEventVaryingBase<Args...>& other) const -> bool final
    {
        if (auto* otherCasted = dynamic_cast<const MethodEventVarying<T, Args...>*>(&other))
        {
            return m_instance == otherCasted->m_instance && m_method == otherCasted->m_method;
        }

        return false;
    }

    void operator()(Args... args) const final
    {
        (m_instance->*m_method)(args...);
    }

private:
    T* m_instance;
    void (T::*m_method)(Args...);
};

template <typename... Args>
class FunctionEventVarying final : public FunctionMethodEventVaryingBase<Args...>
{
public:
    explicit FunctionEventVarying(FunctionPointer<Args...> function) : m_function(function) {}
    FunctionEventVarying(const FunctionEventVarying&) = default;
    FunctionEventVarying(FunctionEventVarying&&) noexcept = default;
    auto operator=(const FunctionEventVarying&) -> FunctionEventVarying& = default;
    auto operator=(FunctionEventVarying&&) noexcept -> FunctionEventVarying& = default;
    ~FunctionEventVarying() = default;

    [[nodiscard]] auto operator==(const FunctionMethodEventVaryingBase<Args...>& other) const -> bool final
    {
        if (auto* otherCasted = dynamic_cast<const FunctionEventVarying<Args...>*>(&other))
        {
            return m_function == otherCasted->m_function;
        }

        return false;
    }

    void operator()(Args... args) const final
    {
        m_function(args...);
    }

private:
    FunctionPointer<> m_function;
};

template <typename... Args>
class SubscriberEventBase
{
public:
    SubscriberEventBase() = default;
    SubscriberEventBase(const SubscriberEventBase&) = default;
    SubscriberEventBase(SubscriberEventBase&&) noexcept = default;
    auto operator=(const SubscriberEventBase&) -> SubscriberEventBase& = default;
    auto operator=(SubscriberEventBase&&) noexcept -> SubscriberEventBase& = default;
    virtual ~SubscriberEventBase() = default;

public:
    virtual void clear() = 0;
    virtual void trigger(Args... args) const = 0;
};

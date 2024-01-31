#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

#pragma region FunctionMethodEventVaryingBase
template <typename... Args>
class FunctionMethodEventVaryingBase {
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
#pragma endregion

#pragma region MethodEventVarying
template <typename... Args>
using FunctionPointer = void (*)(Args...);

template <class T, typename... Args>
class MethodEventVarying final : public FunctionMethodEventVaryingBase<Args...> {
public:
    using MethodPointer = void (T::*)(Args...);

    MethodEventVarying(T* instance, MethodPointer method) : m_instance(instance), m_method(method) {}
    MethodEventVarying(const MethodEventVarying&) = default;
    MethodEventVarying(MethodEventVarying&&) noexcept = default;
    auto operator=(const MethodEventVarying&) -> MethodEventVarying& = default;
    auto operator=(MethodEventVarying&&) noexcept -> MethodEventVarying& = default;
    ~MethodEventVarying() = default;

    [[nodiscard]] auto operator==(const FunctionMethodEventVaryingBase<Args...>& other) const -> bool final {
        if (auto* otherCasted = dynamic_cast<const MethodEventVarying<T, Args...>*>(&other))
        {
            return m_instance == otherCasted->m_instance && m_method == otherCasted->m_method;
        }

        return false;
    }

    void operator()(Args... args) const final {
        (m_instance->*m_method)(args...);
    }

private:
    T* m_instance;
    void (T::*m_method)(Args...);
};
#pragma endregion

#pragma region FunctionEventVarying
template <typename... Args>
using FunctionPointer = void (*)(Args...);

template <typename... Args>
class FunctionEventVarying final : public FunctionMethodEventVaryingBase<Args...> {
public:
    explicit FunctionEventVarying(FunctionPointer<Args...> function) : m_function(function) {}
    FunctionEventVarying(const FunctionEventVarying&) = default;
    FunctionEventVarying(FunctionEventVarying&&) noexcept = default;
    auto operator=(const FunctionEventVarying&) -> FunctionEventVarying& = default;
    auto operator=(FunctionEventVarying&&) noexcept -> FunctionEventVarying& = default;
    ~FunctionEventVarying() = default;

    [[nodiscard]] auto operator==(const FunctionMethodEventVaryingBase<Args...>& other) const -> bool final {
        if (auto* otherCasted = dynamic_cast<const FunctionEventVarying<Args...>*>(&other))
        {
            return m_function == otherCasted->m_function;
        }

        return false;
    }

    void operator()(Args... args) const final {
        m_function(args...);
    }

private:
    FunctionPointer<> m_function;
};
#pragma endregion

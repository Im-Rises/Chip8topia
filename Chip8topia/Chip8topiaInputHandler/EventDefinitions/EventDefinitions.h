#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

template <typename... Args>
using FunctionPointer = void (*)(Args...);

template <typename... Args>
class MethodEventVaryingBase {
public:
    MethodEventVaryingBase() = default;
    MethodEventVaryingBase(const MethodEventVaryingBase&) = default;
    MethodEventVaryingBase(MethodEventVaryingBase&&) noexcept = default;
    auto operator=(const MethodEventVaryingBase&) -> MethodEventVaryingBase& = default;
    auto operator=(MethodEventVaryingBase&&) noexcept -> MethodEventVaryingBase& = default;
    virtual ~MethodEventVaryingBase() = default;

    virtual void operator()(Args... args) const = 0;

    virtual auto operator==(const MethodEventVaryingBase& other) const -> bool = 0;
};

template <class T, typename... Args>
class MethodEventVarying final : public MethodEventVaryingBase<Args...> {
public:
    using MethodPointer = void (T::*)(Args...);

    MethodEventVarying(T* instance, MethodPointer method) : instance(instance), method(method) {}
    MethodEventVarying(const MethodEventVarying&) = default;
    MethodEventVarying(MethodEventVarying&&) noexcept = default;
    auto operator=(const MethodEventVarying&) -> MethodEventVarying& = default;
    auto operator=(MethodEventVarying&&) noexcept -> MethodEventVarying& = default;
    ~MethodEventVarying() = default;

    void operator()(Args... args) const final {
        (instance->*method)(args...);
    }

    [[nodiscard]] auto operator==(const MethodEventVaryingBase<Args...>& other) const -> bool final {
        if (std::is_convertible_v<decltype(other), MethodEventVarying<T, Args...>>)
        {
            auto otherCasted = static_cast<const MethodEventVarying<T, Args...>&>(other);
            return instance == otherCasted.instance && method == otherCasted.method;
        }

        return true;
    }

    [[nodiscard]] auto operator==(const MethodEventVarying<T, Args...>& other) const -> bool {
        return checkObjectEquality(instance, other.instance) && checkMethodEquality(method, other.method);
    }

private:
    T* instance;
    void (T::*method)(Args...);
};

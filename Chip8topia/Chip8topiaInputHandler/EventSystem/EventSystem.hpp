#pragma once

#include <vector>
#include <unordered_set>
#include <memory>
#include <algorithm>

template <typename... Args>
using FunctionPointer = void (*)(Args...);

template <typename... Args>
class MethodEventVaryingBase {
public:
    MethodEventVaryingBase() = default;
    MethodEventVaryingBase(const MethodEventVaryingBase&) = default;
    MethodEventVaryingBase(MethodEventVaryingBase&&) = default;
    virtual ~MethodEventVaryingBase() = default;

    auto operator=(const MethodEventVaryingBase&) -> MethodEventVaryingBase& = default;
    auto operator=(MethodEventVaryingBase&&) -> MethodEventVaryingBase& = default;

    virtual void operator()(Args... args) const = 0;

    virtual auto operator==(const MethodEventVaryingBase& other) const -> bool = 0;
};

template <class T, typename... Args>
class MethodEventVarying : public MethodEventVaryingBase<Args...> {
public:
    using MethodPointer = void (T::*)(Args...);

    MethodEventVarying(T* instance, MethodPointer method) : instance(instance), method(method) {}

    void operator()(Args... args) const final {
        (instance->*method)(args...);
    }

    [[nodiscard]] auto operator==(const MethodEventVaryingBase<Args...>& other) const -> bool final {
        return operator==(static_cast<const MethodEventVarying<T>&>(other));
    }

    [[nodiscard]] auto operator==(const MethodEventVarying<T>& other) const -> bool {
        return instance == other.instance && method == other.method;
    }

private:
    T* instance;
    void (T::*method)(Args...);
};

template <typename... Args>
class EventSystem {
public:
    EventSystem() = default;
    EventSystem(const EventSystem&) = delete;
    EventSystem(EventSystem&&) = delete;
    auto operator=(const EventSystem&) -> EventSystem& = delete;
    auto operator=(EventSystem&&) -> EventSystem& = delete;
    ~EventSystem() = default;

public:
    auto subscribe(const FunctionPointer<Args...>& functionPointer) -> bool {
        auto it = std::find_if(functionsList.begin(), functionsList.end(), [&functionPointer](const auto& function) {
            return *function == functionPointer;
        });

        if (it != functionsList.end())
            return false;

        functionsList.push_back(FunctionPointer<Args...>(functionPointer));
        return true;
    }
    auto unsubscribe(const FunctionPointer<Args...>& functionPointer) -> bool {
        auto it = std::remove_if(functionsList.begin(), functionsList.end(), [&functionPointer](const auto& function) {
            return *function == functionPointer;
        });

        if (it == functionsList.end())
            return false;

        functionsList.erase(it);
        return true;
    }

    auto operator+=(const FunctionPointer<Args...>& functionPointer) -> bool {
        return subscribe(functionPointer);
    }

    auto operator-=(const FunctionPointer<Args...>& functionPointer) -> bool {
        return unsubscribe(functionPointer);
    }

    template <class T>
    auto subscribe(T* instance, void (T::*method)(Args...)) -> bool {
        auto it = std::find_if(methodsList.begin(), methodsList.end(), [instance, method](const auto& methodEvent) {
            return *methodEvent == MethodEventVarying<T, Args...>(instance, method);
        });

        if (it != methodsList.end())
            return false;

        methodsList.push_back(std::make_unique<MethodEventVarying<T, Args...>>(instance, method));
        return true;
    }

    template <class T>
    auto unsubscribe(T* instance, void (T::*method)(Args...)) -> bool {
        auto it = std::remove_if(methodsList.begin(), methodsList.end(), [instance, method](const auto& methodEvent) {
            return *methodEvent == MethodEventVarying<T, Args...>(instance, method);
        });

        if (it == methodsList.end())
            return false;

        methodsList.erase(it);
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

    auto trigger(Args... args) const -> void {
        for (const auto& function : functionsList)
            (*function)(args...);

        for (const auto& method : methodsList)
            (*method)(args...);
    }

private:
    std::vector<FunctionPointer<Args...>> functionsList;
    std::vector<std::unique_ptr<MethodEventVaryingBase<Args...>>> methodsList;
};

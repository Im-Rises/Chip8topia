#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

#pragma region Pointer definitions
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
class MethodEventVarying : public MethodEventVaryingBase<Args...> {
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
#pragma endregion

#pragma region EventSystem
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
#pragma region Functions
    auto subscribe(const FunctionPointer<Args...>& functionPointer) -> bool {
        auto it = std::find_if(functionsList.begin(), functionsList.end(), [&functionPointer](const auto& function) {
            return *function == functionPointer;
        });

        if (it != functionsList.end())
            return false;

        functionsList.emplace_back(functionPointer);
        return true;
    }

    // HACK: This is a workaround to force the impossibility of using std::function as a parameter
    auto subscribe(const std::function<void(Args...)>& function) -> bool = delete;

    auto unsubscribe(const FunctionPointer<Args...>& functionPointer) -> bool {
        auto it = std::remove_if(functionsList.begin(), functionsList.end(), [&functionPointer](const auto& function) {
            return *function == functionPointer;
        });

        if (it == functionsList.end())
            return false;

        functionsList.erase(it);
        return true;
    }

    // HACK: This is a workaround to force the impossibility of using std::function as a parameter
    auto unsubscribe(const std::function<void(Args...)>& function) -> bool = delete;

    auto operator+=(const FunctionPointer<Args...>& functionPointer) -> bool {
        return subscribe(functionPointer);
    }

    auto operator-=(const FunctionPointer<Args...>& functionPointer) -> bool {
        return unsubscribe(functionPointer);
    }
#pragma endregion

#pragma region Methods
    template <class T>
    auto subscribe(T* instance, void (T::*method)(Args...)) -> bool {
        auto it = std::find_if(methodsList.begin(), methodsList.end(), [instance, method](const auto& methodEvent) {
            return *methodEvent == MethodEventVarying<T, Args...>(instance, method);
        });

        if (it != methodsList.end())
            return false;

        methodsList.emplace_back(std::make_unique<MethodEventVarying<T, Args...>>(instance, method));
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
#pragma endregion

    auto trigger(Args... args) const -> void {
        for (const auto& function : functionsList)
            (*function)(args...);

        for (const auto& method : methodsList)
            (*method)(args...);
    }

    void clear() {
        functionsList.clear();
        methodsList.clear();
    }

    [[nodiscard]] auto getFunctionCount() const -> size_t {
        return functionsList.size();
    }

    [[nodiscard]] auto getMethodCount() const -> size_t {
        return methodsList.size();
    }

    [[nodiscard]] auto getSubscriberCount() const -> size_t {
        return getFunctionCount() + getMethodCount();
    }

private:
    std::vector<FunctionPointer<Args...>> functionsList;
    std::vector<std::unique_ptr<MethodEventVaryingBase<Args...>>> methodsList;
};
#pragma endregion

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

    MethodEventVarying(T* instance, MethodPointer method) : m_instance(instance), m_method(method) {}
    MethodEventVarying(const MethodEventVarying&) = default;
    MethodEventVarying(MethodEventVarying&&) noexcept = default;
    auto operator=(const MethodEventVarying&) -> MethodEventVarying& = default;
    auto operator=(MethodEventVarying&&) noexcept -> MethodEventVarying& = default;
    ~MethodEventVarying() = default;

    void operator()(Args... args) const final {
        (m_instance->*m_method)(args...);
    }

    [[nodiscard]] auto operator==(const MethodEventVaryingBase<Args...>& other) const -> bool final {
        if (std::is_convertible_v<decltype(other), MethodEventVarying<T, Args...>>)
        {
            auto otherCasted = static_cast<const MethodEventVarying<T, Args...>&>(other);
            return m_instance == otherCasted.m_instance && m_method == otherCasted.m_method;
        }

        return true;
    }

    [[nodiscard]] auto operator==(const MethodEventVarying<T, Args...>& other) const -> bool {
        return checkObjectEquality(m_instance, other.m_instance) && checkMethodEquality(m_method, other.m_method);
    }

private:
    T* m_instance;
    void (T::*m_method)(Args...);
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
#pragma region Functions
    auto subscribe(const FunctionPointer<Args...>& functionPointer) -> bool {
        auto it = std::find_if(m_functionsList.begin(), m_functionsList.end(), [&functionPointer](const auto& function) {
            return *function == functionPointer;
        });

        if (it != m_functionsList.end())
            return false;

        m_functionsList.emplace_back(functionPointer);
        return true;
    }

    // HACK: This is a workaround to force the impossibility of using std::function as a parameter
    auto subscribe(const std::function<void(Args...)>& function) -> bool = delete;

    auto unsubscribe(const FunctionPointer<Args...>& functionPointer) -> bool {
        auto it = std::remove_if(m_functionsList.begin(), m_functionsList.end(), [&functionPointer](const auto& function) {
            return *function == functionPointer;
        });

        if (it == m_functionsList.end())
            return false;

        m_functionsList.erase(it);
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
        auto it = std::find_if(m_methodsList.begin(), m_methodsList.end(), [instance, method](const auto& methodEvent) {
            return *methodEvent == MethodEventVarying<T, Args...>(instance, method);
        });

        if (it != m_methodsList.end())
            return false;

        m_methodsList.emplace_back(std::make_unique<MethodEventVarying<T, Args...>>(instance, method));
        return true;
    }

    template <class T>
    auto unsubscribe(T* instance, void (T::*method)(Args...)) -> bool {
        auto it = std::remove_if(m_methodsList.begin(), m_methodsList.end(), [instance, method](const auto& methodEvent) {
            return *methodEvent == MethodEventVarying<T, Args...>(instance, method);
        });

        if (it == m_methodsList.end())
            return false;

        m_methodsList.erase(it);
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
        for (const auto& function : m_functionsList)
            (*function)(args...);

        for (const auto& method : m_methodsList)
            (*method)(args...);
    }

    void clear() {
        m_functionsList.clear();
        m_methodsList.clear();
    }

    [[nodiscard]] auto getFunctionCount() const -> size_t {
        return m_functionsList.size();
    }

    [[nodiscard]] auto getMethodCount() const -> size_t {
        return m_methodsList.size();
    }

    [[nodiscard]] auto getSubscriberCount() const -> size_t {
        return getFunctionCount() + getMethodCount();
    }

private:
    std::vector<FunctionPointer<Args...>> m_functionsList;
    std::vector<std::unique_ptr<MethodEventVaryingBase<Args...>>> m_methodsList;
};

#pragma once

template <typename T>
class Singleton {
public:
    static auto getInstance() -> T& {
        static T instance;
        return instance;
    }

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

public:
    Singleton(Singleton const&) = delete;
    auto operator=(Singleton const&) -> Singleton& = delete;
    Singleton(Singleton&&) = delete;
    auto operator=(Singleton&&) -> Singleton& = delete;
};

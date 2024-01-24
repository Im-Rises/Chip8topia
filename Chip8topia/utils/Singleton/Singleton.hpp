#pragma once

template <typename T>
class Singleton {
public:
    static auto getInstance() -> T& {
        static T instance;
        return instance;
    }

public:
    Singleton(Singleton const&) = delete;
    auto operator=(Singleton const&) -> Singleton& = delete;
    Singleton(Singleton&&) = delete;
    auto operator=(Singleton&&) -> Singleton& = delete;

protected:
    Singleton() = default;

    virtual ~Singleton() = default;
};

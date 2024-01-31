#pragma once

class SubscriberEventBase {
public:
    SubscriberEventBase() = default;
    SubscriberEventBase(const SubscriberEventBase&) = default;
    SubscriberEventBase(SubscriberEventBase&&) noexcept = default;
    auto operator=(const SubscriberEventBase&) -> SubscriberEventBase& = default;
    auto operator=(SubscriberEventBase&&) noexcept -> SubscriberEventBase& = default;
    virtual ~SubscriberEventBase() = default;

        [[nodiscard]] virtual auto operator==(const SubscriberEventBase& other) const -> bool = 0;

        virtual void clear() = 0;

        virtual void trigger() = 0;
};
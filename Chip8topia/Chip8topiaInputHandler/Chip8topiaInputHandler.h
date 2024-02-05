#pragma once

#include <binaryLib/binaryLib.h>
#include <Singleton/Singleton.h>

#include "SubscriberEventSystem/MultiSubscriberEvent.h"
#include "SubscriberEventSystem/SingleSubscriberEvent.h"

struct GLFWwindow;
class Chip8topiaInputHandler final : public Singleton<Chip8topiaInputHandler> {
    friend class Singleton<Chip8topiaInputHandler>;

protected:
    Chip8topiaInputHandler() = default;
    ~Chip8topiaInputHandler() final = default;

public:
    Chip8topiaInputHandler(const Chip8topiaInputHandler&) = delete;
    auto operator=(const Chip8topiaInputHandler&) -> Chip8topiaInputHandler& = delete;
    Chip8topiaInputHandler(Chip8topiaInputHandler&&) = delete;
    auto operator=(Chip8topiaInputHandler&&) -> Chip8topiaInputHandler& = delete;

public:
    void update(GLFWwindow* window) const;
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
    [[maybe_unused]] SingleSubscriberEvent<> m_F1KeyButtonPressedEvent;
    [[maybe_unused]] SingleSubscriberEvent<> m_F2KeyButtonPressedEvent;
    [[maybe_unused]] SingleSubscriberEvent<> m_F3KeyButtonPressedEvent;
    [[maybe_unused]] SingleSubscriberEvent<> m_F10KeyButtonPressedEvent;
    [[maybe_unused]] SingleSubscriberEvent<> m_F11KeyButtonPressedEvent;
#ifndef NDEBUG
    [[maybe_unused]] SingleSubscriberEvent<> m_F12KeyDebugButtonPressedEvent;
#endif

    [[maybe_unused]] SingleSubscriberEvent<> m_CTRL_OKeyButtonPressedEvent;

    [[maybe_unused]] SingleSubscriberEvent<> m_EscapeKeyButtonPressedEvent;

    [[maybe_unused]] MultiSubscriberEvent<> m_PKeyButtonPressedEvent;

    [[maybe_unused]] MultiSubscriberEvent<const uint8, const bool> m_GameInput;

private:
};

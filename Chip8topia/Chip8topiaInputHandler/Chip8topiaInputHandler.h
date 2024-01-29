#pragma once

#include "Singleton/Singleton.h"
#include <binaryLib/binaryLib.h>

#include "EventSystem/EventSystem.h"

/*
 * TODO: Replace code with a Singleton Registry
 * TODO: replace with a std::array to handle key up and key down
 * */

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
    void update(GLFWwindow* window);

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
    [[maybe_unused]] EventSystem<> m_EscapeKeyButtonPressedEvent;
    [[maybe_unused]] EventSystem<> m_F10KeyButtonPressedEvent;
    [[maybe_unused]] EventSystem<> m_F11KeyButtonPressedEvent;
    [[maybe_unused]] EventSystem<> m_F12KeyButtonPressedEvent;
    [[maybe_unused]] EventSystem<> m_CTRL_OKeyButtonPressedEvent;

    [[maybe_unused]] EventSystem<const uint8, const bool> m_GameInput;

private:
};

#pragma once

#include "Singleton/Singleton.h"
#include <binaryLib/binaryLib.h>

#include "EventSystem/EventSystem.h"

/*
 * TODO: Replace code with a Singleton Registry
 * */

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
    //    [[maybe_unused]] EventSystem<const bool> m_LeftMouseButtonPressedEvent;
    //    [[maybe_unused]] EventSystem<const bool> m_LeftMouseButtonReleasedEvent;
    //    [[maybe_unused]] EventSystem<const bool> m_LeftMouseButtonHeldEvent;
    //
    //    [[maybe_unused]] EventSystem<const bool> m_RightMouseButtonPressedEvent;
    //    [[maybe_unused]] EventSystem<const bool> m_RightMouseButtonReleasedEvent;
    //    [[maybe_unused]] EventSystem<const bool> m_RightMouseButtonHeldEvent;
    //
    //    [[maybe_unused]] EventSystem<const bool> m_MiddleMouseButtonPressedEvent;
    //    [[maybe_unused]] EventSystem<const bool> m_MiddleMouseButtonReleasedEvent;
    //    [[maybe_unused]] EventSystem<const bool> m_MiddleMouseButtonHeldEvent;
    //
    //    [[maybe_unused]] EventSystem<const bool> m_MouseMovedEvent;
    //    [[maybe_unused]] EventSystem<const bool> m_MouseScrolledEvent;

    //    [[maybe_unused]] EventSystem<bool> m_KeyPressedEvent;
    //    [[maybe_unused]] EventSystem<bool> m_KeyReleasedEvent;
    //    [[maybe_unused]] EventSystem<bool> m_KeyHeldEvent;
    //    [[maybe_unused]] EventSystem<bool> m_KeyRepeatedEvent;

    [[maybe_unused]] EventSystem<> m_EscapeKeyButtonPressedEvent;
    [[maybe_unused]] EventSystem<> m_F11KeyButtonPressedEvent;
    [[maybe_unused]] EventSystem<> m_F12KeyButtonPressedEvent;

    [[maybe_unused]] EventSystem<const uint8, const bool> m_GameInput;
    //    [[maybe_unused]] EventSystem<> m_GameInput;
};

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
#ifndef __EMSCRIPTEN__
    [[maybe_unused]] SingleSubscriberEvent<> m_OpenRomExplorerEvent;
    [[maybe_unused]] SingleSubscriberEvent<> m_ExitChip8topiaEvent;
    [[maybe_unused]] SingleSubscriberEvent<> m_ToggleMainBarEvent;
    [[maybe_unused]] SingleSubscriberEvent<> m_ToggleWindowsVisibilityEvent;
    [[maybe_unused]] SingleSubscriberEvent<> m_ToggleTurboModeEvent;
    [[maybe_unused]] SingleSubscriberEvent<> m_CenterWindowEvent;
    [[maybe_unused]] SingleSubscriberEvent<> m_ToggleFullScreenEvent;
#endif
#if !defined(BUILD_RELEASE)
    [[maybe_unused]] SingleSubscriberEvent<> m_DebugRomFastLoadEvent;
#endif

    [[maybe_unused]] SingleSubscriberEvent<> m_PauseEmulationEvent;
    [[maybe_unused]] SingleSubscriberEvent<> m_RestartEmulationEvent;
    [[maybe_unused]] SingleSubscriberEvent<const uint8, const bool> m_GameInput;
};

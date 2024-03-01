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
    SingleSubscriberEvent<> m_ExitChip8topiaEvent;
    SingleSubscriberEvent<> m_ToggleMainBarEvent;
    SingleSubscriberEvent<> m_ToggleWindowsVisibilityEvent;
    SingleSubscriberEvent<> m_ToggleTurboModeEvent;
    SingleSubscriberEvent<> m_CenterWindowEvent;
    SingleSubscriberEvent<> m_ToggleFullScreenEvent;
#endif
#if !defined(BUILD_RELEASE)
    SingleSubscriberEvent<> m_DebugRomFastLoadEvent;
#endif
    SingleSubscriberEvent<> m_BreakEmulationEvent;
    SingleSubscriberEvent<> m_StepEmulationEvent;
    SingleSubscriberEvent<> m_RunEmulationEvent;
    SingleSubscriberEvent<> m_ClearBreakpointsEvent;
    SingleSubscriberEvent<> m_RestartEmulationEvent;

    SingleSubscriberEvent<> m_OpenRomExplorerEvent;

    SingleSubscriberEvent<const uint8, const bool> m_GameInput;
};

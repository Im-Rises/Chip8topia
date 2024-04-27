#pragma once

#include <string>
#include <SDL.h>
#include <fmt/format.h>
#include <binaryLib/binaryLib.h>
#include <Singleton/Singleton.h>

#include "SubscriberEventSystem/MultiSubscriberEvent.h"
#include "SubscriberEventSystem/SingleSubscriberEvent.h"

class Chip8topia;
class Chip8topiaInputHandler final : public Singleton<Chip8topiaInputHandler>
{
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
    void update(Chip8topia& chip8topia, SDL_Event& event) const;

    [[nodiscard]] auto getInputEnabled() const -> bool;
    void setInputEnabled(const bool inputEnabled);

public:
    /* UI Events */
    SingleSubscriberEvent<> m_BreakEmulationEvent;
    SingleSubscriberEvent<> m_StepEmulationEvent;
    SingleSubscriberEvent<> m_RunEmulationEvent;
    SingleSubscriberEvent<> m_ClearBreakpointsEvent;
    SingleSubscriberEvent<const std::string&> m_LoadRomFromPath;

    /* Input Events */
#ifndef __EMSCRIPTEN__
    //    SingleSubscriberEvent<> m_ExitChip8topiaEvent;
    SingleSubscriberEvent<> m_CenterWindowEvent;
    SingleSubscriberEvent<> m_ToggleFullScreenEvent;
#endif
#if !defined(BUILD_RELEASE)
    SingleSubscriberEvent<> m_DebugRomFastLoadEvent;
#endif
    SingleSubscriberEvent<> m_ToggleTurboModeEvent;
    SingleSubscriberEvent<> m_ToggleMainBarEvent;
    SingleSubscriberEvent<> m_CloseAllWindowsEvent;
    SingleSubscriberEvent<> m_TogglePauseEmulationEvent;
    SingleSubscriberEvent<> m_RestartEmulationEvent;
    SingleSubscriberEvent<> m_OpenRomExplorerEvent;

    SingleSubscriberEvent<const uint8, const bool> m_GameInput;

    SingleSubscriberEvent<const std::string&, std::function<void()>> m_SuccessEvent;
    SingleSubscriberEvent<const std::string&, std::function<void()>> m_ErrorEvent;
    SingleSubscriberEvent<const std::string&, std::function<void()>> m_WarningEvent;
    SingleSubscriberEvent<const std::string&, std::function<void()>> m_InfoEvent;

    MultiSubscriberEvent<const std::string&> m_EmulationError;

private:
    bool m_inputEnabled = true;
};

// #define CHIP8TOPIA_INPUT_HANDLER Chip8topiaInputHandler::getInstance()

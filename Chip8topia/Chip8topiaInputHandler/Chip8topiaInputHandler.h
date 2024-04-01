#pragma once

#include <string>
#include <binaryLib/binaryLib.h>
#include <Singleton/Singleton.h>

#include "SubscriberEventSystem/MultiSubscriberEvent.h"
#include "SubscriberEventSystem/SingleSubscriberEvent.h"

struct GLFWwindow;
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
    void update(GLFWwindow* window) const;
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    [[nodiscard]] auto getInputEnabled() const -> bool { return m_inputEnabled; }
    void setInputEnabled(const bool inputEnabled) { m_inputEnabled = inputEnabled; }

public:
    /* UI Events */
    //    SingleSubscriberEvent<> m_StopEmulationEvent;
    SingleSubscriberEvent<> m_BreakEmulationEvent;
    SingleSubscriberEvent<> m_StepEmulationEvent;
    SingleSubscriberEvent<> m_RunEmulationEvent;
    SingleSubscriberEvent<> m_ClearBreakpointsEvent;

    /* Input Events */
#ifndef __EMSCRIPTEN__
    SingleSubscriberEvent<> m_ExitChip8topiaEvent;
    SingleSubscriberEvent<> m_ToggleTurboModeEvent;
    SingleSubscriberEvent<> m_CenterWindowEvent;
    SingleSubscriberEvent<> m_ToggleFullScreenEvent;
#endif
#if !defined(BUILD_RELEASE)
    SingleSubscriberEvent<> m_DebugRomFastLoadEvent;
#endif
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

#if defined(BUILD_PARAM_SAFE)
#define TRIGGER_EMULATION_ERROR(condition, message)                                                              \
    do                                                                                                           \
    {                                                                                                            \
        static_assert(std::is_same<decltype(condition), bool>::value, "Condition must be a boolean expression"); \
        if ((condition))                                                                                         \
        {                                                                                                        \
            Chip8topiaInputHandler::getInstance().m_EmulationError.trigger((message));                           \
        }                                                                                                        \
    } while (false)
#else
#define TRIGGER_EMULATION_ERROR(condition, message) \
    do                                              \
    {                                               \
        (void)(message);                            \
    } while (false)
#endif
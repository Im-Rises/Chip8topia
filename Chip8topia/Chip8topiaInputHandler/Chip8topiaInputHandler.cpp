#include "Chip8topiaInputHandler.h"

#include <GLFW/glfw3.h>

void Chip8topiaInputHandler::update(GLFWwindow* window) const {
    glfwPollEvents();

    //    auto& inputHandler = Chip8topiaInputHandler::getInstance();
    //
    //    inputHandler.m_GameInput.trigger(0x1, glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS);
}

void Chip8topiaInputHandler::key_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int mods) {
    auto& inputHandler = Chip8topiaInputHandler::getInstance();

#ifndef __EMSCRIPTEN__
    if (action == GLFW_PRESS && (key == GLFW_KEY_O && mods == GLFW_MOD_CONTROL))
    {
        inputHandler.m_OpenRomExplorerEvent.trigger();
        return;
    }
#endif

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
#ifndef __EMSCRIPTEN__
        case GLFW_KEY_ESCAPE: inputHandler.m_ExitChip8topiaEvent.trigger(); break;

        case GLFW_KEY_F1: inputHandler.m_ToggleMainBarEvent.trigger(); break;
        case GLFW_KEY_F2: inputHandler.m_ToggleWindowsVisibilityEvent.trigger(); break;
        case GLFW_KEY_F3: inputHandler.m_ToggleTurboModeEvent.trigger(); break;
        case GLFW_KEY_F10: inputHandler.m_CenterWindowEvent.trigger(); break;
        case GLFW_KEY_F11: inputHandler.m_ToggleFullScreenEvent.trigger(); break;
#endif
#if !defined(BUILD_RELEASE)
        case GLFW_KEY_F12: inputHandler.m_DebugRomFastLoadEvent.trigger(); break;
#endif

        case GLFW_KEY_P: inputHandler.m_PauseEmulationEvent.trigger(); break;
        case GLFW_KEY_L: inputHandler.m_RestartEmulationEvent.trigger(); break;

        case GLFW_KEY_1: inputHandler.m_GameInput.trigger(0x1, true); break;
        case GLFW_KEY_2: inputHandler.m_GameInput.trigger(0x2, true); break;
        case GLFW_KEY_3: inputHandler.m_GameInput.trigger(0x3, true); break;
        case GLFW_KEY_4: inputHandler.m_GameInput.trigger(0xC, true); break;
        case GLFW_KEY_Q: inputHandler.m_GameInput.trigger(0x4, true); break;
        case GLFW_KEY_W: inputHandler.m_GameInput.trigger(0x5, true); break;
        case GLFW_KEY_E: inputHandler.m_GameInput.trigger(0x6, true); break;
        case GLFW_KEY_R: inputHandler.m_GameInput.trigger(0xD, true); break;
        case GLFW_KEY_A: inputHandler.m_GameInput.trigger(0x7, true); break;
        case GLFW_KEY_S: inputHandler.m_GameInput.trigger(0x8, true); break;
        case GLFW_KEY_D: inputHandler.m_GameInput.trigger(0x9, true); break;
        case GLFW_KEY_F: inputHandler.m_GameInput.trigger(0xE, true); break;
        case GLFW_KEY_Z: inputHandler.m_GameInput.trigger(0xA, true); break;
        case GLFW_KEY_X: inputHandler.m_GameInput.trigger(0x0, true); break;
        case GLFW_KEY_C: inputHandler.m_GameInput.trigger(0xB, true); break;
        case GLFW_KEY_V: inputHandler.m_GameInput.trigger(0xF, true); break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_1: inputHandler.m_GameInput.trigger(0x1, false); break;
        case GLFW_KEY_2: inputHandler.m_GameInput.trigger(0x2, false); break;
        case GLFW_KEY_3: inputHandler.m_GameInput.trigger(0x3, false); break;
        case GLFW_KEY_4: inputHandler.m_GameInput.trigger(0xC, false); break;
        case GLFW_KEY_Q: inputHandler.m_GameInput.trigger(0x4, false); break;
        case GLFW_KEY_W: inputHandler.m_GameInput.trigger(0x5, false); break;
        case GLFW_KEY_E: inputHandler.m_GameInput.trigger(0x6, false); break;
        case GLFW_KEY_R: inputHandler.m_GameInput.trigger(0xD, false); break;
        case GLFW_KEY_A: inputHandler.m_GameInput.trigger(0x7, false); break;
        case GLFW_KEY_S: inputHandler.m_GameInput.trigger(0x8, false); break;
        case GLFW_KEY_D: inputHandler.m_GameInput.trigger(0x9, false); break;
        case GLFW_KEY_F: inputHandler.m_GameInput.trigger(0xE, false); break;
        case GLFW_KEY_Z: inputHandler.m_GameInput.trigger(0xA, false); break;
        case GLFW_KEY_X: inputHandler.m_GameInput.trigger(0x0, false); break;
        case GLFW_KEY_C: inputHandler.m_GameInput.trigger(0xB, false); break;
        case GLFW_KEY_V: inputHandler.m_GameInput.trigger(0xF, false); break;
        }
    }
}

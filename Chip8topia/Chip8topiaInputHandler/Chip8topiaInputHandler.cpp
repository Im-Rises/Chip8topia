#include "Chip8topiaInputHandler.h"

#include <GLFW/glfw3.h>

void Chip8topiaInputHandler::update(GLFWwindow* window) const {
    glfwPollEvents();

    //    auto& inputHandler = Chip8topiaInputHandler::getInstance();
    //
    //    inputHandler.m_GameInput.trigger(0x1, glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0x2, glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0x3, glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0xC, glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS);
    //
    //    inputHandler.m_GameInput.trigger(0x4, glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0x5, glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0x6, glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0xD, glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);
    //
    //    inputHandler.m_GameInput.trigger(0x7, glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0x8, glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0x9, glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0xE, glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);
    //
    //    inputHandler.m_GameInput.trigger(0xA, glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0x0, glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0xB, glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS);
    //    inputHandler.m_GameInput.trigger(0xF, glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS);
}

void Chip8topiaInputHandler::key_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int mods) {
    auto& inputHandler = Chip8topiaInputHandler::getInstance();

    if (action == GLFW_PRESS && (key == GLFW_KEY_O && mods == GLFW_MOD_CONTROL))
    {
        inputHandler.m_CTRL_OKeyButtonPressedEvent.trigger();
        return;
    }

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE: inputHandler.m_EscapeKeyButtonPressedEvent.trigger(); break;
        case GLFW_KEY_P: inputHandler.m_PKeyButtonPressedEvent.trigger(); break;
        case GLFW_KEY_F1: inputHandler.m_F1KeyButtonPressedEvent.trigger(); break;
        case GLFW_KEY_F2: inputHandler.m_F2KeyButtonPressedEvent.trigger(); break;
        case GLFW_KEY_F3: inputHandler.m_F3KeyButtonPressedEvent.trigger(); break;
        case GLFW_KEY_F10: inputHandler.m_F10KeyButtonPressedEvent.trigger(); break;
        case GLFW_KEY_F11: inputHandler.m_F11KeyButtonPressedEvent.trigger(); break;
#if !defined(BUILD_RELEASE)
        case GLFW_KEY_F12: inputHandler.m_F12KeyDebugButtonPressedEvent.trigger(); break;
#endif
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

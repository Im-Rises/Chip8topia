#include "Chip8topiaInputHandler.h"

#include <glfw/glfw3.h>

#include "../Chip8topia.h"

void Chip8topiaInputHandler::update(GLFWwindow* window) {
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        m_CTRL_OKeyButtonPressedEvent.trigger();

    //    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    //        m_GameInput.trigger(0x1, true);
    //    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    //        m_GameInput.trigger(0x2, true);
    //    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    //        m_GameInput.trigger(0x3, true);
    //    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    //        m_GameInput.trigger(0xC, true);
    //
    //    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    //        m_GameInput.trigger(0x4, true);
    //    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    //        m_GameInput.trigger(0x5, true);
    //    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    //        m_GameInput.trigger(0x6, true);
    //    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    //        m_GameInput.trigger(0xD, true);
    //
    //    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    //        m_GameInput.trigger(0x7, true);
    //    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    //        m_GameInput.trigger(0x8, true);
    //    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    //        m_GameInput.trigger(0x9, true);
    //    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    //        m_GameInput.trigger(0xE, true);
    //
    //    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    //        m_GameInput.trigger(0xA, true);
    //    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    //        m_GameInput.trigger(0x0, true);
    //    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    //        m_GameInput.trigger(0xB, true);
    //    if (key == GLFW_KEY_V && action == GLFW_PRESS)
    //        m_GameInput.trigger(0xF, true);
    //
    //    if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
    //        m_GameInput.trigger(0x1, false);
    //    if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
    //        m_GameInput.trigger(0x2, false);
    //    if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
    //        m_GameInput.trigger(0x3, false);
    //    if (key == GLFW_KEY_4 && action == GLFW_RELEASE)
    //        m_GameInput.trigger(0xC, false);
}
void Chip8topiaInputHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto& inputHandler = Chip8topiaInputHandler::getInstance();

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
            inputHandler.m_EscapeKeyButtonPressedEvent.trigger();
        break;
    case GLFW_KEY_P:
        if (action == GLFW_PRESS)
            inputHandler.m_PKeyButtonPressedEvent.trigger();
        break;
    case GLFW_KEY_F9:
        if (action == GLFW_PRESS)
            inputHandler.m_F9KeyButtonPressedEvent.trigger();
        break;
    case GLFW_KEY_F10:
        if (action == GLFW_PRESS)
            inputHandler.m_F10KeyButtonPressedEvent.trigger();
        break;
    case GLFW_KEY_F11:
        if (action == GLFW_PRESS)
            inputHandler.m_F11KeyButtonPressedEvent.trigger();
        break;
    case GLFW_KEY_F12:
        if (action == GLFW_PRESS)
            inputHandler.m_F12KeyButtonPressedEvent.trigger();
        break;
    }
}

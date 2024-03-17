#include "Input.h"

#if defined(BUILD_PARAM_SAFE)
void Input::setErrorCallback(const std::function<void(const std::string&)>& errorCallback)
{
    m_errorCallback = errorCallback;
}
#endif

void Input::reset()
{
    m_pressedKeys.reset();
}

auto Input::isAnyKeyPressed() const -> bool
{
    return m_pressedKeys.any();
}

auto Input::isKeyPressed(const uint8 Vx) const -> bool
{
#if defined(BUILD_PARAM_SAFE)
    if (Vx >= KEY_COUNT)
    {
        m_errorCallback("Input::isKeyPressed - Vx out of range"); // TODO: Find a way to immediately return from here
        return false;
    }
#endif

    return m_pressedKeys[Vx];
}

auto Input::getPressedKey() const -> uint8
{
    for (uint8 i = 0; i < KEY_COUNT; i++)
    {
        if (m_pressedKeys[i])
        {
            return i;
        }
    }

    return 0x00;
}

void Input::updateKey(const uint8 key, const bool pressed)
{
    m_pressedKeys[key] = pressed;
}

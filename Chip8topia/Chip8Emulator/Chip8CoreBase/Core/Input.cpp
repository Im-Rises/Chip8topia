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
    m_waitKeyStep = 0;
    m_keysWaitState.reset();
}

void Input::updateKey(const uint8 key, const bool pressed)
{
    m_pressedKeys[key] = pressed;
}

auto Input::isKeyPressed(const uint8 Vx) const -> bool
{
    return m_pressedKeys[Vx];
}

auto Input::getKeyWaitReleased() -> int
{
    switch (m_waitKeyStep)
    {
    case 0:
    {
        for (int i = 0; i < KEY_COUNT; i++)
        {
            if (m_pressedKeys[i])
            {
                m_keysWaitPreviousState[i] = true;
                m_waitKeyStep = 1;
            }
        }
        return -1;
    }
    case 1:
    {
        for (int i = 0; i < KEY_COUNT; i++)
        {
            if (m_keysWaitPreviousState[i] && !m_pressedKeys[i])
            {
                m_keysWaitPreviousState.reset();
                m_waitKeyStep = 0;
                return i;
            }
        }
        return -1;
    }
    default:
    {
        return -1;
    }
    }
}

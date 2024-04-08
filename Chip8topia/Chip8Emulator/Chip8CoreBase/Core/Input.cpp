#include "Input.h"

void Input::reset()
{
    m_pressedKeys.reset();
    m_waitKeyStep = 0;
    m_keysWaitPreviousState.reset();
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
    /* This version check if there is a difference and then iterate to find the key */
    int key = -1;
    std::bitset<KEY_COUNT> pressedKeys = m_pressedKeys;

    auto keysReleased = pressedKeys.flip() & m_keysWaitPreviousState;
    if (keysReleased.any())
    {
        for (int i = 0; i < KEY_COUNT; i++)
        {
            if (keysReleased[i])
            {
                key = i;
            }
        }
    }

    m_keysWaitPreviousState = m_pressedKeys;
    return key;

    /* First version check all key states to find a key which was pressed and now released, it does it every time which is not optimal */
    //    switch (m_waitKeyStep)
    //    {
    //    case 0:
    //    {
    //        for (int i = 0; i < KEY_COUNT; i++)
    //        {
    //            if (m_pressedKeys[i])
    //            {
    //                m_keysWaitPreviousState[i] = true;
    //                m_waitKeyStep = 1;
    //            }
    //        }
    //        return -1;
    //    }
    //    case 1:
    //    {
    //        for (int i = 0; i < KEY_COUNT; i++)
    //        {
    //            if (m_keysWaitPreviousState[i] && !m_pressedKeys[i])
    //            {
    //                m_keysWaitPreviousState.reset();
    //                m_waitKeyStep = 0;
    //                return i;
    //            }
    //        }
    //        return -1;
    //    }
    //    default:
    //    {
    //        return -1;
    //    }
    //    }
}

#include "Input.h"

auto Input::isKeyPressed(const uint8 Vx) const -> bool {
    return m_keys[Vx];
}

void Input::updateKey(const uint8 key, const bool pressed) {
    m_keys[key] = pressed;
}

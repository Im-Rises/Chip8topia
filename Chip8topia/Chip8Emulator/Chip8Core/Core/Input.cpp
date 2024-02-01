#include "Input.h"

void Input::reset() {
    m_keys.fill(false);
}

auto Input::isKeyPressed(const uint8 Vx) const -> bool {
    return m_keys[Vx];
}

void Input::updateKey(const uint8 key, const bool pressed) {
    m_keys[key] = pressed;
}

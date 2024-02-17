#include "Input.h"

void Input::reset() {
    m_pressedKeys.reset();
}

auto Input::isAnyKeyPressed() const -> bool {
    return m_pressedKeys.any();
}

auto Input::isKeyPressed(const uint8 Vx) const -> bool {
    return m_pressedKeys[Vx];
}

void Input::updateKey(const uint8 key, const bool pressed) {
    m_pressedKeys[key] = pressed;
}

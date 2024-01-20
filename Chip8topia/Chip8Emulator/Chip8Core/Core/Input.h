#pragma once

#include "binaryLib/binaryLib.h"

class Input {
public:
    Input() = default;
    Input(const Input&) = delete;
    Input(Input&&) = delete;
    auto operator=(const Input&) -> Input& = delete;
    auto operator=(Input&&) -> Input& = delete;
    ~Input() = default;

public:
    [[nodiscard]] auto isKeyPressed(const uint8 Vx) const -> bool;
    [[nodiscard]] auto waitForKeyPress(const uint8 x) -> bool;
};

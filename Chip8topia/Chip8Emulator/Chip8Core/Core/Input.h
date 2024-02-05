#pragma once

#include <array>
#include <binaryLib/binaryLib.h>

class Input {
public:
    static constexpr auto KEY_COUNT = 16;

public:
    Input() = default;
    Input(const Input&) = delete;
    Input(Input&&) = delete;
    auto operator=(const Input&) -> Input& = delete;
    auto operator=(Input&&) -> Input& = delete;
    ~Input() = default;

public:
    void reset();
    [[nodiscard]] auto isKeyPressed(const uint8 Vx) const -> bool;
    void updateKey(const uint8 key, const bool pressed);

private:
    // TODO: Maybe use std::bitset instead?
    std::array<bool, KEY_COUNT> m_keys{};
};

#pragma once

#include <array>

#include <binaryLib/binaryLib.h>

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
    void updateKey(const uint8 key, const bool pressed);

private:
    std::array<uint8, 16> m_keys{};
};

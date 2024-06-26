#pragma once

#include <array>
#include <bitset>
#include <functional>
#include <binaryLib/binaryLib.h>

class Input
{
public:
    static constexpr int KEY_COUNT = 16;
    static constexpr std::array<const uint8, KEY_COUNT> KEYS = {
        0x1, 0x2, 0x3, 0xC,
        0x4, 0x5, 0x6, 0xD,
        0x7, 0x8, 0x9, 0xE,
        0xA, 0x0, 0xB, 0xF
    };

    static constexpr std::array<const char* const, KEY_COUNT> KEY_NAMES = {
        "1", "2", "3", "C",
        "4", "5", "6", "D",
        "7", "8", "9", "E",
        "A", "0", "B", "F"
    };

public:
    Input() = default;
    Input(const Input&) = delete;
    Input(Input&&) = delete;
    auto operator=(const Input&) -> Input& = delete;
    auto operator=(Input&&) -> Input& = delete;
    ~Input() = default;

public:
    void reset();
    void updateKey(const uint8 key, const bool pressed);

    [[nodiscard]] auto isKeyPressed(const uint8 Vx) const -> bool;
    [[nodiscard]] auto getKeyWaitReleased() -> int;

private:
    std::bitset<KEY_COUNT> m_pressedKeys{};

    std::bitset<KEY_COUNT> m_keysWaitPreviousState{};
    //    uint8 m_waitKeyStep = 0;
};

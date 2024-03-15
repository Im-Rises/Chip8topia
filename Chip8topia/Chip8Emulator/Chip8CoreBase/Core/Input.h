#pragma once

#include <array>
#include <bitset>
#include <functional>
#include <binaryLib/binaryLib.h>

class Input
{
public:
    static constexpr auto KEY_COUNT = 16;
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

#if defined(BUILD_PARAM_SAFE)
    void setErrorCallback(const std::function<void(const std::string&)>& errorCallback);
#endif

public:
    void reset();
    [[nodiscard]] auto isAnyKeyPressed() const -> bool;
    [[nodiscard]] auto isKeyPressed(const uint8 Vx) const -> bool;
    void updateKey(const uint8 key, const bool pressed);

private:
    std::bitset<KEY_COUNT> m_pressedKeys{};
#if defined(BUILD_PARAM_SAFE)
    std::function<void(const std::string&)> m_errorCallback;
#endif
};

#pragma once

#include <array>
#include <cassert>
#include <iostream>

#include "CpuBase.h"

class PpuBase {
public:
    enum class PpuMode {
        LORES,
        HIRES
    };

    static constexpr int SCREEN_LORES_MODE_WIDTH = 64;
    static constexpr int SCREEN_LORES_MODE_HEIGHT = 32;
    static constexpr int SCREEN_HIRES_MODE_WIDTH = 128;
    static constexpr int SCREEN_HIRES_MODE_HEIGHT = 64;

public:
    //    PpuBase() = default;
    PpuBase() : m_videoMemory(SCREEN_LORES_MODE_WIDTH * SCREEN_LORES_MODE_HEIGHT) {}
    PpuBase(const PpuBase&) = delete;
    PpuBase(PpuBase&&) = delete;
    auto operator=(const PpuBase&) -> PpuBase& = delete;
    auto operator=(PpuBase&&) -> PpuBase& = delete;
    virtual ~PpuBase() = default;

public:
    virtual void clearScreen() = 0;
    virtual auto drawSprite(uint8 x, uint8 y, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> bool = 0;

    [[nodiscard]] auto getVideoMemory() const -> const std::vector<uint8>& { return m_videoMemory; }

    virtual void setMode(PpuMode mode) { assert(true); }
    [[nodiscard]] auto getMode() const -> PpuMode { return m_mode; }

protected:
    std::vector<uint8> m_videoMemory;
    PpuMode m_mode = PpuMode::LORES;
};

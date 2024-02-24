#pragma once

#include <array>

#include "../../Chip8CoreBase/Core/PpuBase.h"
#include <binaryLib/binaryLib.h>

class Ppu final : public PpuBase {
private:
    static constexpr size_t WIDTH = 64;
    static constexpr size_t HEIGHT = 32;
    static constexpr uint8 SPRITE_WIDTH = 8;
    //    static constexpr uint8 SPRITE_HEIGHT = 15;

public:
    Ppu() = default;
    Ppu(const Ppu&) = delete;
    Ppu(Ppu&&) = delete;
    auto operator=(const Ppu&) -> Ppu& = delete;
    auto operator=(Ppu&&) -> Ppu& = delete;
    ~Ppu() = default;

public:
    void clearScreen() final;
    auto drawSprite(uint8 x, uint8 y, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> bool final;
    //    [[nodiscard]] auto getVideoMemory() const -> const std::array<uint8, 2048>&;

private:
    static constexpr uint8 PIXEL_ON = 1;
    static constexpr uint8 PIXEL_OFF = 0;

    std::array<uint8, WIDTH * HEIGHT> m_videoMemory{};
};

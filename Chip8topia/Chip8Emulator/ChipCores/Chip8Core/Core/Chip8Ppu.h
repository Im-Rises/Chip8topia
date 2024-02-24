#pragma once

#include <array>

#include "../../../Chip8CoreBase/Core/PpuBase.h"
#include <binaryLib/binaryLib.h>

class Chip8Ppu final : public PpuBase {
private:
    static constexpr size_t WIDTH = 64;
    static constexpr size_t HEIGHT = 32;
    static constexpr uint8 SPRITE_WIDTH = 8;
    //    static constexpr uint8 SPRITE_HEIGHT = 15;
    static constexpr uint8 PIXEL_ON = 1;
    static constexpr uint8 PIXEL_OFF = 0;

public:
    Chip8Ppu();
    Chip8Ppu(const Chip8Ppu&) = delete;
    Chip8Ppu(Chip8Ppu&&) = delete;
    auto operator=(const Chip8Ppu&) -> Chip8Ppu& = delete;
    auto operator=(Chip8Ppu&&) -> Chip8Ppu& = delete;
    ~Chip8Ppu() final = default;

public:
    void clearScreen() final;
    auto drawSprite(uint8 x, uint8 y, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> bool final;
    [[nodiscard]] auto getVideoMemory() const -> const std::vector<uint8>& final { return m_videoMemory; }

private:
    std::vector<uint8> m_videoMemory;
};

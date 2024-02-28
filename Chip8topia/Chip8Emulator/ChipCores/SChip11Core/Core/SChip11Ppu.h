#pragma once

#include "../../../Chip8CoreBase/Core/PpuBase.h"
class SChip11Ppu final : public PpuBase {
private:
    // TODO: Add sprite width and height
    static constexpr uint8 SPRITE_DXYN_WIDTH = 8;
    static constexpr uint8 SPRITE_DXY0_WIDTH = 16;
    static constexpr uint8 SPRITE_DXY0_HEIGHT = 16;

public:
    SChip11Ppu();
    SChip11Ppu(const SChip11Ppu&) = delete;
    SChip11Ppu(SChip11Ppu&&) = delete;
    auto operator=(const SChip11Ppu&) -> SChip11Ppu& = delete;
    auto operator=(SChip11Ppu&&) -> SChip11Ppu& = delete;
    ~SChip11Ppu() final = default;

public:
    void clearScreen() final;
    auto drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8 final;

    auto draw8xNSprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint8 n, uint8* videoMemory) -> bool;
    auto draw16x16Sprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory) -> uint8;

    void scrollDown(uint8 n);
    void scrollRight(uint8 n);
    void scrollLeft(uint8 n);
};

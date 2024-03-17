#pragma once

#include "../../../Chip8CoreBase/Core/PpuBase.h"

class SChip11Ppu final : public PpuBase
{
public:
    SChip11Ppu() = default;
    SChip11Ppu(const SChip11Ppu&) = delete;
    SChip11Ppu(SChip11Ppu&&) = delete;
    auto operator=(const SChip11Ppu&) -> SChip11Ppu& = delete;
    auto operator=(SChip11Ppu&&) -> SChip11Ppu& = delete;
    ~SChip11Ppu() final = default;

public:
    void clearScreen() final;

    void scrollDown(uint8 n, bool isModernMode);
    void scrollRight(uint8 n, bool isModernMode);
    void scrollLeft(uint8 n, bool isModernMode);

    auto drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8 final;

private:
    auto draw8xNSprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint8 n, uint8* videoMemory) -> uint8;
    auto draw16x16Sprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory) -> uint8;
};

#pragma once

#include "../../../Chip8CoreBase/Core/PpuBase.h"

class SChipCPpu final : public PpuBase {
public:
    SChipCPpu();
    SChipCPpu(const SChipCPpu&) = delete;
    SChipCPpu(SChipCPpu&&) = delete;
    auto operator=(const SChipCPpu&) -> SChipCPpu& = delete;
    auto operator=(SChipCPpu&&) -> SChipCPpu& = delete;
    ~SChipCPpu() final = default;

public:
    void clearScreen() final;
    auto drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8 final;

    auto draw8xNSprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint8 n, uint8* videoMemory) -> bool;
    auto draw16x16Sprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory) -> uint8;

    void scrollDown(uint8 n);
    void scrollRight(uint8 n);
    void scrollLeft(uint8 n);
};

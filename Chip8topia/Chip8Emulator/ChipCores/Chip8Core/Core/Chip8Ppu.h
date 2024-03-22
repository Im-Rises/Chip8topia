#pragma once

#include <array>

#include "../../../Chip8CoreBase/Core/PpuBase.h"
#include <binaryLib/binaryLib.h>

class Chip8Ppu final : public PpuBase
{
private:
    static constexpr uint8 PLANE_INDEX = 0;

public:
    Chip8Ppu() = default;
    Chip8Ppu(const Chip8Ppu&) = delete;
    Chip8Ppu(Chip8Ppu&&) = delete;
    auto operator=(const Chip8Ppu&) -> Chip8Ppu& = delete;
    auto operator=(Chip8Ppu&&) -> Chip8Ppu& = delete;
    ~Chip8Ppu() final = default;

public:
    void clearScreen() final;
    auto drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8 final;
};

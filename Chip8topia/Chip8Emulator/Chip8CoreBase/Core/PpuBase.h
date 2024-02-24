#pragma once

#include <array>
#include "CpuBase.h"

class PpuBase {
public:
    PpuBase() = default;
    PpuBase(const PpuBase&) = delete;
    PpuBase(PpuBase&&) = delete;
    auto operator=(const PpuBase&) -> PpuBase& = delete;
    auto operator=(PpuBase&&) -> PpuBase& = delete;
    virtual ~PpuBase() = default;

public:
    virtual void clearScreen() = 0;
    virtual auto drawSprite(uint8 x, uint8 y, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> bool = 0;
    //    [[nodiscard]] virtual auto getVideoMemory() const -> const std::array<uint8, 2048>& = 0;
};

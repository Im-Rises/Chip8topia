#pragma once

#include "../../../Chip8CoreBase/Core/PpuBase.h"
class Schip11Ppu final : public PpuBase {
private:
    // TODO: Add sprite width and height
    static constexpr uint8 SPRITE_DXYN_WIDTH = 8;
    static constexpr uint8 SPRITE_DXY0_WIDTH = 16;
    static constexpr uint8 SPRITE_DXY0_HEIGHT = 16;

public:
    Schip11Ppu() = default;
    Schip11Ppu(const Schip11Ppu&) = delete;
    Schip11Ppu(Schip11Ppu&&) = delete;
    auto operator=(const Schip11Ppu&) -> Schip11Ppu& = delete;
    auto operator=(Schip11Ppu&&) -> Schip11Ppu& = delete;
    ~Schip11Ppu() final = default;

public:
    [[nodiscard]] auto getVideoMemory() const -> const std::vector<uint8>& final { return getMode() == PpuMode::LORES ? m_loresVideoMemory : m_hiresVideoMemory; }

    void clearScreen() final;
    auto drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8 final;

    auto drawLoresSprite(uint8 Vx, uint8 Vy, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg, uint8 width, uint8 height) -> bool;
    auto drawHiresSprite(uint8 Vx, uint8 Vy, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg, uint8 width, uint8 height) -> uint8;

    void scrollDown(uint8 n);
    void scrollRight(uint8 n);
    void scrollLeft(uint8 n);

private:
    std::vector<uint8> m_loresVideoMemory;
    std::vector<uint8> m_hiresVideoMemory;
};

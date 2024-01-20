#pragma once

#include <array>

#include "binaryLib/binaryLib.h"

class Ppu {
public:
    Ppu() = default;
    Ppu(const Ppu&) = delete;
    Ppu(Ppu&&) = delete;
    auto operator=(const Ppu&) -> Ppu& = delete;
    auto operator=(Ppu&&) -> Ppu& = delete;
    ~Ppu() = default;

public:
    void clearScreen();
    void drawSprite(uint8 x, uint8 y, uint8 n);

private:
    std::array<uint8, 2048> m_videoMemory{};
};

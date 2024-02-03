#pragma once

#include <array>

#include <binaryLib/binaryLib.h>
#include "Cpu.h"

class Ppu {
public:
    static constexpr size_t WIDTH = 64;
    static constexpr size_t HEIGHT = 32;
    static constexpr size_t CLOCK_FREQUENCY = 60;
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
    void clearScreen();
    void drawSprite(uint8 x, uint8 y, uint8 n, const std::array<uint8, Cpu::MEMORY_SIZE>& memory, uint16 I_reg); // TODO: Remove the use of std::vector and create a shared memory object
    [[nodiscard]] auto getVideoMemory() const -> const std::array<uint8, 2048>&;

private:
    std::array<uint8, WIDTH * HEIGHT> m_videoMemory{};
};

#include "Ppu.h"

void Ppu::clearScreen() {
    m_videoMemory.fill(0);
}

void Ppu::drawSprite(uint8 x, uint8 y, uint8 n, const std::array<uint8, Cpu::MEMORY_SIZE>& memory, uint16 I_reg) {
//    for (auto i = 0; i < n; ++i)
//    {
//        const auto spriteByte = memory[I_reg + i];
//        for (auto j = 0; j < SPRITE_WIDTH; ++j)
//        {
//            const auto bit = (spriteByte >> (SPRITE_WIDTH - 1 - j)) & 1;
//            const auto index = (x + j) % WIDTH + ((y + i) % HEIGHT) * WIDTH;
//            m_videoMemory[index] ^= bit;
//        }
//    }

    m_videoMemory[0] = 255;
}

auto Ppu::getVideoMemory() const -> const std::array<uint8, 2048>& {
    return m_videoMemory;
}

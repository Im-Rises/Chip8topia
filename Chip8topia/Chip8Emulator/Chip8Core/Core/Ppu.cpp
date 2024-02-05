#include "Ppu.h"

void Ppu::clearScreen() {
    m_videoMemory.fill(0);
}

auto Ppu::drawSprite(uint8 x, uint8 y, uint8 n, const std::array<uint8, Cpu::MEMORY_SIZE>& memory, uint16 I_reg) -> bool {
    bool collision = false;

    for (auto i = 0; i < n; ++i)
    {
        const auto spriteByte = memory[I_reg + i];
        for (auto j = 0; j < SPRITE_WIDTH; j++)
        {
            if (((spriteByte) & (0x1 << (7 - j))) != 0)
            {
                const auto index = (x + j) % WIDTH + ((y + i) % HEIGHT) * WIDTH;

                if (m_videoMemory[index] == 255)
                {
                    m_videoMemory[index] = 0;
                    collision = true;
                }
                else
                {
                    m_videoMemory[index] = 255;
                }
            }
        }
    }

    return collision;
}

auto Ppu::getVideoMemory() const -> const std::array<uint8, 2048>& {
    return m_videoMemory;
}

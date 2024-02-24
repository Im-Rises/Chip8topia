#include "Chip8Ppu.h"

Chip8Ppu::Chip8Ppu() : m_videoMemory(PpuBase::SCREEN_LORES_MODE_WIDTH * PpuBase::SCREEN_LORES_MODE_HEIGHT, PpuBase::PIXEL_OFF) {
}

void Chip8Ppu::clearScreen() {
    std::fill(m_videoMemory.begin(), m_videoMemory.end(), 0);
}

auto Chip8Ppu::drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8 {
    bool collision = false;

    // Wrap around the screen if out of bounds
    Vx %= PpuBase::SCREEN_LORES_MODE_WIDTH;
    Vy %= PpuBase::SCREEN_LORES_MODE_HEIGHT;

    for (auto i = 0; i < n; ++i)
    {
        const auto spriteByte = memory[I_reg + i];
        for (auto j = 0; j < SPRITE_WIDTH; j++)
        {
            if (((spriteByte) & (0x1 << (7 - j))) != 0)
            {
                // Clip the sprite if it goes out of bounds
                if (((Vx + j) >= PpuBase::SCREEN_LORES_MODE_WIDTH && j > 0) || ((Vy + i) >= PpuBase::SCREEN_LORES_MODE_HEIGHT && i > 0))
                {
                    continue;
                }

                // Draw the pixel
                const auto index = (Vx + j) % PpuBase::SCREEN_LORES_MODE_WIDTH + ((Vy + i) % PpuBase::SCREEN_LORES_MODE_HEIGHT) * PpuBase::SCREEN_LORES_MODE_WIDTH;
                if (m_videoMemory[index] == PIXEL_ON)
                {
                    m_videoMemory[index] = PIXEL_OFF;
                    collision = true;
                }
                else
                {
                    m_videoMemory[index] = PIXEL_ON;
                }
            }
        }
    }

    return static_cast<uint8>(collision);
}

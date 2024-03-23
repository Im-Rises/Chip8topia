#include "Chip8Ppu.h"

void Chip8Ppu::clearScreen()
{
    m_loresVideoMemoryPlanes.at(PLANE_INDEX).fill(PIXEL_OFF);
}

auto Chip8Ppu::drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8
{
    auto& videoMemory = m_loresVideoMemoryPlanes.at(PLANE_INDEX);

    bool collision = false;

    // Wrap around the screen if out of bounds
    Vx %= PpuBase::SCREEN_LORES_MODE_WIDTH;
    Vy %= PpuBase::SCREEN_LORES_MODE_HEIGHT;

    for (unsigned int i = 0; i < n; ++i)
    {
        const unsigned int spriteByte = memory[I_reg + i];
        for (unsigned int j = 0; j < 8; j++)
        {
            if (((spriteByte) & (0x1 << (7 - j))) != 0)
            {
                // Clip the sprite if it goes out of bounds
                if (((Vx + j) >= PpuBase::SCREEN_LORES_MODE_WIDTH && j > 0) || ((Vy + i) >= PpuBase::SCREEN_LORES_MODE_HEIGHT && i > 0))
                {
                    continue;
                }

                // Draw the pixel
                const unsigned int index = (Vx + j) % PpuBase::SCREEN_LORES_MODE_WIDTH + ((Vy + i) % PpuBase::SCREEN_LORES_MODE_HEIGHT) * PpuBase::SCREEN_LORES_MODE_WIDTH;
                if (videoMemory[index] == PIXEL_ON)
                {
                    videoMemory[index] = PIXEL_OFF;
                    collision = true;
                }
                else
                {
                    videoMemory[index] = PIXEL_ON;
                }
            }
        }
    }

    return static_cast<uint8>(collision);
}

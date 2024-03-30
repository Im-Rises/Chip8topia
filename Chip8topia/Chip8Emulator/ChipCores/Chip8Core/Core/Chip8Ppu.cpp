#include "Chip8Ppu.h"

void Chip8Ppu::clearScreen()
{
    m_loresVideoMemoryPlanes.at(PLANE_INDEX).fill(PIXEL_OFF);
}

auto Chip8Ppu::drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8
{
    auto& videoMemory = m_loresVideoMemoryPlanes.at(PLANE_INDEX);

    bool collision = false;

    Vx = Vx % PpuBase::SCREEN_LORES_MODE_WIDTH;
    Vy = Vy % PpuBase::SCREEN_LORES_MODE_HEIGHT;

    for (unsigned int i = 0; i < n; ++i)
    {
        const unsigned int spriteByte = memory[I_reg + i];
        for (unsigned int j = 0; j < 8; j++)
        {
            if (((spriteByte) & (0x1 << (7 - j))) != 0)
            {
                unsigned int x = (Vx + j);
                unsigned int y = (Vy + i);

                if (x >= PpuBase::SCREEN_LORES_MODE_WIDTH || y >= PpuBase::SCREEN_LORES_MODE_HEIGHT)
                {
                    continue;
                }

                const unsigned int index = x + y * PpuBase::SCREEN_LORES_MODE_WIDTH;
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

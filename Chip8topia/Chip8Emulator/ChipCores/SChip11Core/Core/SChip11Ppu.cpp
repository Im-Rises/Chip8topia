#include "SChip11Ppu.h"

SChip11Ppu::SChip11Ppu() {
}

void SChip11Ppu::clearScreen() {
    if (getMode() == PpuMode::LORES)
    {
        std::fill(m_loresVideoMemory.begin(), m_loresVideoMemory.end(), 0);
    }
    else
    {
        std::fill(m_hiresVideoMemory.begin(), m_hiresVideoMemory.end(), 0);
    }
}

auto SChip11Ppu::drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8 {
    //  The original SCHIP-1.1 in hires mode set VF to the number of sprite rows with collisions plus the number of rows clipped at the bottom border

    if (getMode() == PpuMode::LORES)
    {
        return static_cast<uint8>(drawLoresSprite(Vx, Vy, memory, I_reg, SPRITE_DXYN_WIDTH, n));
    }
    else
    {
        const uint8 spriteWidth = n == 0 ? SPRITE_DXY0_WIDTH : SPRITE_DXYN_WIDTH;
        const uint8 spriteHeight = n == 0 ? SPRITE_DXY0_HEIGHT : n;
        return drawHiresSprite(Vx, Vy, memory, I_reg, spriteWidth, spriteHeight);
    }
}

auto SChip11Ppu::drawLoresSprite(uint8 Vx, uint8 Vy, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg, uint8 width, uint8 height) -> bool {
    bool collision = false;

    // Wrap around the screen if out of bounds
    Vx %= PpuBase::SCREEN_LORES_MODE_WIDTH;
    Vy %= PpuBase::SCREEN_LORES_MODE_HEIGHT;

    for (auto i = 0; i < height; ++i)
    {
        const auto spriteByte = memory[I_reg + i];
        for (auto j = 0; j < width; j++)
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
                if (m_loresVideoMemory[index] == PIXEL_ON)
                {
                    m_loresVideoMemory[index] = PIXEL_OFF;
                    collision = true;
                }
                else
                {
                    m_loresVideoMemory[index] = PIXEL_ON;
                }
            }
        }
    }

    return collision;
}

auto SChip11Ppu::drawHiresSprite(uint8 Vx, uint8 Vy, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg, uint8 width, uint8 height) -> uint8 {
    for (auto i = 0; i < height; ++i)
    {
        const auto spriteByte = memory[I_reg + i];
        for (auto j = 0; j < width; j++)
        {
            // const auto memoryIndex = (Vx + j) % PpuBase::SCREEN_HIRES_MODE_WIDTH + ((Vy + i) % PpuBase::SCREEN_HIRES_MODE_HEIGHT) * PpuBase::SCREEN_HIRES_MODE_WIDTH;
            const auto memoryIndex = (Vx + j) + (Vy + i) * PpuBase::SCREEN_HIRES_MODE_WIDTH;
        }
    }

    return 0;
}

void SChip11Ppu::scrollDown(uint8 n) {
    const int width = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = getMode() == PpuMode::LORES ? m_loresVideoMemory.data() : m_hiresVideoMemory.data();
    n = getMode() == PpuMode::LORES ? n / 2 : n;

    for (int row = height - n - 1; row >= 0; row--)
    {
        for (int col = 0; col < width; col++)
        {
            videoMemory[(row + n) * width + col] = videoMemory[row * width + col];
            videoMemory[row * width + col] = 0;
        }
    }
}

void SChip11Ppu::scrollRight(uint8 n) {
    const int width = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = getMode() == PpuMode::LORES ? m_loresVideoMemory.data() : m_hiresVideoMemory.data();
    n = getMode() == PpuMode::LORES ? 2 : 4;

    for (int col = width - n - 1; col >= 0; col--)
    {
        for (int row = 0; row < height; row++)
        {
            videoMemory[row * width + col + n] = videoMemory[row * width + col];
            videoMemory[row * width + col] = 0;
        }
    }
}

void SChip11Ppu::scrollLeft(uint8 n) {
    const int width = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = getMode() == PpuMode::LORES ? m_loresVideoMemory.data() : m_hiresVideoMemory.data();
    n = getMode() == PpuMode::LORES ? 2 : 4;

    for (int col = 0; col < width - n; col++)
    {
        for (int row = 0; row < height; row++)
        {
            videoMemory[row * width + col] = videoMemory[row * width + col + n];
            videoMemory[row * width + col + n] = 0;
        }
    }
}

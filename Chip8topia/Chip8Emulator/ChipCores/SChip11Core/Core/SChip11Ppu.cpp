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
    const uint8 spriteWidth = n == 0 ? SPRITE_DXY0_WIDTH : SPRITE_DXYN_WIDTH;
    const uint8 spriteHeight = n == 0 ? SPRITE_DXY0_HEIGHT : n;

    if (getMode() == PpuMode::LORES)
    {
        return static_cast<uint8>(drawLoresSprite(Vx, Vy, memory, I_reg, spriteWidth, spriteHeight));
    }
    else
    {
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
    int collisionCount = 0;
    int rowsClippedCount = 0;

    // Wrap around the screen if out of bounds
    Vx %= PpuBase::SCREEN_LORES_MODE_WIDTH;
    Vy %= PpuBase::SCREEN_LORES_MODE_HEIGHT;

    for (auto i = 0; i < height; ++i)
    {
        bool collision = false;
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

        if (collision)
        {
            collisionCount++;
        }

        if (Vy + i >= PpuBase::SCREEN_LORES_MODE_HEIGHT)
        {
            rowsClippedCount++;
        }
    }

    return collisionCount + rowsClippedCount;
}

void SChip11Ppu::scrollDown(uint8 n) {
    if (getMode() == PpuMode::LORES)
    {
        std::rotate(m_loresVideoMemory.begin(), m_loresVideoMemory.begin() + n * PpuBase::SCREEN_LORES_MODE_WIDTH, m_loresVideoMemory.end());
    }
    else
    {
        std::rotate(m_hiresVideoMemory.begin(), m_hiresVideoMemory.begin() + n * PpuBase::SCREEN_HIRES_MODE_WIDTH, m_hiresVideoMemory.end());
    }
}

void SChip11Ppu::scrollRight(uint8 n) {
    if (getMode() == PpuMode::LORES)
    {
        for (auto i = 0; i < n; ++i)
        {
            for (auto j = 0; j < PpuBase::SCREEN_LORES_MODE_HEIGHT; ++j)
            {
                std::rotate(m_loresVideoMemory.begin() + j * PpuBase::SCREEN_LORES_MODE_WIDTH, m_loresVideoMemory.begin() + j * PpuBase::SCREEN_LORES_MODE_WIDTH + 1, m_loresVideoMemory.begin() + (j + 1) * PpuBase::SCREEN_LORES_MODE_WIDTH);
            }
        }
    }
    else
    {
        for (auto i = 0; i < n; ++i)
        {
            for (auto j = 0; j < PpuBase::SCREEN_HIRES_MODE_HEIGHT; ++j)
            {
                std::rotate(m_hiresVideoMemory.begin() + j * PpuBase::SCREEN_HIRES_MODE_WIDTH, m_hiresVideoMemory.begin() + j * PpuBase::SCREEN_HIRES_MODE_WIDTH + 1, m_hiresVideoMemory.begin() + (j + 1) * PpuBase::SCREEN_HIRES_MODE_WIDTH);
            }
        }
    }
}

void SChip11Ppu::scrollLeft(uint8 n) {
    if (getMode() == PpuMode::LORES)
    {
        for (auto i = 0; i < n; ++i)
        {
            for (auto j = 0; j < PpuBase::SCREEN_LORES_MODE_HEIGHT; ++j)
            {
                std::rotate(m_loresVideoMemory.begin() + j * PpuBase::SCREEN_LORES_MODE_WIDTH, m_loresVideoMemory.begin() + (j + 1) * PpuBase::SCREEN_LORES_MODE_WIDTH - 1, m_loresVideoMemory.begin() + (j + 1) * PpuBase::SCREEN_LORES_MODE_WIDTH);
            }
        }
    }
    else
    {
        for (auto i = 0; i < n; ++i)
        {
            for (auto j = 0; j < PpuBase::SCREEN_HIRES_MODE_HEIGHT; ++j)
            {
                std::rotate(m_hiresVideoMemory.begin() + j * PpuBase::SCREEN_HIRES_MODE_WIDTH, m_hiresVideoMemory.begin() + (j + 1) * PpuBase::SCREEN_HIRES_MODE_WIDTH - 1, m_hiresVideoMemory.begin() + (j + 1) * PpuBase::SCREEN_HIRES_MODE_WIDTH);
            }
        }
    }
}

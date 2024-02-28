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

    if ((getMode() == PpuMode::LORES) || (getMode() == PpuMode::HIRES && n != 0)) // Draw 8xN sprite
    {
        return static_cast<uint8>(draw8xNSprite(Vx, Vy, I_reg, memory, n, getMode() == PpuMode::LORES ? m_loresVideoMemory.data() : m_hiresVideoMemory.data()));
    }
    else // Draw 16x16 sprite
    {
        return draw16x16Sprite(Vx, Vy, I_reg, memory);
    }
}

auto SChip11Ppu::draw8xNSprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint8 n, uint8* videoMemory) -> bool {
    bool collision = false;

    const int screenWidth = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int screenHeight = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;

    Vx %= screenWidth;
    Vy %= screenHeight;

    for (auto i = 0; i < n; ++i)
    {
        const auto spriteByte = memory[I_reg + i];
        for (auto j = 0; j < 8; j++)
        {
            if (((spriteByte) & (0x1 << (7 - j))) != 0)
            {
                // Clip the sprite if it goes out of bounds
                if (((Vx + j) >= screenWidth && j > 0) || ((Vy + i) >= screenHeight && i > 0))
                {
                    continue;
                }

                // Draw the pixel
                const auto index = (Vx + j) % screenWidth + ((Vy + i) % screenHeight) * screenWidth;
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

    return collision;
}

auto SChip11Ppu::draw16x16Sprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory) -> uint8 {
    uint8 rowCollisionCount = 0;
    uint8 rowClippedCount = 0;

    for (int i = 0; i < 16; i++)
    {
        const auto spriteByte1 = memory[I_reg + i];
        const auto spriteByte2 = memory[I_reg + i + 16];

        for (int j = 0; j < 8; j++)
        {


            //            if (((spriteByte1) & (0x1 << (7 - j))) != 0 || ((spriteByte2) & (0x1 << (7 - j))) != 0)
            //            {
            //                // Clip the sprite if it goes out of bounds
            //                if (((Vx + j) >= PpuBase::SCREEN_HIRES_MODE_WIDTH && j > 0) || ((Vy + i) >= PpuBase::SCREEN_HIRES_MODE_HEIGHT && i > 0))
            //                {
            //                    rowClippedCount++;
            //                    continue;
            //                }
            //
            //                // Draw the pixel
            //                const auto index = (Vx + j) % PpuBase::SCREEN_HIRES_MODE_WIDTH + ((Vy + i) % PpuBase::SCREEN_HIRES_MODE_HEIGHT) * PpuBase::SCREEN_HIRES_MODE_WIDTH;
            //                if (m_hiresVideoMemory[index] == PIXEL_ON)
            //                {
            //                    m_hiresVideoMemory[index] = PIXEL_OFF;
            //                    rowCollisionCount++;
            //                }
            //                else
            //                {
            //                    m_hiresVideoMemory[index] = PIXEL_ON;
            //                }
            //            }
        }
    }

    return static_cast<uint8>(rowCollisionCount + rowClippedCount);
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

#include "SChip11Ppu.h"

void SChip11Ppu::clearScreen()
{
    if (getMode() == PpuMode::LORES)
    {
        m_loresVideoMemoryPlanes[m_plane].fill(PIXEL_OFF);
    }
    else
    {
        m_hiresVideoMemoryPlanes[m_plane].fill(PIXEL_OFF);
    }
}

void SChip11Ppu::scrollDown(uint8 n)
{
    const int width = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = getMode() == PpuMode::LORES ? m_loresVideoMemoryPlanes[m_plane].data() : m_hiresVideoMemoryPlanes[m_plane].data();

    for (int row = height - n - 1; row >= 0; row--)
    {
        for (int col = 0; col < width; col++)
        {
            videoMemory[(row + n) * width + col] = videoMemory[row * width + col];
            videoMemory[row * width + col] = 0;
        }
    }
}

void SChip11Ppu::scrollRight(uint8 n)
{
    const int width = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = getMode() == PpuMode::LORES ? m_loresVideoMemoryPlanes[m_plane].data() : m_hiresVideoMemoryPlanes[m_plane].data();

    for (int col = width - n - 1; col >= 0; col--)
    {
        for (int row = 0; row < height; row++)
        {
            videoMemory[row * width + col + n] = videoMemory[row * width + col];
            videoMemory[row * width + col] = 0;
        }
    }
}

void SChip11Ppu::scrollLeft(uint8 n)
{
    const int width = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = getMode() == PpuMode::LORES ? m_loresVideoMemoryPlanes[m_plane].data() : m_hiresVideoMemoryPlanes[m_plane].data();

    for (int col = 0; col < width - n; col++)
    {
        for (int row = 0; row < height; row++)
        {
            videoMemory[row * width + col] = videoMemory[row * width + col + n];
            videoMemory[row * width + col + n] = 0;
        }
    }
}

auto SChip11Ppu::drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8
{
    if ((getMode() == PpuMode::LORES) || (getMode() == PpuMode::HIRES && n != 0)) // Draw 8xN sprite
    {
        return draw8xNSprite(Vx, Vy, I_reg, memory, n, getMode() == PpuMode::LORES ? m_loresVideoMemoryPlanes[m_plane].data() : m_hiresVideoMemoryPlanes[m_plane].data());
    }

    // Draw 16x16 sprite
    return draw16x16Sprite(Vx, Vy, I_reg, memory);
}

auto SChip11Ppu::draw8xNSprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint8 n, uint8* videoMemory) -> uint8
{
    uint8 rowCollisionCount = 0;
    uint8 rowClippedCount = 0;

    const unsigned int screenWidth = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const unsigned int screenHeight = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;

    Vx %= screenWidth;
    Vy %= screenHeight;

    for (unsigned int i = 0; i < n; ++i)
    {
        const unsigned int spriteByte = memory[I_reg + i];
        for (unsigned int j = 0; j < 8; j++)
        {
            if (((spriteByte) & (0x1 << (7 - j))) != PIXEL_OFF)
            {
                // Clip the sprite if it goes out of bounds
                if (((Vx + j) >= screenWidth && j > 0) || ((Vy + i) >= screenHeight && i > 0))
                {
                    rowClippedCount++;
                    continue;
                }

                // Draw the pixel
                const unsigned int index = (Vx + j) % screenWidth + ((Vy + i) % screenHeight) * screenWidth;
                if (videoMemory[index] == PIXEL_ON)
                {
                    videoMemory[index] = PIXEL_OFF;
                    rowCollisionCount++;
                }
                else
                {
                    videoMemory[index] = PIXEL_ON;
                }
            }
        }
    }

    if (getMode() == PpuMode::HIRES)
    {
        return rowCollisionCount + rowClippedCount;
    }

    // LORES
    return (rowClippedCount + rowCollisionCount) > 0 ? 1 : 0;
}

auto SChip11Ppu::draw16x16Sprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory) -> uint8
{
    auto& videoMemory = m_hiresVideoMemoryPlanes.at(m_plane);

    uint8 rowCollisionCount = 0;
    uint8 rowClippedCount = 0;

    for (unsigned int i = 0; i < 16; i++) // 16 rows
    {
        for (unsigned int byteIndex = 0; byteIndex < 2; byteIndex++) // Two bytes per row (16 pixels) each pixel is 1 bit
        {
            for (unsigned int j = 0; j < 8; j++) // 8 pixels per byte
            {
                if (((memory[I_reg + i * 2 + byteIndex] >> (7 - j)) & 0x1) != PIXEL_OFF)
                {
                    const unsigned int x = (Vx + j + byteIndex * 8) % PpuBase::SCREEN_HIRES_MODE_WIDTH;
                    const unsigned int y = (Vy + i) % PpuBase::SCREEN_HIRES_MODE_HEIGHT;

                    if (x >= PpuBase::SCREEN_HIRES_MODE_WIDTH || y >= PpuBase::SCREEN_HIRES_MODE_HEIGHT)
                    {
                        rowClippedCount++;
                        continue;
                    }

                    if (videoMemory[y * PpuBase::SCREEN_HIRES_MODE_WIDTH + x] == PIXEL_ON)
                    {
                        videoMemory[y * PpuBase::SCREEN_HIRES_MODE_WIDTH + x] = PIXEL_OFF;
                        rowCollisionCount++;
                    }
                    else
                    {
                        videoMemory[y * PpuBase::SCREEN_HIRES_MODE_WIDTH + x] = PIXEL_ON;
                    }
                }
            }
        }
    }

    return rowCollisionCount + rowClippedCount;
}

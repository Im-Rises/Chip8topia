#include "SChipCPpu.h"

void SChipCPpu::clearScreen()
{
    if (getMode() == PpuMode::LORES)
    {
        m_loresVideoMemoryPlanes[PLANE_INDEX].fill(PIXEL_OFF);
    }
    else
    {
        m_hiresVideoMemoryPlanes[PLANE_INDEX].fill(PIXEL_OFF);
    }
}

void SChipCPpu::scrollDown(uint8 n)
{
    const int width = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = getMode() == PpuMode::LORES ? m_loresVideoMemoryPlanes[PLANE_INDEX].data() : m_hiresVideoMemoryPlanes[0].data();

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

void SChipCPpu::scrollRight(uint8 n)
{
    const int width = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = getMode() == PpuMode::LORES ? m_loresVideoMemoryPlanes[PLANE_INDEX].data() : m_hiresVideoMemoryPlanes[0].data();

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

void SChipCPpu::scrollLeft(uint8 n)
{
    const int width = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = getMode() == PpuMode::LORES ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = getMode() == PpuMode::LORES ? m_loresVideoMemoryPlanes[PLANE_INDEX].data() : m_hiresVideoMemoryPlanes[0].data();

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

auto SChipCPpu::drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8
{
    if ((getMode() == PpuMode::LORES) || (getMode() == PpuMode::HIRES && n != 0)) // Draw 8xN sprite
    {
        return draw8xNSprite(Vx, Vy, I_reg, memory, n, getMode() == PpuMode::LORES ? m_loresVideoMemoryPlanes[PLANE_INDEX].data() : m_hiresVideoMemoryPlanes[PLANE_INDEX].data());
    }
    else // Draw 16x16 sprite
    {
        return draw16x16Sprite(Vx, Vy, I_reg, memory);
    }
}

auto SChipCPpu::draw8xNSprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint8 n, uint8* videoMemory) -> bool
{
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
            if (((spriteByte) & (0x1 << (7 - j))) == PIXEL_ON)
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

auto SChipCPpu::draw16x16Sprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory) -> bool
{
    auto& videoMemory = m_hiresVideoMemoryPlanes.at(PLANE_INDEX);

    bool collision = false;

    for (int i = 0; i < 16; i++) // 16 rows
    {
        for (int byteIndex = 0; byteIndex < 2; byteIndex++) // Two bytes per row (16 pixels) each pixel is 1 bit
        {
            for (int j = 0; j < 8; j++) // 8 pixels per byte
            {
                if (((memory[I_reg + i * 2 + byteIndex] >> (7 - j)) & 0x1) == PIXEL_ON)
                {
                    int x = (Vx + j + byteIndex * 8) % PpuBase::SCREEN_HIRES_MODE_WIDTH;
                    int y = (Vy + i) % PpuBase::SCREEN_HIRES_MODE_HEIGHT;

                    if (x >= PpuBase::SCREEN_HIRES_MODE_WIDTH || y >= PpuBase::SCREEN_HIRES_MODE_HEIGHT)
                    {
                        continue;
                    }

                    if (videoMemory[y * PpuBase::SCREEN_HIRES_MODE_WIDTH + x] == PIXEL_ON)
                    {
                        videoMemory[y * PpuBase::SCREEN_HIRES_MODE_WIDTH + x] = PIXEL_OFF;
                        collision = true;
                    }
                    else
                    {
                        videoMemory[y * PpuBase::SCREEN_HIRES_MODE_WIDTH + x] = PIXEL_ON;
                    }
                }
            }
        }
    }

    return collision;
}

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
    const bool isLoresMode = getMode() == PpuMode::LORES;
    const int width = isLoresMode ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = isLoresMode ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = isLoresMode ? m_loresVideoMemoryPlanes[PLANE_INDEX].data() : m_hiresVideoMemoryPlanes[PLANE_INDEX].data();

    for (int row = height - n - 1; row >= 0; row--)
    {
        for (int col = 0; col < width; col++)
        {
            videoMemory[(row + n) * width + col] = videoMemory[row * width + col];
            videoMemory[row * width + col] = PIXEL_OFF;
        }
    }
}

void SChipCPpu::scrollRight(uint8 n)
{
    const bool isLoresMode = getMode() == PpuMode::LORES;
    const int width = isLoresMode ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = isLoresMode ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = isLoresMode ? m_loresVideoMemoryPlanes[PLANE_INDEX].data() : m_hiresVideoMemoryPlanes[PLANE_INDEX].data();

    for (int col = width - n - 1; col >= 0; col--)
    {
        for (int row = 0; row < height; row++)
        {
            videoMemory[row * width + col + n] = videoMemory[row * width + col];
            videoMemory[row * width + col] = PIXEL_OFF;
        }
    }
}

void SChipCPpu::scrollLeft(uint8 n)
{
    const bool isLoresMode = getMode() == PpuMode::LORES;
    const int width = isLoresMode ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = isLoresMode ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    uint8* videoMemory = isLoresMode ? m_loresVideoMemoryPlanes[PLANE_INDEX].data() : m_hiresVideoMemoryPlanes[PLANE_INDEX].data();

    for (int col = 0; col < width - n; col++)
    {
        for (int row = 0; row < height; row++)
        {
            videoMemory[row * width + col] = videoMemory[row * width + col + n];
            videoMemory[row * width + col + n] = PIXEL_OFF;
        }
    }
}

auto SChipCPpu::drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8
{
    const bool isLoresMode = getMode() == PpuMode::LORES;
    if (isLoresMode || (getMode() == PpuMode::HIRES && n != 0)) // Draw 8xN sprite
    {
        return static_cast<uint8>(draw8xNSprite(Vx, Vy, I_reg, memory, n, isLoresMode ? m_loresVideoMemoryPlanes[PLANE_INDEX].data() : m_hiresVideoMemoryPlanes[PLANE_INDEX].data()));
    }

    // Draw 16x16 sprite
    return static_cast<uint8>(draw16x16Sprite(Vx, Vy, I_reg, memory, m_hiresVideoMemoryPlanes[PLANE_INDEX].data()));
}

auto SChipCPpu::draw8xNSprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint8 n, uint8* videoMemory) -> bool
{
    bool collision = false;

    const bool isLoresMode = getMode() == PpuMode::LORES;
    const unsigned int screenWidth = isLoresMode ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const unsigned int screenHeight = isLoresMode ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;

    Vx = Vx % screenWidth;
    Vy = Vy % screenHeight;

    for (unsigned int i = 0; i < n; ++i)
    {
        const unsigned int spriteByte = memory[I_reg + i];
        for (unsigned int j = 0; j < 8; j++)
        {
            if (((spriteByte) & (0x1 << (7 - j))) != 0)
            {
                if (((Vx + j) >= screenWidth && j > 0) || ((Vy + i) >= screenHeight && i > 0))
                {
                    continue;
                }

                const unsigned int index = (Vx + j) % screenWidth + ((Vy + i) % screenHeight) * screenWidth;
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

auto SChipCPpu::draw16x16Sprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint8* videoMemory) -> bool
{
    bool collision = false;

    Vx = Vx % PpuBase::SCREEN_HIRES_MODE_WIDTH;
    Vy = Vy % PpuBase::SCREEN_HIRES_MODE_HEIGHT;

    for (unsigned int i = 0; i < 16; i++) // 16 rows
    {
        for (unsigned int byteIndex = 0; byteIndex < 2; byteIndex++) // Two bytes per row (16 pixels) each pixel is 1 bit
        {
            const unsigned int spriteByte = memory[I_reg + i * 2 + byteIndex];
            for (unsigned int j = 0; j < 8; j++) // 8 pixels per byte
            {
                if ((spriteByte & (0x1 << (7 - j))) != 0)
                {
                    unsigned int x = (Vx + j + byteIndex * 8) % PpuBase::SCREEN_HIRES_MODE_WIDTH;
                    unsigned int y = (Vy + i) % PpuBase::SCREEN_HIRES_MODE_HEIGHT;

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

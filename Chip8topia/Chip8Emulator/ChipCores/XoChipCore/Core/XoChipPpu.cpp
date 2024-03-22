#include "XoChipPpu.h"

#include <binaryLib/binaryLib.h>

void XoChipPpu::clearScreen()
{
    if (getMode() == PpuMode::LORES)
    {
        for (int i = 0; i < PLANE_COUNT; i++)
        {
            if (getBit(m_plane, i))
            {
                m_loresVideoMemoryPlanes[i].fill(0);
            }
        }
    }
    else
    {
        for (int i = 0; i < PLANE_COUNT; i++)
        {
            if (getBit(m_plane, i))
            {
                m_hiresVideoMemoryPlanes[i].fill(0);
            }
        }
    }
}

void scrollDownInternal(uint8 n, uint8* videoMemory, int width, int height)
{
    for (int row = height - n - 1; row >= 0; row--)
    {
        for (int col = 0; col < width; col++)
        {
            videoMemory[(row + n) * width + col] = videoMemory[row * width + col];
            videoMemory[row * width + col] = 0;
        }
    }
}

void XoChipPpu::scrollDown(uint8 n)
{
    bool loresMode = getMode() == PpuMode::LORES;
    const int width = loresMode ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = loresMode ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;

    for (int i = 0; i < PLANE_COUNT; i++)
    {
        if (getBit(m_plane, i))
        {
            scrollDownInternal(n, loresMode ? m_loresVideoMemoryPlanes[i].data() : m_hiresVideoMemoryPlanes[i].data(), width, height);
        }
    }
}

void scrollUpInternal(uint8 n, uint8* videoMemory, int width, int height)
{
    for (int row = 0; row < height - n; row++)
    {
        for (int col = 0; col < width; col++)
        {
            videoMemory[row * width + col] = videoMemory[(row + n) * width + col];
            videoMemory[(row + n) * width + col] = 0;
        }
    }
}

void XoChipPpu::scrollUp(uint8 n)
{
    bool loresMode = getMode() == PpuMode::LORES;
    const int width = loresMode ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = loresMode ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;

    for (int i = 0; i < PLANE_COUNT; i++)
    {
        if (getBit(m_plane, i))
        {
            scrollUpInternal(n, loresMode ? m_loresVideoMemoryPlanes[i].data() : m_hiresVideoMemoryPlanes[i].data(), width, height);
        }
    }
}

void scrollRightInternal(uint8 n, uint8* videoMemory, int width, int height)
{
    for (int col = width - n - 1; col >= 0; col--)
    {
        for (int row = 0; row < height; row++)
        {
            videoMemory[row * width + col + n] = videoMemory[row * width + col];
            videoMemory[row * width + col] = 0;
        }
    }
}

void XoChipPpu::scrollRight(uint8 n)
{
    bool loresMode = getMode() == PpuMode::LORES;
    const int width = loresMode ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = loresMode ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    n = 4;

    for (int i = 0; i < PLANE_COUNT; i++)
    {
        if (getBit(m_plane, i))
        {
            scrollRightInternal(n, loresMode ? m_loresVideoMemoryPlanes[i].data() : m_hiresVideoMemoryPlanes[i].data(), width, height);
        }
    }
}

void scrollLeftInternal(uint8 n, uint8* videoMemory, int width, int height)
{
    for (int col = 0; col < width - n; col++)
    {
        for (int row = 0; row < height; row++)
        {
            videoMemory[row * width + col] = videoMemory[row * width + col + n];
            videoMemory[row * width + col + n] = 0;
        }
    }
}

void XoChipPpu::scrollLeft(uint8 n)
{
    bool loresMode = getMode() == PpuMode::LORES;
    const int width = loresMode ? PpuBase::SCREEN_LORES_MODE_WIDTH : PpuBase::SCREEN_HIRES_MODE_WIDTH;
    const int height = loresMode ? PpuBase::SCREEN_LORES_MODE_HEIGHT : PpuBase::SCREEN_HIRES_MODE_HEIGHT;
    n = 4;

    for (int i = 0; i < PLANE_COUNT; i++)
    {
        if (getBit(m_plane, i))
        {
            scrollLeftInternal(n, loresMode ? m_loresVideoMemoryPlanes[i].data() : m_hiresVideoMemoryPlanes[i].data(), width, height);
        }
    }
}

auto XoChipPpu::drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8
{
    if ((getMode() == PpuMode::LORES) || (getMode() == PpuMode::HIRES && n != 0)) // Draw 8xN sprite
    {
        bool collision = false;
        int drawCount = 0;

        for (int i = 0; i < PLANE_COUNT; i++)
        {
            if (getBit(m_plane, i))
            {
                collision |= draw8xNSprite(Vx, Vy, I_reg + drawCount * n, memory, n, m_loresVideoMemoryPlanes[i].data());
                drawCount++;
            }
        }
    }
    else // Draw 16x16 sprite
    {
        bool collision = false;
        int drawCount = 0;

        for (int i = 0; i < PLANE_COUNT; i++)
        {
            if (getBit(m_plane, i))
            {
                collision |= draw16x16Sprite(Vx, Vy, I_reg + drawCount * 32, memory, m_hiresVideoMemoryPlanes[i].data());
                drawCount++;
            }
        }
    }

    return 0;
}

auto XoChipPpu::draw8xNSprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint8 n, uint8* videoMemory) -> bool
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

auto XoChipPpu::draw16x16Sprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint8* videoMemory) -> bool
{
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

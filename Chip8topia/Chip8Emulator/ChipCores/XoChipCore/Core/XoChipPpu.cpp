#include "XoChipPpu.h"

void XoChipPpu::clearScreen()
{
    if (getMode() == PpuMode::LORES)
    {
        if (m_plane == 1)
        {
            std::fill(m_loresVideoMemory.begin(), m_loresVideoMemory.end(), 0);
        }
        else if (m_plane == 2)
        {
            std::fill(m_loresVideoMemoryPlane.begin(), m_loresVideoMemoryPlane.end(), 0);
        }
        else if (m_plane == 3)
        {
            std::fill(m_loresVideoMemory.begin(), m_loresVideoMemory.end(), 0);
            std::fill(m_loresVideoMemoryPlane.begin(), m_loresVideoMemoryPlane.end(), 0);
        }
    }
    else
    {
        if (m_plane == 1)
        {
            std::fill(m_hiresVideoMemory.begin(), m_hiresVideoMemory.end(), 0);
        }
        else if (m_plane == 2)
        {
            std::fill(m_hiresVideoMemoryPlane.begin(), m_hiresVideoMemoryPlane.end(), 0);
        }
        else if (m_plane == 3)
        {
            std::fill(m_hiresVideoMemory.begin(), m_hiresVideoMemory.end(), 0);
            std::fill(m_hiresVideoMemoryPlane.begin(), m_hiresVideoMemoryPlane.end(), 0);
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

    if (m_plane == 1)
    {
        scrollDownInternal(n, loresMode ? m_loresVideoMemory.data() : m_hiresVideoMemory.data(), width, height);
    }
    else if (m_plane == 2)
    {
        scrollDownInternal(n, loresMode ? m_loresVideoMemoryPlane.data() : m_hiresVideoMemoryPlane.data(), width, height);
    }
    else if (m_plane == 3)
    {
        scrollDownInternal(n, loresMode ? m_loresVideoMemory.data() : m_hiresVideoMemory.data(), width, height);
        scrollDownInternal(n, loresMode ? m_loresVideoMemoryPlane.data() : m_hiresVideoMemoryPlane.data(), width, height);
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

    if (m_plane == 1)
    {
        scrollUpInternal(n, loresMode ? m_loresVideoMemory.data() : m_hiresVideoMemory.data(), width, height);
    }
    else if (m_plane == 2)
    {
        scrollUpInternal(n, loresMode ? m_loresVideoMemoryPlane.data() : m_hiresVideoMemoryPlane.data(), width, height);
    }
    else if (m_plane == 3)
    {
        scrollUpInternal(n, loresMode ? m_loresVideoMemory.data() : m_hiresVideoMemory.data(), width, height);
        scrollUpInternal(n, loresMode ? m_loresVideoMemoryPlane.data() : m_hiresVideoMemoryPlane.data(), width, height);
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

    if (m_plane == 1)
    {
        scrollRightInternal(n, loresMode ? m_loresVideoMemory.data() : m_hiresVideoMemory.data(), width, height);
    }
    else if (m_plane == 2)
    {
        scrollRightInternal(n, loresMode ? m_loresVideoMemoryPlane.data() : m_hiresVideoMemoryPlane.data(), width, height);
    }
    else if (m_plane == 3)
    {
        scrollRightInternal(n, loresMode ? m_loresVideoMemory.data() : m_hiresVideoMemory.data(), width, height);
        scrollRightInternal(n, loresMode ? m_loresVideoMemoryPlane.data() : m_hiresVideoMemoryPlane.data(), width, height);
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

    if (m_plane == 1)
    {
        scrollLeftInternal(n, loresMode ? m_loresVideoMemory.data() : m_hiresVideoMemory.data(), width, height);
    }
    else if (m_plane == 2)
    {
        scrollLeftInternal(n, loresMode ? m_loresVideoMemoryPlane.data() : m_hiresVideoMemoryPlane.data(), width, height);
    }
    else if (m_plane == 3)
    {
        scrollLeftInternal(n, loresMode ? m_loresVideoMemory.data() : m_hiresVideoMemory.data(), width, height);
        scrollLeftInternal(n, loresMode ? m_loresVideoMemoryPlane.data() : m_hiresVideoMemoryPlane.data(), width, height);
    }
}

auto XoChipPpu::drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8
{
    if ((getMode() == PpuMode::LORES) || (getMode() == PpuMode::HIRES && n != 0)) // Draw 8xN sprite
    {
        if (m_plane == 1)
        {
            return draw8xNSprite(Vx, Vy, I_reg, memory, n, getMode() == PpuMode::LORES ? m_loresVideoMemory.data() : m_hiresVideoMemory.data());
        }
        else if (m_plane == 2)
        {
            return draw8xNSprite(Vx, Vy, I_reg, memory, n, getMode() == PpuMode::LORES ? m_loresVideoMemoryPlane.data() : m_hiresVideoMemoryPlane.data());
        }
        else if (m_plane == 3)
        {
            // TODO: Check if this is correct (http://johnearnest.github.io/Octo/docs/XO-ChipSpecification.html)
            return draw8xNSprite(Vx, Vy, I_reg, memory, n, getMode() == PpuMode::LORES ? m_loresVideoMemory.data() : m_hiresVideoMemory.data()) |
                   draw8xNSprite(Vx, Vy, I_reg + n, memory, n, getMode() == PpuMode::LORES ? m_loresVideoMemoryPlane.data() : m_hiresVideoMemoryPlane.data());
        }
    }
    else // Draw 16x16 sprite
    {
        if (m_plane == 1)
        {
            return draw16x16Sprite(Vx, Vy, I_reg, memory);
        }
        else if (m_plane == 2)
        {
            return draw16x16Sprite(Vx, Vy, I_reg, memory);
        }
        else if (m_plane == 3)
        {
            // TODO: Check if this is correct (http://johnearnest.github.io/Octo/docs/XO-ChipSpecification.html)
            return draw16x16Sprite(Vx, Vy, I_reg, memory) | draw16x16Sprite(Vx, Vy, I_reg + 32, memory); // 32 bytes per sprite (16x16)
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

auto XoChipPpu::draw16x16Sprite(uint8 Vx, uint8 Vy, uint16 I_reg, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory) -> bool
{
    bool collision = false;

    for (int i = 0; i < 16; i++) // 16 rows
    {
        for (int byteIndex = 0; byteIndex < 2; byteIndex++) // Two bytes per row (16 pixels) each pixel is 1 bit
        {
            for (int j = 0; j < 8; j++) // 8 pixels per byte
            {
                if (((memory[I_reg + i * 2 + byteIndex] >> (7 - j)) & 0x1) == 1)
                {
                    int x = (Vx + j + byteIndex * 8) % PpuBase::SCREEN_HIRES_MODE_WIDTH;
                    int y = (Vy + i) % PpuBase::SCREEN_HIRES_MODE_HEIGHT;

                    if (x >= PpuBase::SCREEN_HIRES_MODE_WIDTH || y >= PpuBase::SCREEN_HIRES_MODE_HEIGHT)
                    {
                        continue;
                    }

                    if (m_hiresVideoMemory[y * PpuBase::SCREEN_HIRES_MODE_WIDTH + x] == PIXEL_ON)
                    {
                        m_hiresVideoMemory[y * PpuBase::SCREEN_HIRES_MODE_WIDTH + x] = PIXEL_OFF;
                        collision = true;
                    }
                    else
                    {
                        m_hiresVideoMemory[y * PpuBase::SCREEN_HIRES_MODE_WIDTH + x] = PIXEL_ON;
                    }
                }
            }
        }
    }

    return static_cast<uint8>(collision);
}

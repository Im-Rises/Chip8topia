#include "Ppu.h"

void Ppu::clearScreen() {
    for (auto& pixel : m_videoMemory)
    {
        pixel = 0;
    }
}

void Ppu::drawSprite(uint8 x, uint8 y, uint8 n) {
    for (auto i = 0; i < n; ++i)
    {
        const auto spriteByte = m_videoMemory[i];
        for (auto j = 0; j < 8; ++j)
        {
            const auto spritePixel = spriteByte & (0x80 >> j);
            const auto screenPixel = m_videoMemory[i + j];
            if (spritePixel != 0)
            {
                if (screenPixel != 0u)
                {
                    m_videoMemory[i + j] = 0;
                }
                else
                {
                    m_videoMemory[i + j] = 1;
                }
            }
        }
    }
}

auto Ppu::getVideoMemory() const -> const std::array<uint8, 2048>& {
    return m_videoMemory;
}

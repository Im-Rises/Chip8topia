#include "Chip8VideoEmulation.h"

#include <binaryLib/binaryLib.h>

#include "../Chip8CoreBase/Chip8CoreBase.h"

Chip8VideoEmulation::Chip8VideoEmulation() : m_shaderXoChipLores(PpuBase::SCREEN_LORES_MODE_WIDTH, PpuBase::SCREEN_LORES_MODE_HEIGHT),
                                             m_shaderXoChipHires(PpuBase::SCREEN_HIRES_MODE_WIDTH, PpuBase::SCREEN_HIRES_MODE_HEIGHT)
{
    resetToGrayscaleColors();
}

void Chip8VideoEmulation::reset()
{
    resetToGrayscaleColors();
    m_shaderXoChipLores.reset();
    m_shaderXoChipHires.reset();
}

void Chip8VideoEmulation::resetToGrayscaleColors()
{
    for (int i = 0; i < m_colors.size(); i++)
    {
        const float color = static_cast<float>(i) / static_cast<float>(m_colors.size());
        m_colors[i] = ImVec4(color, color, color, 1.0F);
    }
}

void Chip8VideoEmulation::resetToColorColors()
{
    // Set each index to a color
    m_colors[0] = ImVec4(0.0F, 0.0F, 0.0F, 1.0F);
    m_colors[1] = ImVec4(0.0F, 0.0F, 0.5F, 1.0F);
    m_colors[2] = ImVec4(0.0F, 0.5F, 0.0F, 1.0F);
    m_colors[3] = ImVec4(0.0F, 0.5F, 0.5F, 1.0F);
    m_colors[4] = ImVec4(0.5F, 0.0F, 0.0F, 1.0F);
    m_colors[5] = ImVec4(0.5F, 0.0F, 0.5F, 1.0F);
    m_colors[6] = ImVec4(0.5F, 0.5F, 0.0F, 1.0F);
    m_colors[7] = ImVec4(0.5F, 0.5F, 0.5F, 1.0F);
    m_colors[8] = ImVec4(0.0F, 0.0F, 1.0F, 1.0F);
    m_colors[9] = ImVec4(0.0F, 1.0F, 0.0F, 1.0F);
    m_colors[10] = ImVec4(0.0F, 1.0F, 1.0F, 1.0F);
    m_colors[11] = ImVec4(1.0F, 0.0F, 0.0F, 1.0F);
    m_colors[12] = ImVec4(1.0F, 0.0F, 1.0F, 1.0F);
    m_colors[13] = ImVec4(1.0F, 1.0F, 0.0F, 1.0F);
    m_colors[14] = ImVec4(1.0F, 1.0F, 1.0F, 1.0F);
    m_colors[15] = ImVec4(0.0F, 0.0F, 0.0F, 1.0F);
}

void Chip8VideoEmulation::updateTexture(const std::unique_ptr<Chip8CoreBase>& core)
{
    const int planeMask = core->getPpu()->getPlane();
    if (core->getPpu()->getMode() == PpuBase::PpuMode::LORES)
    {
        for (int i = 0; i < PpuBase::PLANE_COUNT; i++)
        {
            if (getBit(planeMask, i))
            {
                m_shaderXoChipLores.updateTexture(core->getPpu()->getLoresVideoMemory(i).data(), i);
            }
        }
    }
    else
    {
        for (int i = 0; i < PpuBase::PLANE_COUNT; i++)
        {
            if (getBit(planeMask, i))
            {
                m_shaderXoChipHires.updateTexture(core->getPpu()->getHiresVideoMemory(i).data(), i);
            }
        }
    }
}

void Chip8VideoEmulation::update(const std::unique_ptr<Chip8CoreBase>& core, const float screenWidth, const float screenHeight, const float chip8AspectRatio)
{
    float screenAspectRatio = screenWidth / screenHeight;

    float scaleX = 1.0F;
    float scaleY = 1.0F;

    if (chip8AspectRatio > screenAspectRatio)
    {
        scaleY = screenAspectRatio / chip8AspectRatio;
    }
    else
    {
        scaleX = chip8AspectRatio / screenAspectRatio;
    }

    if (core->getPpu()->getMode() == PpuBase::PpuMode::LORES)
    {
        m_shaderXoChipLores.update(m_colors, scaleX, scaleY);
    }
    else
    {
        m_shaderXoChipHires.update(m_colors, scaleX, scaleY);
    }
}

auto Chip8VideoEmulation::getColorMode() const -> EmulationColorMode
{
    return m_colorMode;
}

auto Chip8VideoEmulation::getColor(const int index) -> ImVec4&
{
    return m_colors[index];
}

auto Chip8VideoEmulation::getLoresPlaneTexture(const int planeIndex) -> GLuint
{
    return m_shaderXoChipLores.getTexture(planeIndex);
}

auto Chip8VideoEmulation::getHiresPlaneTexture(const int planeIndex) -> GLuint
{
    return m_shaderXoChipHires.getTexture(planeIndex);
}

// auto Chip8VideoEmulation::getPlaneTexture(const PpuBase::PpuMode ppuMode, const int planeIndex) -> GLuint
//{
//     if (ppuMode == PpuBase::PpuMode::LORES)
//     {
//         return m_shaderLores.getTexture(planeIndex);
//     }
//     else
//     {
//         return m_shaderHires.getTexture(planeIndex);
//     }
//}

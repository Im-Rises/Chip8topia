#include "Chip8VideoEmulation.h"

#include <binaryLib/binaryLib.h>

#include "../Chip8CoreBase/Chip8CoreBase.h"

Chip8VideoEmulation::Chip8VideoEmulation() : m_shaderLores(PpuBase::SCREEN_LORES_MODE_WIDTH, PpuBase::SCREEN_LORES_MODE_HEIGHT),
                                             m_shaderHires(PpuBase::SCREEN_HIRES_MODE_WIDTH, PpuBase::SCREEN_HIRES_MODE_HEIGHT),
                                             m_shaderXoChipLores(PpuBase::SCREEN_LORES_MODE_WIDTH, PpuBase::SCREEN_LORES_MODE_HEIGHT),
                                             m_shaderXoChipHires(PpuBase::SCREEN_HIRES_MODE_WIDTH, PpuBase::SCREEN_HIRES_MODE_HEIGHT)
{
    resetColors();
}

void Chip8VideoEmulation::reset()
{
    resetColors();
}

void Chip8VideoEmulation::resetColors()
{
    for (int i = 0; i < m_colors.size(); i++)
    {
        const float color = static_cast<float>(i) / static_cast<float>(m_colors.size());
        m_colors[i] = ImVec4(color, color, color, 1.0F);
    }
}

void Chip8VideoEmulation::updateTexture(const std::unique_ptr<Chip8CoreBase>& core)
{
    switch (core->getType())
    {
    case Chip8CoreType::Chip8:
    case Chip8CoreType::SChip11Legacy:
    case Chip8CoreType::SChip11Modern:
    case Chip8CoreType::SChipC:
    {
        if (core->getPpu()->getMode() == PpuBase::PpuMode::LORES)
        {
            m_shaderLores.updateTexture(core->getPpu()->getLoresVideoMemory(0).data());
        }
        else
        {
            m_shaderHires.updateTexture(core->getPpu()->getHiresVideoMemory(0).data());
        }
        break;
    }
    case Chip8CoreType::XoChip:
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
        break;
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

    switch (core->getType())
    {
    case Chip8CoreType::Chip8:
    case Chip8CoreType::SChip11Legacy:
    case Chip8CoreType::SChip11Modern:
    case Chip8CoreType::SChipC:
    {
        if (core->getPpu()->getMode() == PpuBase::PpuMode::LORES)
        {
            m_shaderLores.update(m_colors[SCREEN_BACKGROUND_COLOR_INDEX], m_colors[SCREEN_MAIN_PLANE_COLOR_INDEX], scaleX, scaleY);
        }
        else
        {
            m_shaderHires.update(m_colors[SCREEN_BACKGROUND_COLOR_INDEX], m_colors[SCREEN_MAIN_PLANE_COLOR_INDEX], scaleX, scaleY);
        }
        break;
    }
    case Chip8CoreType::XoChip:
    {
        if (core->getPpu()->getMode() == PpuBase::PpuMode::LORES)
        {
            m_shaderXoChipLores.update(m_colors, scaleX, scaleY);
        }
        else
        {
            m_shaderXoChipHires.update(m_colors, scaleX, scaleY);
        }
        break;
    }
    }
}

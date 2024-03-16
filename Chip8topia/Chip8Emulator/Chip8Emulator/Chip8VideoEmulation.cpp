#include "Chip8VideoEmulation.h"

#include "../Chip8CoreBase/Core/PpuBase.h"
#include "../Chip8CoreBase/Chip8CoreBase.h"

Chip8VideoEmulation::Chip8VideoEmulation() : m_shaderLores(PpuBase::SCREEN_LORES_MODE_WIDTH, PpuBase::SCREEN_LORES_MODE_HEIGHT),
                                             m_shaderHires(PpuBase::SCREEN_HIRES_MODE_WIDTH, PpuBase::SCREEN_HIRES_MODE_HEIGHT),
                                             m_shaderXoChipLores(PpuBase::SCREEN_LORES_MODE_WIDTH, PpuBase::SCREEN_LORES_MODE_HEIGHT),
                                             m_shaderXoChipHires(PpuBase::SCREEN_HIRES_MODE_WIDTH, PpuBase::SCREEN_HIRES_MODE_HEIGHT)
{
}

void Chip8VideoEmulation::reset()
{
}

void Chip8VideoEmulation::updateTexture(const std::unique_ptr<Chip8CoreBase>& core)
{
    if (core->getPpu()->getMode() == PpuBase::PpuMode::LORES)
    {
        m_shaderLores.updateTexture(core->getPpu()->getLoresVideoMemory().data());
    }
    else
    {
        m_shaderHires.updateTexture(core->getPpu()->getHiresVideoMemory().data());
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
        m_shaderLores.update(m_backgroundColor, m_mainPlaneColor, scaleX, scaleY);
    }
    else
    {
        m_shaderHires.update(m_backgroundColor, m_mainPlaneColor, scaleX, scaleY);
    }
}

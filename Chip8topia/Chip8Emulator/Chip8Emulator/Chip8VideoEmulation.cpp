#include "Chip8VideoEmulation.h"

#include "../Chip8CoreBase/Core/PpuBase.h"
#include "../Chip8CoreBase/Chip8CoreBase.h"

Chip8VideoEmulation::Chip8VideoEmulation() : m_shaderLores(PpuBase::SCREEN_LORES_MODE_WIDTH, PpuBase::SCREEN_LORES_MODE_HEIGHT),
                                             m_shaderHires(PpuBase::SCREEN_HIRES_MODE_WIDTH, PpuBase::SCREEN_HIRES_MODE_HEIGHT) {
}

void Chip8VideoEmulation::reset() {
}

void Chip8VideoEmulation::updateTexture(const std::unique_ptr<Chip8CoreBase>& core) {
    if (core->getPpu()->getMode() == PpuBase::PpuMode::LORES)
    {
        m_shaderLores.updateTexture(core->getPpu()->getVideoMemory());
    }
    else
    {
        m_shaderHires.updateTexture(core->getPpu()->getVideoMemory());
    }
}

void Chip8VideoEmulation::update(const std::unique_ptr<Chip8CoreBase>& core) {
    if (core->getPpu()->getMode() == PpuBase::PpuMode::LORES)
    {
        m_shaderLores.update();
    }
    else
    {
        m_shaderHires.update();
    }
}

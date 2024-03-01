#include "Chip8CoreBase.h"

#include <utility>

#include "Core/Input.h"
#include "Core/CpuBase.h"
#include "Core/PpuBase.h"

Chip8CoreBase::Chip8CoreBase(std::unique_ptr<CpuBase> cpu, std::shared_ptr<PpuBase> ppu) : m_cpu(std::move(cpu)),
                                                                                           m_ppu(std::move(ppu)),
                                                                                           m_input(std::make_shared<Input>()),
                                                                                           m_clockCounter(0) {
    m_cpu->setPpu(m_ppu);
    m_cpu->setInput(m_input);
}

void Chip8CoreBase::readRom(const std::vector<uint8>& rom) {
    m_cpu->readRom(rom);
}

void Chip8CoreBase::clock() {
    m_cpu->clock();
    m_clockCounter++;

    if (m_clockCounter >= CpuBase::CLOCK_FREQUENCY / SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_cpu->clockTimers();
        m_clockCounter = 0;
    }
}

void Chip8CoreBase::updateKey(const uint8 key, const bool pressed) {
    m_input->updateKey(key, pressed);
}

void Chip8CoreBase::reset() {
    m_cpu->reset();
    m_ppu->clearScreen();
    m_input->reset();
}

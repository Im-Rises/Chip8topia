#include "SChip11Core.h"

#include "Core/SChip11Cpu.h"
#include "Core/SChip11Ppu.h"

SChip11Core::SChip11Core() : Chip8CoreBase(std::make_unique<SChip11Cpu>(), std::make_shared<SChip11Ppu>()) {
}

void SChip11Core::clock() {
    while (m_clockCounter < CpuBase::CLOCK_FREQUENCY / SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_cpu->clock();
        m_clockCounter++;
    }

    m_cpu->clockTimers();
    m_clockCounter = 0;
}

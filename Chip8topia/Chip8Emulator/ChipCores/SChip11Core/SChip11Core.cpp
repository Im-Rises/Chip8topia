#include "SChip11Core.h"

#include "Core/SChip11Ppu.h"

SChip11Core::SChip11Core(Chip8Frequency cpuClockFrequency, bool isModernMode) : Chip8CoreBase(cpuClockFrequency, std::make_unique<SChip11Cpu>(isModernMode), std::make_shared<SChip11Ppu>()), m_cpuCasted(dynamic_cast<SChip11Cpu*>(m_cpu.get())) {
}

auto SChip11Core::clock() -> bool {
    m_cpu->clock();
    m_clockCounter++;

    if (m_clockCounter >= CPU_CLOCK_FREQUENCY / SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_cpu->clockTimers();
        m_cpuCasted->requestDisableHalt();
        m_clockCounter = 0;

        return true;
    }

    return false;
}

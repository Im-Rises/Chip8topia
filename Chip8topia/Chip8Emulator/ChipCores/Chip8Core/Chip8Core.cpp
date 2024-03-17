#include "Chip8Core.h"

#include "Core/Chip8Cpu.h"
#include "Core/Chip8Ppu.h"

Chip8Core::Chip8Core(Chip8Frequency cpuClockFrequency) : Chip8CoreBase(cpuClockFrequency, std::make_unique<Chip8Cpu>(), std::make_shared<Chip8Ppu>()), m_cpuCasted(dynamic_cast<Chip8Cpu*>(m_cpu.get())) {
}

auto Chip8Core::clock() -> bool {
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

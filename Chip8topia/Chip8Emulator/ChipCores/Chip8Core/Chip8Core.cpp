#include "Chip8Core.h"

#include "Core/Chip8Cpu.h"
#include "Core/Chip8Ppu.h"

Chip8Core::Chip8Core(unsigned int cpuClockFrequency) : Chip8CoreBase(cpuClockFrequency, std::make_unique<Chip8Cpu>(), std::make_shared<Chip8Ppu>()), m_cpuCasted(dynamic_cast<Chip8Cpu*>(m_cpu.get())) {
}

auto Chip8Core::clock() -> bool {
    m_cpu->clock();
    m_clockCounter++;

    if (m_clockCounter >= m_cpuClockFrequency / SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_cpu->clockTimers();
        m_cpuCasted->requestDisableHalt();
        //    dynamic_cast<Cpu*>(m_cpu.get())->requestDisableHalt(); // TODO: Find a better solution...
        m_clockCounter = 0;

        return true;
    }

    return false;
}

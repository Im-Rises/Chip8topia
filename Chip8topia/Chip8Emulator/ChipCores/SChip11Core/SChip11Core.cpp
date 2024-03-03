#include "SChip11Core.h"

#include "Core/SChip11Cpu.h"
#include "Core/SChip11Ppu.h"

SChip11Core::SChip11Core(Chip8Frequency cpuClockFrequency) : Chip8CoreBase(cpuClockFrequency, std::make_unique<SChip11Cpu>(), std::make_shared<SChip11Ppu>()), m_cpuCasted(dynamic_cast<SChip11Cpu*>(m_cpu.get())) {
}

auto SChip11Core::clock() -> bool {
    m_cpu->clock();
    m_clockCounter++;

    if (m_clockCounter >= m_cpuClockFrequency / SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_cpu->clockTimers();
        m_cpuCasted->requestDisableHalt();
        //   dynamic_cast<Cpu*>(m_cpu.get())->requestDisableHalt(); // TODO: Find a better solution...
        m_clockCounter = 0;

        return true;
    }

    return false;
}

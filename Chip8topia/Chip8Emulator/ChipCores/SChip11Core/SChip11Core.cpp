#include "SChip11Core.h"

// #include "Core/Chip8Cpu.h"
// #include "Core/Chip8Ppu.h"

SChip11Core::SChip11Core() : Chip8CoreBase(std::make_unique<SChip11Cpu>(), std::make_shared<SChip11Ppu>()), m_cpuCasted(dynamic_cast<Chip8Cpu*>(m_cpu.get())) {
}

void SChip11Core::clock() {
    while (m_clockCounter < Chip8BaseCpu::CLOCK_FREQUENCY / SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_cpu->clock();
        m_clockCounter++;
    }

    m_cpu->clockTimers();
    //    dynamic_cast<Cpu*>(m_cpu.get())->requestDisableHalt(); // TODO: Find a better solution...
    m_cpuCasted->requestDisableHalt();
    m_clockCounter = 0;
}

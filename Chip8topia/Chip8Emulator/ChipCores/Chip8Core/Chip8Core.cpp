#include "Chip8Core.h"

#include "Core/Chip8Cpu.h"
#include "Core/Chip8Ppu.h"

Chip8Core::Chip8Core() : Chip8CoreBase(std::make_unique<Chip8Cpu>(), std::make_shared<Chip8Ppu>()), m_cpuCasted(dynamic_cast<Chip8Cpu*>(m_cpu.get())) {
}

void Chip8Core::clock() {
    while (m_clockCounter < CpuBase::CLOCK_FREQUENCY / SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_cpu->clock();
        m_clockCounter++;
    }

    m_cpu->clockTimers();
    //    dynamic_cast<Cpu*>(m_cpu.get())->requestDisableHalt(); // TODO: Find a better solution...
    m_cpuCasted->requestDisableHalt();
    m_clockCounter = 0;
}

#include "Chip8Core.h"

#include "Core/Chip8Cpu.h"
#include "Core/Chip8Ppu.h"

Chip8Core::Chip8Core() : Chip8CoreBase(std::make_unique<Chip8Cpu>(), std::make_shared<Chip8Ppu>()), m_cpuCasted(dynamic_cast<Chip8Cpu*>(m_cpu.get())) {
}

void Chip8Core::clock() {
    m_cpu->clock();
    m_clockCounter++;

    if (m_clockCounter >= CpuBase::CLOCK_FREQUENCY / SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_cpu->clockTimers();
        m_cpuCasted->requestDisableHalt();
        //    dynamic_cast<Cpu*>(m_cpu.get())->requestDisableHalt(); // TODO: Find a better solution...
        m_clockCounter = 0;
    }
}

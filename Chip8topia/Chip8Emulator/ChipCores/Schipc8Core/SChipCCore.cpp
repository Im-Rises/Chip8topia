#include "SChipCCore.h"

#include "Core/SChipCCpu.h"
#include "Core/SChipCPpu.h"

SChipCCore::SChipCCore() : Chip8CoreBase(std::make_unique<SChipCCpu>(), std::make_shared<SChipCPpu>()) {
}

void SChipCCore::clock() {
    while (m_clockCounter < CpuBase::CLOCK_FREQUENCY / SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_cpu->clock();
        m_clockCounter++;
    }

    m_cpu->clockTimers();
    m_clockCounter = 0;
}
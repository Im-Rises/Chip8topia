#include "Chip8CoreBase.h"

#include <utility>

#include "Core/Input.h"
#include "Core/CpuBase.h"
#include "Core/PpuBase.h"

Chip8CoreBase::Chip8CoreBase(Chip8Frequency cpuClockFrequency, std::unique_ptr<CpuBase> cpu, std::shared_ptr<PpuBase> ppu) : CPU_CLOCK_FREQUENCY(static_cast<unsigned int>(cpuClockFrequency)),
                                                                                                                             m_cpu(std::move(cpu)),
                                                                                                                             m_ppu(std::move(ppu)),
                                                                                                                             m_input(std::make_shared<Input>()),
                                                                                                                             m_clockCounter(0)
{
    m_cpu->setPpu(m_ppu);
    m_cpu->setInput(m_input);
}


#if defined(BUILD_PARAM_SAFE)
void Chip8CoreBase::setErrorCallback(const std::function<void(const std::string&)>& errorCallback)
{
    m_errorCallback = errorCallback;
    m_cpu->setErrorCallback(errorCallback);
    m_ppu->setErrorCallback(errorCallback);
    m_input->setErrorCallback(errorCallback);
}
#endif

void Chip8CoreBase::readRom(const std::vector<uint8>& rom)
{
    m_cpu->readRom(rom);
}

auto Chip8CoreBase::clock() -> bool
{
    m_cpu->clock();
    m_clockCounter++;

    if (m_clockCounter >= CPU_CLOCK_FREQUENCY / SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_cpu->clockTimers();
        m_clockCounter = 0;

        return true;
    }

    return false;
}

void Chip8CoreBase::updateKey(const uint8 key, const bool pressed)
{
    m_input->updateKey(key, pressed);
}

void Chip8CoreBase::reset()
{
    m_cpu->reset();
    m_ppu->clearScreen();
    m_input->reset();
}

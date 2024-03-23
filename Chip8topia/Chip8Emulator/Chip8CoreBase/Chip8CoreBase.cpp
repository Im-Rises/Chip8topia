#include "Chip8CoreBase.h"

#include <utility>

#include "Core/Input.h"
#include "Core/CpuBase.h"
#include "Core/PpuBase.h"

Chip8CoreBase::Chip8CoreBase(Chip8Frequency cpuClockFrequency, std::unique_ptr<CpuBase> cpu, std::shared_ptr<PpuBase> ppu) : CPU_CLOCK_FREQUENCY(static_cast<unsigned int>(cpuClockFrequency)),
                                                                                                                             CPU_CLOCK_FREQUENCY_ENUM(cpuClockFrequency),
                                                                                                                             m_cpu(std::move(cpu)),
                                                                                                                             m_ppu(std::move(ppu)),
                                                                                                                             m_input(std::make_shared<Input>()),
                                                                                                                             m_clockCounter(0)
{
    m_cpu->setPpu(m_ppu);
    m_cpu->setInput(m_input);
}

void Chip8CoreBase::reset()
{
    m_cpu->reset();
    m_ppu->reset();
    m_input->reset();
}

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

auto Chip8CoreBase::getFrequency() const -> Chip8Frequency
{
    return CPU_CLOCK_FREQUENCY_ENUM;
}

auto Chip8CoreBase::getCpu() -> std::unique_ptr<CpuBase>&
{
    return m_cpu;
}

auto Chip8CoreBase::getPpu() -> std::shared_ptr<PpuBase>&
{
    return m_ppu;
}

auto Chip8CoreBase::getInput() -> std::shared_ptr<Input>&
{
    return m_input;
}

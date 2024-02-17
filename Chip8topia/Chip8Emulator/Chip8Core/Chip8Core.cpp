#include "Chip8Core.h"

#include "Core/Input.h"
#include "Core/Ppu.h"

Chip8Core::Chip8Core() : m_ppu(std::make_shared<Ppu>()),
                         m_input(std::make_shared<Input>()),
                         m_clockCounter(0) {
    m_cpu.setPpu(m_ppu);
    m_cpu.setInput(m_input);
}

void Chip8Core::readRom(const std::vector<uint8>& rom) {
    m_cpu.readRom(rom);
}

void Chip8Core::clock() {
    while (m_clockCounter < Cpu::CLOCK_FREQUENCY / SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_cpu.clock();
        m_clockCounter++;
    }

    m_cpu.clockTimers();
    m_cpu.requestDisableHalt();
    m_clockCounter = 0;
}

void Chip8Core::updateKey(const uint8 key, const bool pressed) {
    m_input->updateKey(key, pressed);
}

void Chip8Core::reset() {
    m_cpu.reset();
    m_ppu->clearScreen();
    m_input->reset();
}

auto Chip8Core::getCpu() -> Cpu& {
    return m_cpu;
}

auto Chip8Core::getPpu() -> std::shared_ptr<Ppu> {
    return m_ppu;
}

auto Chip8Core::getInput() -> std::shared_ptr<Input> {
    return m_input;
}

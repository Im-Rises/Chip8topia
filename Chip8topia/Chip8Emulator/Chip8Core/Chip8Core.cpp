#include "Chip8Core.h"

#include "Core/Input.h"
#include "Core/Ppu.h"

Chip8Core::Chip8Core() : m_ppu(), m_input(), m_cpu() {
}

void Chip8Core::readRom(const uint8* rom, const size_t romSize) {
    m_cpu.readRom(rom, romSize);
}

void Chip8Core::clock() {
    m_cpu.clock();
}

void Chip8Core::updateKey(const uint8 key, const bool pressed) {
    m_input->updateKey(key, pressed);
}

void Chip8Core::reset() {
    m_cpu.reset();
    m_ppu->clearScreen();
    m_input->reset();
}

#include "Chip8Emulator.h"

void Chip8Emulator::update(const float deltaTime) {
    m_core.clock();
}

void Chip8Emulator::readRom(const uint8* rom, const size_t romSize) {
    m_core.readRom(rom, romSize);
}

#include "Chip8Emulator.h"

void Chip8Emulator::readRom(const uint8* rom, const size_t romSize) {
    m_core.readRom(rom, romSize);
    m_isRomLoaded = true;
}

void Chip8Emulator::update(const float deltaTime) {
    if (!m_isRomLoaded)
    {
        return;
    }

    m_core.clock();
    m_videoEmulation.update(deltaTime);
    m_soundEmulation.update(deltaTime);
}

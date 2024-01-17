#pragma once

#include "Chip8Core/Chip8Core.h"
#include "Chip8Emulation/Chip8VideoEmulation.h"
#include "Chip8Emulation/Chip8SoundEmulation.h"

class Chip8Emulator {
public:
    void update(const float deltaTime);

    void readRom(const uint8* rom, const size_t romSize);

private:
    Chip8Core m_core;
    Chip8VideoEmulation m_videoEmulation;
    Chip8SoundEmulation m_soundEmulation;
};

#pragma once

#include "Chip8Core/Chip8Core.h"
#include "Chip8Emulation/Chip8VideoEmulation.h"
#include "Chip8Emulation/Chip8SoundEmulation.h"

class Chip8Emulator {
public:
    void update(float deltaTime);

private:
    Chip8Core m_core;
    Chip8VideoEmulation m_videoEmulation;
    Chip8SoundEmulation m_soundEmulation;
};

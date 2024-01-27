#pragma once

#include "Chip8Core/Chip8Core.h"
#include "Chip8Emulation/Chip8VideoEmulation.h"
#include "Chip8Emulation/Chip8SoundEmulation.h"
#include "Chip8Emulation/Chip8RomLoader.h"

class Chip8Emulator {
public:
    Chip8Emulator();
    Chip8Emulator(const Chip8Emulator&) = delete;
    Chip8Emulator(Chip8Emulator&&) = delete;
    auto operator=(const Chip8Emulator&) -> Chip8Emulator& = delete;
    auto operator=(Chip8Emulator&&) -> Chip8Emulator& = delete;
    ~Chip8Emulator();

public:
    void readRom(const char* romPath);
    void update(const float deltaTime);

    void toglleTurboMode();
    void togglePause();

private:
    void OnInput(const uint8 key, const bool isPressed);

private:
    Chip8Core m_core;
    Chip8VideoEmulation m_videoEmulation;
    Chip8SoundEmulation m_soundEmulation;
    Chip8RomLoader m_romLoader;

    bool m_isRomLoaded = false;
    bool m_isTurboMode = false;
    bool m_isPaused = false;

    float m_accumulator = 0.0F;
};

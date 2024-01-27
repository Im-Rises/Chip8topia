#pragma once

#include "Chip8Core/Chip8Core.h"
#include "Chip8Emulation/Chip8VideoEmulation.h"
#include "Chip8Emulation/Chip8SoundEmulation.h"

class Chip8Emulator {
public:
    Chip8Emulator();
    Chip8Emulator(const Chip8Emulator&) = delete;
    Chip8Emulator(Chip8Emulator&&) = delete;
    auto operator=(const Chip8Emulator&) -> Chip8Emulator& = delete;
    auto operator=(Chip8Emulator&&) -> Chip8Emulator& = delete;
    ~Chip8Emulator();

public:
    void readRom(const uint8* rom, const size_t romSize);
    void update(const float deltaTime);

    [[nodiscard]] auto getIsRomLoaded() const -> bool;

private:
    //    void updateCore(const float deltaTime);
    //    void updateVideoEmulation();
    //    void updateSoundEmulation();

    void OnInput(const uint8 key, const bool isPressed);

private:
    Chip8Core m_core;
    Chip8VideoEmulation m_videoEmulation;
    Chip8SoundEmulation m_soundEmulation;

    bool m_isRomLoaded = false;
};

#pragma once

#include "ChipCores/Chip8Core/Chip8Core.h"
#include "Chip8Emulator/Chip8VideoEmulation.h"
// #include "Chip8Emulation/Chip8SoundEmulation.h"

class Chip8Emulator {
public:
    Chip8Emulator();
    Chip8Emulator(const Chip8Emulator&) = delete;
    Chip8Emulator(Chip8Emulator&&) = delete;
    auto operator=(const Chip8Emulator&) -> Chip8Emulator& = delete;
    auto operator=(Chip8Emulator&&) -> Chip8Emulator& = delete;
    ~Chip8Emulator();

public:
    void restart();
    //    void loadRom(const std::string& romPath);
    void loadRom(const std::vector<uint8_t>& romData);

    void update(const float deltaTime);
    void render();

    void togglePause();

    void setIsTurboMode(const bool isTurboMode);
    [[nodiscard]] auto getIsPaused() const -> bool;
    [[nodiscard]] auto getChip8Core() -> Chip8CoreBase*;
    [[nodiscard]] auto getChip8VideoEmulation() -> Chip8VideoEmulation&;

    void setRomName(const std::string& romName) { m_romName = romName; }
    [[nodiscard]] auto getRomName() const -> std::string { return m_romName; }
    [[nodiscard]] static auto getConsoleName() -> std::string { return "Chip8"; } // TODO: Move to a more appropriate place

    void switchCore(const Chip8CoreType coreType);
    [[nodiscard]] auto getCoreType() const -> Chip8CoreType;
    void switchFrequency(const Chip8Frequency frequency);
    [[nodiscard]] auto getFrequency() const -> Chip8Frequency;

private:
    void OnInput(const uint8 key, const bool isPressed);

private:
    std::string m_romName = "ROM";

    std::unique_ptr<Chip8CoreBase> m_core;
    Chip8VideoEmulation m_videoEmulation;
    //    Chip8SoundEmulation m_soundEmulation;

    bool m_isRomLoaded = false;
    bool m_isTurboMode = false;
    bool m_isPaused = false;

    float m_accumulator = 0.0F;
};

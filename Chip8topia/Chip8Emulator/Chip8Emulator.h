#pragma once

#include <bitset>

#include "ChipCores/Chip8Core/Chip8Core.h"
#include "Chip8CoreBase/Core/CpuBase.h"
#include "Chip8Emulator/Chip8VideoEmulation.h"
// #include "Chip8Emulation/Chip8SoundEmulation.h"

class Chip8Emulator {
public:
    static constexpr auto CHIP8_ASPECT_RATIO = 2.0F;
    static constexpr auto DEFAULT_CORE_TYPE = Chip8CoreType::Chip8;
    static constexpr auto DEFAULT_FREQUENCY = Chip8Frequency::Freq1200Hz;

public:
    Chip8Emulator();
    Chip8Emulator(const Chip8Emulator&) = delete;
    Chip8Emulator(Chip8Emulator&&) = delete;
    auto operator=(const Chip8Emulator&) -> Chip8Emulator& = delete;
    auto operator=(Chip8Emulator&&) -> Chip8Emulator& = delete;
    ~Chip8Emulator();

public:
    void restart();
    void loadRom(const std::vector<uint8_t>& romData);

    void update(const float deltaTime);
    void render(const float screenWidth, const float screenHeight);

    void stop();

    void setIsTurboMode(const bool isTurboMode);
    [[nodiscard]] auto getChip8Core() -> Chip8CoreBase*;
    [[nodiscard]] auto getChip8VideoEmulation() -> Chip8VideoEmulation&;

    void setRomName(const std::string& romName) {
        m_romName = romName;
    }
    [[nodiscard]] auto getRomName() const -> std::string {
        return m_romName;
    }
    [[nodiscard]] auto getConsoleName() -> std::string {
        return m_core->getConsoleName();
    }

    [[nodiscard]] auto getCoreType() const -> Chip8CoreType;
    [[nodiscard]] auto getFrequency() const -> Chip8Frequency;
    void switchCoreFrequency(const Chip8CoreType coreType, const Chip8Frequency frequency);

    auto getCanBreak() -> bool* {
        return &m_canBreak;
    }

    auto getBreakpoints() -> std::bitset<CpuBase::MEMORY_SIZE>& {
        return m_breakpoints;
    }
    void clearBreakpoints() {
        m_breakpoints.reset();
    }

    void stepEmulation() {
        m_isBreak = true;
        m_stepNextFrame = true;
    }

    void runEmulation() {
        m_isBreak = false;
    }

    void breakEmulation() {
        m_isBreak = true;
    }

    void toggleBreakEmulation() {
        m_isBreak = !m_isBreak;
    }

    auto getIsBreak() const -> bool {
        return m_isBreak;
    }

    [[nodiscard]] auto getIsRomLoaded() const -> bool {
        return m_isRomLoaded;
    }

private:
    void OnInput(const uint8 key, const bool isPressed);
#if defined(BUILD_PARAM_SAFE)
    void errorCallback(const std::string& errorMessage);
//    void warningCallback(const std::string& errorMessage);
#endif

private:
    std::string m_romName = "ROM";

    std::unique_ptr<Chip8CoreBase> m_core;
    Chip8VideoEmulation m_videoEmulation;
    //    Chip8SoundEmulation m_soundEmulation;

    bool m_isRomLoaded = false;
    bool m_isTurboMode = false;

    float m_accumulator = 0.0F;

    bool m_isBreak = false;
    bool m_stepNextFrame = false;
    bool m_canBreak = true;

    std::bitset<CpuBase::MEMORY_SIZE> m_breakpoints;
};

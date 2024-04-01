#pragma once

#include <set>

#include "ChipCores/Chip8Core/Chip8Core.h"
#include "Chip8CoreBase/Core/CpuBase.h"
#include "Chip8Emulator/Chip8VideoEmulation.h"
// #include "Chip8Emulation/Chip8SoundEmulation.h"

class Chip8Emulator
{
public:
    static constexpr float CHIP8_ASPECT_RATIO = 2.0F;
    static constexpr Chip8CoreType DEFAULT_CORE_TYPE = Chip8CoreType::XoChip;
    static constexpr Chip8Frequency DEFAULT_FREQUENCY = Chip8Frequency::Freq1200Hz;

private:
    static constexpr int TOAST_DURATION_INFO = 1000;
    static constexpr int TOAST_DURATION_SUCCESS = 1000;
    static constexpr int TOAST_DURATION_ERROR = 3000;

public:
    Chip8Emulator();
    Chip8Emulator(const Chip8Emulator&) = delete;
    Chip8Emulator(Chip8Emulator&&) = delete;
    auto operator=(const Chip8Emulator&) -> Chip8Emulator& = delete;
    auto operator=(Chip8Emulator&&) -> Chip8Emulator& = delete;
    ~Chip8Emulator();

public:
    void resetColorPalette();
    void restart();
    void loadRom(const std::vector<uint8_t>& romData);

    void update(const float deltaTime);
    void render(const float screenWidth, const float screenHeight);

    [[nodiscard]] auto getChip8Core() -> Chip8CoreBase*;
    [[nodiscard]] auto getChip8VideoEmulation() -> Chip8VideoEmulation&;
    [[nodiscard]] auto getRomName() const -> std::string;
    [[nodiscard]] auto getConsoleName() -> std::string;
    [[nodiscard]] auto getIsBreak() const -> bool;
    [[nodiscard]] auto getIsRomLoaded() const -> bool;
    [[nodiscard]] auto getCanBreak() -> bool*;
    [[nodiscard]] auto getBreakpoints() -> std::set<uint16>&;
    [[nodiscard]] auto getCoreType() const -> Chip8CoreType;
    [[nodiscard]] auto getFrequency() const -> Chip8Frequency;

    void stop();
#if defined(BUILD_PARAM_SAFE)
    void triggerEmulationError(const std::string& message);
#endif
    void setIsTurboMode(const bool isTurboMode);
    void setRomName(const std::string& romName);
    void switchCoreFrequency(const Chip8CoreType coreType, const Chip8Frequency frequency);
    void clearBreakpoints();
    void stepEmulation();
    void runEmulation();
    void breakEmulation();
    void toggleBreakEmulation();

private:
    void OnInput(const uint8 key, const bool isPressed);

private:
    std::string m_romName = "ROM";

    std::unique_ptr<Chip8CoreBase> m_core;
    Chip8VideoEmulation m_videoEmulation;
    //    Chip8SoundEmulation m_soundEmulation;

    // TODO: Move to constructor
    float m_accumulator = 0.0F;
    bool m_isRomLoaded = false;
    bool m_isTurboMode = false;
    bool m_isBreak = false;
    bool m_step = false;
    bool m_canBreak = true;
    //    bool m_errorTriggered = false;

    std::set<uint16> m_breakpoints;
};

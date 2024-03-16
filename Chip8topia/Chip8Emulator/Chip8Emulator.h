#pragma once

#include <bitset>

#include "ChipCores/Chip8Core/Chip8Core.h"
#include "Chip8CoreBase/Core/CpuBase.h"
#include "Chip8Emulator/Chip8VideoEmulation.h"
// #include "Chip8Emulation/Chip8SoundEmulation.h"

class Chip8Emulator
{
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

    [[nodiscard]] auto getChip8Core() -> Chip8CoreBase*;
    [[nodiscard]] auto getChip8VideoEmulation() -> Chip8VideoEmulation&;
    [[nodiscard]] auto getRomName() const -> std::string;
    [[nodiscard]] auto getConsoleName() -> std::string;
    [[nodiscard]] auto getIsBreak() const -> bool;
    [[nodiscard]] auto getIsRomLoaded() const -> bool;
    [[nodiscard]] auto getCanBreak() -> bool*;
    [[nodiscard]] auto getBreakpoints() -> std::bitset<CpuBase::MEMORY_SIZE>&;
    [[nodiscard]] auto getCoreType() const -> Chip8CoreType;
    [[nodiscard]] auto getFrequency() const -> Chip8Frequency;

    void stop();
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

    std::bitset<CpuBase::MEMORY_SIZE> m_breakpoints; // TODO: Maybe use a map or a set instead of a bitset, this way we can access quickly to the elements and only on the elements we need to access (for the draw function)
};

#pragma once

#include <set>

#include "ChipCores/Chip8Core/Chip8Core.h"
#include "Chip8CoreBase/Core/CpuBase.h"
#include "Chip8Emulator/Chip8VideoEmulation.h"
#include "Chip8Emulator/Chip8SoundEmulation.h"

#if defined(BUILD_PARAM_SAFE)
#define TRIGGER_ERROR(condition, ...)                                                                            \
    do                                                                                                           \
    {                                                                                                            \
        static_assert(std::is_same<decltype(condition), bool>::value, "Condition must be a boolean expression"); \
        if ((condition))                                                                                         \
        {                                                                                                        \
            Chip8topiaInputHandler::getInstance().m_EmulationError.trigger(fmt::format(__VA_ARGS__));            \
        }                                                                                                        \
    } while (false)
#else
#define TRIGGER_ERROR(condition, message, ...) \
    do                                         \
    {                                          \
        (void)(message);                       \
    } while (false)
#endif

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
    //    static constexpr int PC_HISTORY_SIZE = 10;

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
    void emitSound();
    void render(const float screenWidth, const float screenHeight);

    void stop();
    void setSoundVolume(float volume);

#if defined(BUILD_PARAM_SAFE)
    void triggerEmulationError(const std::string& message);
#endif
    //    void setIsTurboMode(const bool isTurboMode);
    void setRomName(const std::string& romName);
    void switchCoreFrequency(const Chip8CoreType coreType, const Chip8Frequency frequency);
    void clearBreakpoints();
    void stepEmulation();
    void runEmulation();
    void breakEmulation();
    void toggleBreakEmulation();
    //    void updatePcHistory();

private:
    void OnInput(const uint8 key, const bool isPressed);

public:
    [[nodiscard]] auto getChip8Core() -> Chip8CoreBase*;
    [[nodiscard]] auto getChip8VideoEmulation() -> Chip8VideoEmulation&;
    [[nodiscard]] auto getChip8SoundEmulation() -> Chip8SoundEmulation&;
    [[nodiscard]] auto getRomName() const -> std::string;
    [[nodiscard]] auto getConsoleName() -> std::string;
    [[nodiscard]] auto getIsBreak() const -> bool;
    [[nodiscard]] auto getIsRomLoaded() const -> bool;
    [[nodiscard]] auto getCanBreak() -> bool*;
    [[nodiscard]] auto getBreakpointsStates() -> std::array<bool, CpuBase::MEMORY_SIZE>&;
    [[nodiscard]] auto getBreakpointsList() -> std::set<uint16>&;
    [[nodiscard]] auto getCoreType() const -> Chip8CoreType;
    [[nodiscard]] auto getFrequency() const -> Chip8Frequency;

private:
    std::string m_romName = "ROM";

    std::unique_ptr<Chip8CoreBase> m_core;
    Chip8VideoEmulation m_videoEmulation;
    Chip8SoundEmulation m_soundEmulation;

    bool m_isRomLoaded;
    bool m_isBreak;
    bool m_step;
    bool m_canBreak;
    bool m_errorTriggered;

    std::set<uint16> m_breakpointsList;
    std::array<bool, CpuBase::MEMORY_SIZE> m_breakpointsStates;
};

#pragma once

#include <array>
#include <IconsFontAwesome6.h>

#include "../ImGuiHelper/ImGuiHelper.h"
#include "../../Chip8Emulator/Chip8Emulator.h"
#include "Chip8topiaPerformanceMonitor.h"

class Chip8topia;
class Chip8topiaEmulationUi
{
private:
#if defined(BUILD_RELEASE)
    static constexpr bool INITIAL_WINDOW_STATE = false;
#else
    static constexpr bool INITIAL_WINDOW_STATE = true;
#endif

    static constexpr const char* const CHIP8_SETTINGS_STRING = ICON_FA_GEAR " Chip8 Settings";
    static constexpr const char* const CHIP8_STATS_STRING = ICON_FA_CHARGING_STATION " Emulation Stats";

public:
    Chip8topiaEmulationUi() = default;
    Chip8topiaEmulationUi(const Chip8topiaEmulationUi&) = delete;
    Chip8topiaEmulationUi(Chip8topiaEmulationUi&&) = delete;
    auto operator=(const Chip8topiaEmulationUi&) -> Chip8topiaEmulationUi& = delete;
    auto operator=(Chip8topiaEmulationUi&&) -> Chip8topiaEmulationUi& = delete;
    ~Chip8topiaEmulationUi() = default;

public:
    void drawEmulationMenu(Chip8topia& chip8topia);
    void drawEmulationWindows(Chip8topia& chip8topia, bool isMainBarOpen);
    void closeAllWindows();

private:
    void drawEmulationSettings(Chip8topia* chip8topia);
    void drawEmulationStats(Chip8topia& chip8topia, bool isMainBarOpen);

private:
    Chip8topiaPerformanceMonitor m_performanceMonitor;
    Chip8CoreType m_selectedCore = Chip8Emulator::DEFAULT_CORE_TYPE;
    Chip8Frequency m_selectedFrequency = Chip8Emulator::DEFAULT_FREQUENCY;

    bool m_emulationSettingsOpen = INITIAL_WINDOW_STATE;
    bool m_emulationStatsOpen = INITIAL_WINDOW_STATE;
};

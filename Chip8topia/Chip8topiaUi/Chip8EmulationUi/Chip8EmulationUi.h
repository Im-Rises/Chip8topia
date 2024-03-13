#pragma once

#include <array>
#include <IconsFontAwesome6.h>

#include "../ImGuiHelper/ImGuiHelper.h"
#include "../../Chip8Emulator/Chip8Emulator.h"

class Chip8topia;
class Chip8EmulationUi {
public:
    Chip8EmulationUi() = default;
    Chip8EmulationUi(const Chip8EmulationUi&) = delete;
    Chip8EmulationUi(Chip8EmulationUi&&) = delete;
    auto operator=(const Chip8EmulationUi&) -> Chip8EmulationUi& = delete;
    auto operator=(Chip8EmulationUi&&) -> Chip8EmulationUi& = delete;
    ~Chip8EmulationUi() = default;

public:
    void drawEmulationMenu(Chip8topia& chip8topia);
    void drawEmulationWindows(Chip8topia& chip8topia);
    void closeAllWindows();

private:
    void drawEmulationStats(Chip8topia& chip8topia);
    void drawEmulationSettings(Chip8topia* chip8topia);

private:
#if defined(BUILD_RELEASE)
    static constexpr auto INITIAL_WINDOW_STATE = false;
#else
    static constexpr auto INITIAL_WINDOW_STATE = true;
#endif
    std::array<ImGuiMenuItemWindow<Chip8topia>, 2> m_menuItems = {
        ImGuiMenuItemWindow<Chip8topia>(ICON_FA_CHARGING_STATION " Emulation Stats", INITIAL_WINDOW_STATE, [this](Chip8topia* chip8topia) { drawEmulationStats(*chip8topia); }),
        ImGuiMenuItemWindow<Chip8topia>(ICON_FA_GEAR " Chip8 Settings", true, [this](Chip8topia* chip8topia) { drawEmulationSettings(chip8topia); })
    };

    Chip8CoreType m_selectedCore = Chip8Emulator::DEFAULT_CORE_TYPE;
    Chip8Frequency m_selectedFrequency = Chip8Emulator::DEFAULT_FREQUENCY;
};

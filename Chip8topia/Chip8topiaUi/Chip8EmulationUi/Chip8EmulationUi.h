#pragma once

#include <array>

#include "../ImGuiHelper/ImGuiHelper.h"
#include "../../Chip8Emulator/Chip8CoreBase/Chip8CoreBase.h"

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
    // TODO: Add new window which indicate emulation stats (fps, frame time, etc.)

private:
    void drawEmulationStats(Chip8topia& chip8topia);
    void drawEmulationSettings(Chip8topia* chip8topia);

private:
#if defined(BUILD_RELEASE)
    static constexpr auto INITIAL_WINDOW_STATE = true;
#else
    static constexpr auto INITIAL_WINDOW_STATE = false;
#endif
    std::array<ImGuiMenuItemWindow<Chip8topia>, 2> m_menuItems = {
        "Emulation Stats", false, [this](Chip8topia* chip8topia) { drawEmulationStats(*chip8topia); },
        "Chip8 Settings", INITIAL_WINDOW_STATE, [this](Chip8topia* chip8topia) { drawEmulationSettings(chip8topia); }
    };

    Chip8CoreType m_selectedCore = Chip8CoreType::Chip8;
    Chip8Frequency m_selectedFrequency = Chip8Frequency::FREQ_1200_HZ;
};

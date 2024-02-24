#pragma once

#include "../ImGuiHelper/ImGuiHelper.h"

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

private:
    void drawEmulationSettings();

private:
    ImGuiMenuItemPopup<> m_Chip8SettingsMenuItem = { "Chip8 Settings", false, [this]() { drawEmulationSettings(); } };
};

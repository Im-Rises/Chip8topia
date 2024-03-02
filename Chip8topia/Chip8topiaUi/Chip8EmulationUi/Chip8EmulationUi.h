#pragma once

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

    // TODO: Add new window which indicate emulation stats (fps, frame time, etc.)

private:
    void drawEmulationSettings(Chip8topia* chip8topia);

private:
    ImGuiMenuItemWindow<Chip8topia> m_Chip8SettingsMenuItem = { "Chip8 Settings", false, [this](Chip8topia* chip8topia) { drawEmulationSettings(chip8topia); } };

    Chip8CoreType m_selectedCore = Chip8CoreType::Chip8;
    Chip8Frequency m_selectedFrequency = Chip8Frequency::FREQ_1200_HZ;
};

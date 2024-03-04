#pragma once

#include <array>

#include "../ImGuiHelper/ImGuiHelper.h"

class Chip8Emulator;
class Chip8VideoUi {
public:
    Chip8VideoUi() = default;
    Chip8VideoUi(const Chip8VideoUi&) = delete;
    Chip8VideoUi(Chip8VideoUi&&) = delete;
    auto operator=(const Chip8VideoUi&) -> Chip8VideoUi& = delete;
    auto operator=(Chip8VideoUi&&) -> Chip8VideoUi& = delete;
    ~Chip8VideoUi() = default;

public:
    void drawVideoMenu();
    void drawVideoWindows(Chip8Emulator& emulator);
    void closeAllWindows();

private:
    void drawBackgroundColor(Chip8Emulator* emulator);
    void drawDrawColor(Chip8Emulator* emulator);

private:
    std::array<ImGuiMenuItemWindow<Chip8Emulator>, 2> m_menuItem = {
        ImGuiMenuItemWindow<Chip8Emulator>("Background color", false, [this](Chip8Emulator* chip8Emulator) { drawBackgroundColor(chip8Emulator); }),
        ImGuiMenuItemWindow<Chip8Emulator>("Draw color", false, [this](Chip8Emulator* chip8Emulator) { drawDrawColor(chip8Emulator); })
    };
};

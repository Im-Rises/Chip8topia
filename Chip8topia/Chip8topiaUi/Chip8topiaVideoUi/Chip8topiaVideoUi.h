#pragma once

#include <array>
#include <IconsFontAwesome6.h>

#include "../ImGuiHelper/ImGuiHelper.h"

class Chip8Emulator;
class Chip8topiaVideoUi
{
public:
    Chip8topiaVideoUi() = default;
    Chip8topiaVideoUi(const Chip8topiaVideoUi&) = delete;
    Chip8topiaVideoUi(Chip8topiaVideoUi&&) = delete;
    auto operator=(const Chip8topiaVideoUi&) -> Chip8topiaVideoUi& = delete;
    auto operator=(Chip8topiaVideoUi&&) -> Chip8topiaVideoUi& = delete;
    ~Chip8topiaVideoUi() = default;

public:
    void drawVideoMenu();
    void drawVideoWindows(Chip8Emulator& emulator);
    void closeAllWindows();

private:
    void drawBackgroundColor(Chip8Emulator* emulator);
    void drawMainPlaneColor(Chip8Emulator* emulator);
    void drawSecondaryPlaneColor(Chip8Emulator* emulator);
    void drawPixelsCommonColor(Chip8Emulator* emulator);

private:
    std::array<ImGuiMenuItemWindow<Chip8Emulator>, 4> m_menuItem = {
        ImGuiMenuItemWindow<Chip8Emulator>(ICON_FA_PALETTE " Background color", false, [this](Chip8Emulator* chip8Emulator)
            { drawBackgroundColor(chip8Emulator); }),
        ImGuiMenuItemWindow<Chip8Emulator>(ICON_FA_PALETTE " Draw main plane color", false, [this](Chip8Emulator* chip8Emulator)
            { drawMainPlaneColor(chip8Emulator); }),
        ImGuiMenuItemWindow<Chip8Emulator>(ICON_FA_PALETTE " Draw secondary plane color", false, [this](Chip8Emulator* chip8Emulator)
            { drawSecondaryPlaneColor(chip8Emulator); }),
        ImGuiMenuItemWindow<Chip8Emulator>(ICON_FA_PALETTE " Draw plane pixels common color", false, [this](Chip8Emulator* chip8Emulator)
            { drawPixelsCommonColor(chip8Emulator); })
    };
};

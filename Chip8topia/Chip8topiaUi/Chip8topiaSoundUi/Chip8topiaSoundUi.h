#pragma once

#include <array>
#include <IconsFontAwesome6.h>

#include "../ImGuiHelper/ImGuiHelper.h"

class Chip8Emulator;
class Chip8topiaSoundUi
{
private:
#if defined(BUILD_RELEASE)
    static constexpr bool INITIAL_WINDOW_STATE = false;
#else
    static constexpr bool INITIAL_WINDOW_STATE = true;
#endif

public:
    Chip8topiaSoundUi() = default;
    Chip8topiaSoundUi(const Chip8topiaSoundUi&) = delete;
    Chip8topiaSoundUi(Chip8topiaSoundUi&&) = delete;
    auto operator=(const Chip8topiaSoundUi&) -> Chip8topiaSoundUi& = delete;
    auto operator=(Chip8topiaSoundUi&&) -> Chip8topiaSoundUi& = delete;
    ~Chip8topiaSoundUi() = default;

public:
    void drawSoundMenu();
    void drawSoundWindows(Chip8Emulator& emulator);

private:
    // Create a window to show ST anf if its playing or not
    // Another window or just a tool in the menu item to change volume
    std::array<ImGuiMenuItemWindow<Chip8Emulator>, 0> m_menuItem = {
        //        ImGuiMenuItemWindow<Chip8Emulator>(ICON_FA_PALETTE " Planes color", INITIAL_WINDOW_STATE, [this](Chip8Emulator* chip8Emulator)
        //            { drawPlanesColorEditor(*chip8Emulator); }),
        //        ImGuiMenuItemWindow<Chip8Emulator>(ICON_FA_WINDOW_RESTORE " Planes", INITIAL_WINDOW_STATE, [this](Chip8Emulator* chip8Emulator)
        //            { drawPlanes(*chip8Emulator); }),
    };
};

#pragma once

#include <array>
#include <IconsFontAwesome6.h>

#include "../ImGuiHelper/ImGuiHelper.h"

class Chip8Emulator;
class Chip8topiaVideoUi
{
private:
#if defined(BUILD_RELEASE)
    static constexpr bool INITIAL_WINDOW_STATE = false;
#else
    static constexpr bool INITIAL_WINDOW_STATE = true;
#endif

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
    void drawPlanesColorEditor(Chip8Emulator& emulator);
    void drawPlanes(Chip8Emulator& emulator);

private:
    std::array<ImGuiMenuItemWindow<Chip8Emulator>, 2> m_menuItem = {
        ImGuiMenuItemWindow<Chip8Emulator>(ICON_FA_PALETTE " Planes color", INITIAL_WINDOW_STATE, [this](Chip8Emulator* chip8Emulator)
            { drawPlanesColorEditor(*chip8Emulator); }),
        ImGuiMenuItemWindow<Chip8Emulator>(ICON_FA_WINDOW_RESTORE " Planes", INITIAL_WINDOW_STATE, [this](Chip8Emulator* chip8Emulator)
            { drawPlanes(*chip8Emulator); }),
    };
};

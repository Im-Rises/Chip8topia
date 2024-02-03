#pragma once

#include <functional>
#include <string_view>

#include "Chip8topiaDebugger/Chip8topiaDebugger.h"

class Chip8topia;
class Chip8topiaUi {
private:
    static constexpr auto CHIP8_ROM_FILE_EXTENSION = ".ch8";
    static constexpr auto FILE_DIALOG_NAME = "RomFileWindowDialog";

public:
    Chip8topiaUi();
    Chip8topiaUi(const Chip8topiaUi&) = delete;
    Chip8topiaUi(Chip8topiaUi&&) = delete;
    auto operator=(const Chip8topiaUi&) -> Chip8topiaUi& = delete;
    auto operator=(Chip8topiaUi&&) -> Chip8topiaUi& = delete;
    ~Chip8topiaUi();

public:
    void drawUi(Chip8topia& chip8topia);

private:
    void drawMainMenuBar(Chip8topia& chip8topia);

    void drawFileMenu(Chip8topia& chip8topia);
    void drawEngineEmulationMenu(Chip8topia& chip8topia);
    void drawViewMenu(Chip8topia& chip8topia);
    void drawVideoMenu();
    void drawAboutMenu();

    void drawVideoWindow(Chip8topia& chip8topia);

    void drawAboutChip8topiaPopUpWindow();
    void drawAboutChip8PopUpWindow();
    void drawAboutPopUpInternal(const std::string_view& popupName, const std::function<void()>& drawAboutPopUpContent, bool& showAboutPopup);

    void openRomWindow();
    void drawRomWindow(Chip8topia& chip8topia);

    void toggleMenuBar();

private:
    Chip8topiaDebugger m_chip8topiaDebugger;

    bool m_isMenuBarOpen = true;
    bool m_showAboutChip8topiaPopup = false;
    bool m_showAboutChip8Popup = false;

    bool m_showBackgroundColor = false;
    bool m_showForegroundColor = false;
};

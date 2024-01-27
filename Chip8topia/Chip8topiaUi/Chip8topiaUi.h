#pragma once

#include <functional>
#include <string_view>

#include "Chip8topiaDebugger/Chip8topiaDebugger.h"

class Chip8topia;
class Chip8topiaUi {
private:
    static constexpr auto CHIP8_ROM_FILE_EXTENSION = ".ch8";
//    static constexpr auto FILE_DIALOG_KEY = "ChooseFileDlgKey";
#define FILE_DIALOG_KEY "ChooseFileDlgKey"

public:
    Chip8topiaUi();
    Chip8topiaUi(const Chip8topiaUi&) = delete;
    Chip8topiaUi(Chip8topiaUi&&) = delete;
    auto operator=(const Chip8topiaUi&) -> Chip8topiaUi& = delete;
    auto operator=(Chip8topiaUi&&) -> Chip8topiaUi& = delete;
    ~Chip8topiaUi();

public:
    void init(Chip8topia* chip8topia); // TODO: Maybe remove this init and pass the Chip8topia pointer in the constructor orpass directly the pointer/reference to the Chip8topia object in the drawUi() function
    void drawUi();

private:
    void drawMainMenuBar();

    void drawFileMenu();
    void drawViewMenu();
    void drawDesignMenu();
    void drawAboutMenu();

    void drawAboutPopUpWindow();
    void drawAboutPopUpInternal(const std::string_view& popupName, const std::function<void()>& drawAboutPopUpContent);

    // TODO: Correct open dialog string name not working
    void drawRomWindow();

    void openRomWindow();
    void toggleMenuBar();

private:
    Chip8topiaDebugger m_chip8topiaDebugger;
    Chip8topia* m_chip8topia = nullptr;

    bool m_isMenuBarOpen = true;
    bool m_showAboutPopup = false;
};

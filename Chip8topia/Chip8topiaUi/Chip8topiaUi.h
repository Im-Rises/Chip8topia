#pragma once

#include "Chip8topiaDebugger/Chip8topiaDebugger.h"

class Chip8topiaUi {
public:
    Chip8topiaUi() = default;
    Chip8topiaUi(const Chip8topiaUi&) = delete;
    Chip8topiaUi(Chip8topiaUi&&) = delete;
    auto operator=(const Chip8topiaUi&) -> Chip8topiaUi& = delete;
    auto operator=(Chip8topiaUi&&) -> Chip8topiaUi& = delete;
    ~Chip8topiaUi() = default;

public:
    void drawUi();

private:
    void drawMainMenuBar();

    void drawFileMenu();
    void drawViewMenu();
    void drawDesignMenu(); // Change background color and draw color
    //    void drawToolsMenu();
    void drawAboutMenu();

    void drawAboutPopUpWindow();

private:
    bool m_showAboutPopup = false;
    Chip8topiaDebugger m_chip8topiaDebugger;
};

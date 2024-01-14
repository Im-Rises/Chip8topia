#pragma once

class Chip8topiaUi {
public:
    void drawUi();

private:
    void drawMainMenuBar();

    void drawFileMenu();
    void drawViewMenu();
    void drawDesignMenu(); // Change background color and draw color
    void drawToolsMenu();
    void drawAboutMenu();

    void drawAboutPopUpWindow();

private:
    bool m_showAboutPopup = false;
};

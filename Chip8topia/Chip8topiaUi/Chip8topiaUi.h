#pragma once

#include <functional>
#include <string_view>

#include "Chip8topiaDebugger/Chip8topiaDebugger.h"
#include "Chip8VideoUi/Chip8VideoUi.h"
#include "Chip8About/Chip8About.h"
#include "Chip8RomLoaderUi/Chip8RomLoaderUi.h"
#include "Chip8EmulationUi/Chip8EmulationUi.h"
#include "Chip8MessageHandler/Chip8MessageHandler.h"

class Chip8topia;
class Chip8topiaUi
{
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
    void drawViewMenu(Chip8topia& chip8topia);

    void toggleMenuBarVisibility();

    void closeAllWindows();

private:
    Chip8RomLoaderUi m_chip8RomLoaderUi;
    Chip8EmulationUi m_chip8EmulationUi;
    Chip8VideoUi m_chip8VideoUi;
    Chip8topiaDebugger m_chip8topiaDebugger;
    Chip8About m_chip8About;
    Chip8MessageHandler m_chip8Error;

    bool m_isMenuBarOpen = true;
};

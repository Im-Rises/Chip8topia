#pragma once

#include <functional>
#include <string_view>

#include "Chip8topiaDebugger/Chip8topiaDebugger.h"
#include "Chip8topiaVideoUi/Chip8topiaVideoUi.h"
#include "Chip8topiaAbout/Chip8topiaAbout.h"
#include "Chip8topiaRomLoaderUi/Chip8topiaRomLoaderUi.h"
#include "Chip8topiaEmulationUi/Chip8topiaEmulationUi.h"
#include "Chip8topiaMessageHandler/Chip8topiaMessageHandler.h"

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
    Chip8topiaRomLoaderUi m_chip8topiaRomLoaderUi;
    Chip8topiaEmulationUi m_chip8topiaEmulationUi;
    Chip8topiaVideoUi m_chip8topiaVideoUi;
    Chip8topiaDebugger m_chip8topiatopiaDebugger;
    Chip8topiaAbout m_chip8topiaAbout;
    Chip8topiaMessageHandler m_chip8topiaError;

    bool m_isMenuBarOpen = true;

#if defined(BUILD_DEBUG)
    bool m_showDemo = true;
#endif
};

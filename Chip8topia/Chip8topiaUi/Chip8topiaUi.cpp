#include "Chip8topiaUi.h"

#include <imgui.h>

#include "../Chip8topia.h"

Chip8topiaUi::Chip8topiaUi() {
#ifndef __EMSCRIPTEN__
    Chip8topiaInputHandler::getInstance().m_ToggleMainBarEvent.subscribe(this, &Chip8topiaUi::toggleMenuBarVisibility);
#endif
    Chip8topiaInputHandler::getInstance().m_CloseAllWindowsEvent.subscribe(this, &Chip8topiaUi::closeAllWindows);
}

Chip8topiaUi::~Chip8topiaUi() {
#ifndef __EMSCRIPTEN__
    Chip8topiaInputHandler::getInstance().m_ToggleMainBarEvent.unsubscribe(this, &Chip8topiaUi::toggleMenuBarVisibility);
#endif
    Chip8topiaInputHandler::getInstance().m_CloseAllWindowsEvent.unsubscribe(this, &Chip8topiaUi::closeAllWindows);
}

void Chip8topiaUi::drawUi(Chip8topia& chip8topia) {
    drawMainMenuBar(chip8topia);
}

void Chip8topiaUi::drawMainMenuBar(Chip8topia& chip8topia) {
    if (m_isMenuBarOpen && ImGui::BeginMainMenuBar())
    {
        m_chip8RomLoaderUi.drawFileMenu(chip8topia);
        drawViewMenu(chip8topia);
        m_chip8EmulationUi.drawEmulationMenu(chip8topia);
        m_chip8VideoUi.drawVideoMenu();
        m_chip8topiaDebugger.drawDebuggerMenu();
        m_chip8About.drawAboutMenu();

        ImGui::EndMainMenuBar();
    }

    m_chip8VideoUi.drawVideoWindows(chip8topia.getChip8Emulator());
    m_chip8topiaDebugger.drawDebuggerWindows(chip8topia.getChip8Emulator());

    m_chip8RomLoaderUi.drawRomWindow(chip8topia);
    m_chip8EmulationUi.drawEmulationWindows(chip8topia);
    m_chip8About.drawAboutWindows();

#if !defined(BUILD_RELEASE)
    ImGui::ShowDemoWindow();
#endif
}

void Chip8topiaUi::drawViewMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("View"))
    {
        ImGui::MenuItem("Show/Hide MenuBar", "U", &m_isMenuBarOpen);

        if (ImGui::MenuItem("Close all Windows", "I"))
        {
            closeAllWindows();
        }

#ifndef __EMSCRIPTEN__
        if (ImGui::MenuItem("Center window", "F10"))
        {
            chip8topia.centerWindow();
        }

        if (ImGui::MenuItem("FullScreen", "F11"))
        {
            chip8topia.toggleFullScreen();
        }
#endif

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::toggleMenuBarVisibility() {
    m_isMenuBarOpen = !m_isMenuBarOpen;
}

void Chip8topiaUi::closeAllWindows() {
    m_chip8VideoUi.closeAllWindows();
    m_chip8topiaDebugger.closeAllWindows();
    m_chip8RomLoaderUi.closeAllWindows();
    m_chip8EmulationUi.closeAllWindows();
    //    m_chip8About.closeAboutWindows();
}

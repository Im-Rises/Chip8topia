#include "Chip8topiaUi.h"

#include <imgui.h>
#include <IconsFontAwesome6.h>

#include "../Chip8topia.h"

Chip8topiaUi::Chip8topiaUi()
{
    Chip8topiaInputHandler::getInstance().m_ToggleMainBarEvent.subscribe(this, &Chip8topiaUi::toggleMenuBarVisibility);
    Chip8topiaInputHandler::getInstance().m_CloseAllWindowsEvent.subscribe(this, &Chip8topiaUi::closeAllWindows);
}

Chip8topiaUi::~Chip8topiaUi()
{
    Chip8topiaInputHandler::getInstance().m_ToggleMainBarEvent.unsubscribe(this, &Chip8topiaUi::toggleMenuBarVisibility);
    Chip8topiaInputHandler::getInstance().m_CloseAllWindowsEvent.unsubscribe(this, &Chip8topiaUi::closeAllWindows);
}

void Chip8topiaUi::drawUi(Chip8topia& chip8topia)
{
    drawMainMenuBar(chip8topia);
}

void Chip8topiaUi::drawMainMenuBar(Chip8topia& chip8topia)
{
    if (m_isMenuBarOpen && ImGui::BeginMainMenuBar())
    {
        m_chip8topiaRomLoaderUi.drawFileMenu(chip8topia);
        drawViewMenu(chip8topia);
        m_chip8topiaEmulationUi.drawEmulationMenu(chip8topia);
        m_chip8topiaVideoUi.drawVideoMenu();
        m_chip8topiaSoundUi.drawSoundMenu(chip8topia.getChip8Emulator());
        m_chip8topiatopiaDebugger.drawDebuggerMenu();
        m_chip8topiaAbout.drawAboutMenu();
#if !defined(BUILD_RELEASE)
        if (ImGui::MenuItem(ICON_FA_INDUSTRY " ImGui Demo"))
        {
            m_showDemo = !m_showDemo;
        }

        if (ImGui::MenuItem(ICON_FA_BUG " ImGui Metrics"))
        {
            m_showImGuiMetrics = !m_showImGuiMetrics;
        }
#endif
        ImGui::Indent(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 7);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

        ImGui::EndMainMenuBar();
    }

    m_chip8topiaVideoUi.drawVideoWindows(chip8topia.getChip8Emulator());
    m_chip8topiaSoundUi.drawSoundWindows(chip8topia.getChip8Emulator());
    m_chip8topiatopiaDebugger.drawDebuggerWindows(chip8topia.getChip8Emulator());
    m_chip8topiaRomLoaderUi.drawRomWindow(chip8topia);
    m_chip8topiaEmulationUi.drawEmulationWindows(chip8topia, m_isMenuBarOpen);
    m_chip8topiaAbout.drawAboutWindows();
    m_chip8topiaError.showMessages();

#if defined(BUILD_DEBUG)
    if (m_showDemo)
    {
        ImGui::ShowDemoWindow(&m_showDemo);
    }

    if (m_showImGuiMetrics)
    {
        ImGui::ShowMetricsWindow(&m_showImGuiMetrics);
    }
#endif
}

void Chip8topiaUi::drawViewMenu(Chip8topia& /* chip8topia */)
{
    if (ImGui::BeginMenu(ICON_FA_EYE " View"))
    {
        ImGui::MenuItem(ICON_FA_WINDOW_MINIMIZE " Show/Hide MenuBar", "U", &m_isMenuBarOpen);

        if (ImGui::MenuItem(ICON_FA_XMARK " Close all windows", "I"))
        {
            closeAllWindows();
        }

#ifndef __EMSCRIPTEN__
        if (ImGui::MenuItem(ICON_FA_WINDOW_RESTORE " Center window", "F10"))
        {
            Chip8topiaInputHandler::getInstance().m_CenterWindowEvent.trigger();
        }

        if (ImGui::MenuItem(ICON_FA_WINDOW_MAXIMIZE " Fullscreen", "F11"))
        {
            Chip8topiaInputHandler::getInstance().m_ToggleFullScreenEvent.trigger();
        }
#endif

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::toggleMenuBarVisibility()
{
    m_isMenuBarOpen = !m_isMenuBarOpen;
}

void Chip8topiaUi::closeAllWindows()
{
    m_chip8topiaVideoUi.closeAllWindows();
    m_chip8topiatopiaDebugger.closeAllWindows();
    m_chip8topiaRomLoaderUi.closeAllWindows();
    m_chip8topiaEmulationUi.closeAllWindows();
    m_chip8topiaSoundUi.closeAllWindows();

#if defined(BUILD_DEBUG)
    m_showDemo = false;
#endif
}

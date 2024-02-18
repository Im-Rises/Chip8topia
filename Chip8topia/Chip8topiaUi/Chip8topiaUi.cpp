#include "Chip8topiaUi.h"

#include <imgui.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>
// #include <format>
#include <fmt/format.h>
#include <plateformIdentifier/plateformIdentifier.h>

#include "../Chip8topia.h"

Chip8topiaUi::Chip8topiaUi() {
    Chip8topiaInputHandler::getInstance().m_F1KeyButtonPressedEvent.subscribe(this, &Chip8topiaUi::toggleMenuBarVisibility);
    Chip8topiaInputHandler::getInstance().m_F2KeyButtonPressedEvent.subscribe(this, &Chip8topiaUi::toggleWindowsVisibility);
    Chip8topiaInputHandler::getInstance().m_CTRL_OKeyButtonPressedEvent.subscribe(this, &Chip8topiaUi::openRomWindow);
}

Chip8topiaUi::~Chip8topiaUi() {
    Chip8topiaInputHandler::getInstance().m_F1KeyButtonPressedEvent.unsubscribe(this, &Chip8topiaUi::toggleMenuBarVisibility);
    Chip8topiaInputHandler::getInstance().m_F2KeyButtonPressedEvent.unsubscribe(this, &Chip8topiaUi::toggleWindowsVisibility);
    Chip8topiaInputHandler::getInstance().m_CTRL_OKeyButtonPressedEvent.unsubscribe(this, &Chip8topiaUi::openRomWindow);
}

void Chip8topiaUi::drawUi(Chip8topia& chip8topia) {
    drawMainMenuBar(chip8topia);
}

void Chip8topiaUi::drawMainMenuBar(Chip8topia& chip8topia) {
    if (m_isMenuBarOpen && ImGui::BeginMainMenuBar())
    {
        drawFileMenu(chip8topia);
        drawViewMenu(chip8topia);
        drawEngineEmulationMenu(chip8topia);
        drawVideoMenu();
        m_chip8topiaDebugger.drawDebuggerMenu();
        drawAboutMenu();

        ImGui::EndMainMenuBar();
    }

    if (m_windowsVisible)
    {
        m_chip8topiaDebugger.drawDebuggerWindows(chip8topia.getChip8Emulator());
        drawVideoWindow(chip8topia);
    }

    drawRomWindow(chip8topia);
    drawAboutChip8topiaPopUpWindow();
    drawAboutChip8PopUpWindow();

#if !defined(BUILD_RELEASE)
    ImGui::ShowDemoWindow();
#endif
}

void Chip8topiaUi::drawFileMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open rom..", "Ctrl+O"))
        {
            openRomWindow();
        }

#ifndef __EMSCRIPTEN__
        if (ImGui::MenuItem("Exit", "Alt+F4"))
        {
            chip8topia.closeRequest();
        }
#endif

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::drawEngineEmulationMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("Engine/Emulation"))
    {
        if (ImGui::MenuItem(fmt::format("Toggle Engine turbo mode : {}", chip8topia.getIsTurboMode() ? "ON " : "OFF").c_str(), "F3"))
        {
            chip8topia.toggleTurboMode();
        }

        if (ImGui::MenuItem(chip8topia.getChip8Emulator().getIsPaused() ? "Resume" : "Pause", "F"))
        {
            chip8topia.getChip8Emulator().togglePause();
        }

        // TODO: Move this code in the Debugger with the disassembly or put it here too

        if (ImGui::MenuItem("Resume"))
        {
        }

        if (ImGui::MenuItem("Break"))
        {
        }

        if (ImGui::MenuItem("Step"))
        {
        }

        if (ImGui::MenuItem("Restart"))
        {
            chip8topia.getChip8Emulator().restart();
        }

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::drawViewMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("View"))
    {
        // TODO: Create a dictonary of the keys and the name of the menu item with the event, to be sure to call the right event
        if (ImGui::MenuItem("Show/Hide MenuBar", "F1", &m_isMenuBarOpen))
        {
        }

        if (ImGui::MenuItem("Show/Hide Windows", "F2", &m_windowsVisible))
        {
        }

        if (ImGui::MenuItem("Center window", "F10"))
        {
            chip8topia.centerWindow();
        }

        if (ImGui::MenuItem("FullScreen", "F11"))
        {
            chip8topia.toggleFullScreen();
        }

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::drawVideoMenu() {
    if (ImGui::BeginMenu("Video"))
    {
        if (ImGui::MenuItem("Background color"))
        {
            m_showBackgroundColor = !m_showBackgroundColor;
        }
        if (ImGui::MenuItem("Draw color"))
        {
            m_showForegroundColor = !m_showForegroundColor;
        }
        ImGui::EndMenu();
    }
}

void Chip8topiaUi::drawAboutMenu() {
    if (ImGui::BeginMenu("About..."))
    {
        if (ImGui::MenuItem(Chip8topia::PROJECT_NAME))
        {
            m_showAboutChip8topiaPopup = true;
        }

        if (ImGui::MenuItem(Chip8topia::PROJECT_EMULATION_CONSOLE_NAME))
        {
            m_showAboutChip8Popup = true;
        }

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::drawVideoWindow(Chip8topia& chip8topia) {
    if (m_showBackgroundColor)
    {
        ImGui::Begin("Background color", &m_showBackgroundColor);
        ImGui::ColorPicker4("Background color", reinterpret_cast<float*>(&chip8topia.getChip8Emulator().getChip8VideoEmulation().getBackgroundColor()));
        ImGui::End();
    }
    
    if (m_showForegroundColor)
    {
        ImGui::Begin("Draw color", &m_showForegroundColor);
        ImGui::ColorPicker4("Draw color", reinterpret_cast<float*>(&chip8topia.getChip8Emulator().getChip8VideoEmulation().getForegroundColor()));
        ImGui::End();
    }
}

void Chip8topiaUi::drawAboutChip8topiaPopUpWindow() {
    static constexpr auto ABOUT_CHIP8TOPIA = []() {
        ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), Chip8topia::PROJECT_NAME);
        ImGui::Text("Version: %s\n" // TODO: Use R"("")"; string type
                    "\n"
                    "Developed by:\n "
                    "- %s\n"
                    "\n"
                    "Github:\n"
                    " - %s",
            Chip8topia::PROJECT_VERSION,
            Chip8topia::PROJECT_AUTHOR,
            Chip8topia::PROJECT_LINK);
    };
    drawAboutPopUpInternal(Chip8topia::PROJECT_NAME, ABOUT_CHIP8TOPIA, m_showAboutChip8topiaPopup);
}

void Chip8topiaUi::drawAboutChip8PopUpWindow() {
    static constexpr auto ABOUT_CHIP8 = []() {
        ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), Chip8topia::PROJECT_EMULATION_CONSOLE_NAME);
        ImGui::Text(Chip8topia::PROJECT_EMULATION_CONSOLE_DESCRIPTION);
    };
    drawAboutPopUpInternal(Chip8topia::PROJECT_EMULATION_CONSOLE_NAME, ABOUT_CHIP8, m_showAboutChip8Popup);
}

void Chip8topiaUi::drawAboutPopUpInternal(const std::string_view& popupName, const std::function<void()>& drawAboutPopUpContent, bool& showAboutPopup) {
    if (showAboutPopup)
    {
        ImGui::OpenPopup(popupName.data());
    }
    if (ImGui::BeginPopupModal(popupName.data(), &showAboutPopup,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        drawAboutPopUpContent();
        ImGui::EndPopup();
    }
}

void Chip8topiaUi::openRomWindow() {
    IGFD::FileDialogConfig config;
    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog(FILE_DIALOG_NAME, "Select a game rom", CHIP8_ROM_FILE_EXTENSION, config);
}

void Chip8topiaUi::drawRomWindow(Chip8topia& chip8topia) {
    if (ImGuiFileDialog::Instance()->Display(FILE_DIALOG_NAME))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            const std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            chip8topia.getChip8Emulator().loadRom(filePathName);
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void Chip8topiaUi::toggleMenuBarVisibility() {
    m_isMenuBarOpen = !m_isMenuBarOpen;
}

void Chip8topiaUi::toggleWindowsVisibility() {
    m_windowsVisible = !m_windowsVisible;
}

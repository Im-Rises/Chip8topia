#include "Chip8topiaUi.h"

#include <imgui/imgui.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>
#include <iostream>

#include "../Chip8topia.h"

Chip8topiaUi::~Chip8topiaUi() {
    Chip8topiaInputHandler::getInstance().m_F12KeyButtonPressedEvent.unsubscribe(this, &Chip8topiaUi::toggleMenuBar);
    Chip8topiaInputHandler::getInstance().m_CTRL_OKeyButtonPressedEvent.unsubscribe(this, &Chip8topiaUi::openRomWindow);
}

Chip8topiaUi::Chip8topiaUi() {
    Chip8topiaInputHandler::getInstance().m_F12KeyButtonPressedEvent.subscribe(this, &Chip8topiaUi::toggleMenuBar);
    Chip8topiaInputHandler::getInstance().m_CTRL_OKeyButtonPressedEvent.subscribe(this, &Chip8topiaUi::openRomWindow);
}

void Chip8topiaUi::drawUi(Chip8topia& chip8topia) {
    drawMainMenuBar(chip8topia);
}

void Chip8topiaUi::drawMainMenuBar(Chip8topia& chip8topia) {
    if (m_isMenuBarOpen && ImGui::BeginMainMenuBar())
    {
        drawFileMenu(chip8topia);
        drawViewMenu(chip8topia);
        drawDesignMenu();
        m_chip8topiaDebugger.drawDebugger(chip8topia.getChip8Emulator());
        drawAboutMenu();

        ImGui::EndMainMenuBar();
    }

    drawAboutChip8topiaPopUpWindow();
    drawAboutChip8PopUpWindow();
    drawRomWindow(chip8topia);
}

void Chip8topiaUi::drawFileMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open rom..", "Ctrl+O"))
        {
            openRomWindow();
        }
        if (ImGui::MenuItem("Exit", "Alt+F4"))
        {
            chip8topia.close();
        }

        ImGui::EndMenu();
    }
}
void Chip8topiaUi::drawViewMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("View"))
    {
        if (ImGui::MenuItem("FullScreen", "F11"))
        {
            chip8topia.toggleFullScreen();
        }
        if (ImGui::MenuItem("Show/Hide MenuBar", "F12"))
        {
            toggleMenuBar();
        }

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::drawDesignMenu() {
    if (ImGui::BeginMenu("Design"))
    {
        if (ImGui::MenuItem("Background color"))
        {
            //                m_chip8.setDebuggerMode(true);
            // set a tooltip to show the window of each debugger tool
        }
        if (ImGui::MenuItem("Draw color"))
        {
            //                m_chip8.setDebuggerMode(true);
            // set a tooltip to show the window of each debugger tool
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

void Chip8topiaUi::drawAboutChip8topiaPopUpWindow() {
    static constexpr auto ABOUT_CHIP8TOPIA = []() {
        ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), Chip8topia::PROJECT_NAME);
        ImGui::Text("Version: %s\n"
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
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
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
            chip8topia.getChip8Emulator().readRom(filePathName); // TODO: use std::string_view or std::filesystem::path or std::string
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void Chip8topiaUi::toggleMenuBar() {
    m_isMenuBarOpen = !m_isMenuBarOpen;
}

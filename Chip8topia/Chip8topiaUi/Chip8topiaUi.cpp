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

void Chip8topiaUi::init(Chip8topia* chip8topia) {
    m_chip8topia = chip8topia;
}

void Chip8topiaUi::drawUi() {
    drawMainMenuBar();
}

void Chip8topiaUi::drawMainMenuBar() {
    if (m_isMenuBarOpen && ImGui::BeginMainMenuBar())
    {
        drawFileMenu();
        drawViewMenu();
        drawDesignMenu();
        m_chip8topiaDebugger.drawDebugger();
        drawAboutMenu();

        ImGui::EndMainMenuBar();
    }

    drawAboutChip8topiaPopUpWindow();
    drawAboutChip8PopUpWindow();
    drawRomWindow();
}

void Chip8topiaUi::drawFileMenu() {
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open rom..", "Ctrl+O"))
        {
            openRomWindow();
        }
        if (ImGui::MenuItem("Exit", "Alt+F4"))
        {
            m_chip8topia->close();
        }

        ImGui::EndMenu();
    }
}
void Chip8topiaUi::drawViewMenu() {
    if (ImGui::BeginMenu("View"))
    {
        if (ImGui::MenuItem("FullScreen", "F11"))
        {
            m_chip8topia->toggleFullScreen();
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

void Chip8topiaUi::drawRomWindow() {
    if (ImGuiFileDialog::Instance()->Display(FILE_DIALOG_NAME))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::cout << "Selected file: " << ImGuiFileDialog::Instance()->GetCurrentPath() << std::endl;
            const std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            //            m_chip8topia->loadRom(filePathName);
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void Chip8topiaUi::toggleMenuBar() {
    m_isMenuBarOpen = !m_isMenuBarOpen;
}

#include "Chip8topiaUi.h"

#include <imgui/imgui.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

#include "../Chip8topia.h"

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
        //        drawToolsMenu();
        m_chip8topiaDebugger.drawDebuggerMenu();
        m_chip8topiaDebugger.drawDebuggerWindows();
        drawAboutMenu();

        ImGui::EndMainMenuBar();
    }

    drawAboutPopUpWindow();
}

void Chip8topiaUi::drawFileMenu() {
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open rom..", "Ctrl+O"))
        {
            // Reset emulation, open file explorer and search path to file to rom
            // Get rom path and read it.

            IGFD::FileDialogConfig config;
            config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", config);
        }
        if (ImGui::MenuItem("Exit", "Alt+F4"))
        {
            m_chip8topia->close();
        }

        ImGui::EndMenu();
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            const std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            const std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
        }

        // close
        ImGuiFileDialog::Instance()->Close();
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
            m_isMenuBarOpen = !m_isMenuBarOpen;
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
        //        if (ImGui::MenuItem("Dependencies"))
        //        {
        //            ImGui::OpenPopup("Dependencies");
        //        }

        if (ImGui::MenuItem(Chip8topia::PROJECT_NAME))
        {
            m_showAboutPopup = true;
        }

        //        if (ImGui::MenuItem(Chip8topia::PROJECT_EMULATION_CONSOLE_NAME))
        //        {
        //            ImGui::OpenPopup(Chip8topia::PROJECT_EMULATION_CONSOLE_NAME);
        //        }

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::drawAboutPopUpWindow() {
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
    drawAboutPopUpInternal(Chip8topia::PROJECT_NAME, ABOUT_CHIP8TOPIA);

    //    static constexpr auto ABOUT_CHIP8 = []() {
    //        ImGui::Text(Chip8topia::PROJECT_EMULATION_CONSOLE_DESCRIPTION);
    //    };
    //    drawAboutPopUpInternal(Chip8topia::PROJECT_EMULATION_CONSOLE_NAME, ABOUT_CHIP8);

    //    static constexpr auto aboutProjectDependencies = []() {
    //        ImGui::Text("The Chip8topia project uses the following dependencies:");
    //        ImGui::Text(" - OpenGL Vendor: %s", Chip8topia::getOpenGLVendor().data());
    //        ImGui::Text(" - OpenGL Version: %s", Chip8topia::getOpenGLVersion().data());
    //        ImGui::Text(" - GLFW Version: %s", Chip8topia::getGLFWVersion().data());
    //        ImGui::Text(" - Glad Version: %s", Chip8topia::getGladVersion().data());
    //        ImGui::Text(" - ImGui Version: %s", Chip8topia::getImGuiVersion().data());
    //        ImGui::NewLine();
    //    };
    //    drawAboutPopUpInternal("Dependencies", aboutProjectDependencies);
}
void Chip8topiaUi::drawAboutPopUpInternal(const std::string_view& popupName, const std::function<void()>& drawAboutPopUpContent) {
    if (m_showAboutPopup)
    {
        ImGui::OpenPopup(popupName.data());
    }
    if (ImGui::BeginPopupModal(popupName.data(), &m_showAboutPopup,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
    {
        drawAboutPopUpContent();
        ImGui::EndPopup();
    }
}

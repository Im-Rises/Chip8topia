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
        if (ImGui::MenuItem("About Chip8topia"))
        {
            m_showAboutPopup = true;
        }

        if (ImGui::MenuItem("About the Chip8"))
        {
            ImGui::OpenPopup("About the Chip8");
        }

        if (ImGui::MenuItem("About the project dependencies"))
        {
            ImGui::OpenPopup("About the project dependencies");
        }

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::drawAboutPopUpWindow() {
    if (m_showAboutPopup)
    {
        ImGui::OpenPopup("About");
    }
    if (ImGui::BeginPopupModal("About", &m_showAboutPopup,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), Chip8topia::PROJECT_NAME);
        ImGui::Text("Version: %s", Chip8topia::PROJECT_VERSION);
        ImGui::NewLine();

        ImGui::Text("Developed by:");
        ImGui::Text(" - %s", Chip8topia::PROJECT_AUTHOR);
        ImGui::NewLine();
        ImGui::Text("Github:");
        ImGui::Text(" - %s", Chip8topia::PROJECT_LINK);
        ImGui::NewLine();

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("About the Chip8", nullptr,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::Text("The Chip8 is an interpreted programming language developed by Joseph Weisbecker in the 1970s.");
        ImGui::Text("It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers to make game programming easier.");
    }

    if (ImGui::BeginPopupModal("About the project dependencies", nullptr,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::Text("The Chip8topia project uses the following dependencies:");
        //        ImGui::Text("Libraries used: ");
        //        ImGui::Text(" - OpenGL Vendor: %s", Chip8topia::getOpenGLVendor().data());
        //        ImGui::Text(" - OpenGL Version: %s", Chip8topia::getOpenGLVersion().data());
        //        ImGui::Text(" - GLFW Version: %s", Chip8topia::getGLFWVersion().data());
        //        ImGui::Text(" - Glad Version: %s", Chip8topia::getGladVersion().data());
        //        ImGui::Text(" - ImGui Version: %s", Chip8topia::getImGuiVersion().data());
        //        ImGui::NewLine();
    }
}

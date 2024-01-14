#include "Chip8topiaUi.h"

#include <imgui/imgui.h>

// Send the chip8core to the debugger or the chip8topia to the debugger
#include "Chip8Core/Chip8Core.h"
#include "Chip8topia.h"

void Chip8topiaUi::drawUi() {
    drawMainMenuBar();
}

void Chip8topiaUi::drawMainMenuBar() {
    //    static bool my_tool_active = true;
    //    static bool showAboutPopup = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open rom..", "Ctrl+O"))
            { /* Do stuff */
                // Reset emulation and load rom to memory
            }
            //            if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
            //            if (ImGui::MenuItem("Exit", "Alt+F4")) { glfwSetWindowShouldClose(window, true); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            //            if (ImGui::MenuItem("FullScreen", "F11")) { toggleFullScreen(); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Debugger"))
            {
                //                m_chip8.setDebuggerMode(true);
                // set a tooltip to show the window of each debugger tool
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("About..."))
        {
            if (ImGui::MenuItem("About %s ...", Chip8topia::PROJECT_NAME))
            {
                m_showAboutPopup = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Chip8topiaUi::drawAboutPopUpWindow() {
    //    if (m_showAboutPopup)
    //    {
    //        ImGui::OpenPopup("About");
    //    }
    if (ImGui::BeginPopupModal("About", &m_showAboutPopup,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), Chip8topia::PROJECT_NAME);
        ImGui::Text("Version: %s", Chip8topia::PROJECT_VERSION);
        ImGui::NewLine();

        ImGui::Text("Developed by: ");
        ImGui::Text("%s", Chip8topia::PROJECT_AUTHOR);
        ImGui::NewLine();
        ImGui::Text("Github: %s", Chip8topia::PROJECT_LINK);
        ImGui::NewLine();

        //        ImGui::Separator();
        ImGui::Text("Build with: ");
        ImGui::Text(" - OpenGL Vendor: %s", Chip8topia::getOpenGLVendor().data());
        ImGui::Text(" - OpenGL Version: %s", Chip8topia::getOpenGLVersion().data());
        ImGui::Text(" - GLFW Version: %s", Chip8topia::getGLFWVersion().data());
        ImGui::Text(" - Glad Version: %s", Chip8topia::getGladVersion().data());
        ImGui::Text(" - ImGui Version: %s", Chip8topia::getImGuiVersion().data());
        ImGui::NewLine();
        //        ImGui::Separator();

        // Is this really necessary ?
        if (ImGui::Button("Close"))
        { ImGui::CloseCurrentPopup(); }

        ImGui::EndPopup();
    }
}

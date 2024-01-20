#include "Chip8topiaUi.h"

#include <imgui/imgui.h>

#include "../Chip8topia.h"

void Chip8topiaUi::drawUi() {
    drawMainMenuBar();
}

void Chip8topiaUi::drawMainMenuBar() {
    if (ImGui::BeginMainMenuBar())
    {
        drawFileMenu();
        drawViewMenu();
        drawDesignMenu();
        //        drawToolsMenu();
        m_chip8topiaDebugger.drawDebuggerMenu();
        drawAboutMenu();

        ImGui::EndMainMenuBar();
    }

    drawAboutPopUpWindow();
}

void Chip8topiaUi::drawFileMenu() {
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open rom..", "Ctrl+O"))
        { /* Do stuff */
            // Reset emulation and load rom to memory
        }
        if (ImGui::MenuItem("Exit", "Alt+F4"))
        {
            //            glfwSetWindowShouldClose(window, true);
        }
        ImGui::EndMenu();
    }
}
void Chip8topiaUi::drawViewMenu() {
    if (ImGui::BeginMenu("View"))
    {
        if (ImGui::MenuItem("FullScreen", "F11"))
        {
            //            toggleFullScreen();
        }
        //        if (ImGui::MenuItem("Pause", "Space"))
        //        {
        ////            togglePause();
        //        }
        //        if (ImGui::MenuItem("Step", "S"))
        //        {
        ////            toggleStep();
        //        }

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
// void Chip8topiaUi::drawToolsMenu() {
//     if (ImGui::BeginMenu("Tools"))
//     {
//         if (ImGui::MenuItem("Debugger"))
//         {
//             //                m_chip8.setDebuggerMode(true);
//             // set a tooltip to show the window of each debugger tool
//         }
//         ImGui::EndMenu();
//     }
// }

void Chip8topiaUi::drawAboutMenu() {
    if (ImGui::BeginMenu("About..."))
    {
        //        if (ImGui::IsItemClicked())
        //        {
        //            m_showAboutPopup = true;
        //        }
        //
        //        if (ImGui::IsItemHovered())
        //        {
        //            ImGui::SetTooltip("About Chip8topia");
        //        }

        if (ImGui::MenuItem("About Chip8topia"))
        {
            m_showAboutPopup = true;
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

        //        ImGui::Text("Libraries used: ");
        //        ImGui::Text(" - OpenGL Vendor: %s", Chip8topia::getOpenGLVendor().data());
        //        ImGui::Text(" - OpenGL Version: %s", Chip8topia::getOpenGLVersion().data());
        //        ImGui::Text(" - GLFW Version: %s", Chip8topia::getGLFWVersion().data());
        //        ImGui::Text(" - Glad Version: %s", Chip8topia::getGladVersion().data());
        //        ImGui::Text(" - ImGui Version: %s", Chip8topia::getImGuiVersion().data());
        //        ImGui::NewLine();

        ImGui::EndPopup();
    }
}

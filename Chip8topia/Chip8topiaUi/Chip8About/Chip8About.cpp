#include "Chip8About.h"

#include "../../Chip8topia.h"

Chip8About::Chip8About() : m_menuItem{
                               { Chip8topia::PROJECT_EMULATION_CONSOLE_NAME, false, [this]() { drawAboutChip8topia(); },
                                   Chip8topia::PROJECT_NAME, false, [this]() { drawAboutChip8(); } }
                           } {
}


void Chip8About::drawAboutMenu() {
    if (ImGui::BeginMenu("About..."))
    {
        for (auto& menuItem : m_menuItem)
        {
            menuItem.drawMenuItem();
        }

        ImGui::EndMenu();
    }
}

void Chip8About::drawAboutWindows() {
    for (auto& menuItem : m_menuItem)
    {
        menuItem.drawWindow();
    }
}

void Chip8About::drawAboutChip8topia() {
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
    ImGui::Text("\n"
                "Description:\n"
                " - %s",
        Chip8topia::PROJECT_DESCRIPTION);
}

void Chip8About::drawAboutChip8() {
    ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), Chip8topia::PROJECT_EMULATION_CONSOLE_NAME);
    ImGui::Text(Chip8topia::PROJECT_EMULATION_CONSOLE_DESCRIPTION);
}

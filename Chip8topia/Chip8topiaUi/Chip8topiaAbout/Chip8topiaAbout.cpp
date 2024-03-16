#include "Chip8topiaAbout.h"

#include <IconsFontAwesome6.h>

#include "../../Chip8topia.h"

Chip8topiaAbout::Chip8topiaAbout() : m_menuItem{
                                         { ImGuiMenuItemPopup<>(Chip8topia::PROJECT_NAME, false, [this]()
                                               { drawAboutChip8topia(); }),
                                             ImGuiMenuItemPopup<>(Chip8topia::PROJECT_EMULATION_CONSOLE_NAME, false, [this]()
                                                 { drawAboutChip8(); }) }
                                     }
{
}

void Chip8topiaAbout::drawAboutMenu()
{
    if (ImGui::BeginMenu(ICON_FA_INFO " About"))
    {
        for (auto& menuItem : m_menuItem)
        {
            menuItem.drawMenuItem();
        }

        ImGui::EndMenu();
    }
}

void Chip8topiaAbout::drawAboutWindows()
{
    for (auto& menuItem : m_menuItem)
    {
        menuItem.drawWindow();
    }
}

void Chip8topiaAbout::drawAboutChip8topia()
{
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

void Chip8topiaAbout::drawAboutChip8()
{
    ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), Chip8topia::PROJECT_EMULATION_CONSOLE_NAME);
    ImGui::Text(Chip8topia::PROJECT_EMULATION_CONSOLE_DESCRIPTION);
}

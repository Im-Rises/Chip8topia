#include "Chip8EmulationUi.h"

#include <imgui.h>
#include <fmt/format.h>

#include "../../Chip8topia/Chip8topia.h"
#include "../../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

void Chip8EmulationUi::drawEmulationMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("Engine/Emulation"))
    {
        m_Chip8SettingsMenuItem.drawMenuItem();

        if (ImGui::MenuItem(fmt::format("Toggle turbo mode : {}", chip8topia.getIsTurboMode() ? "ON " : "OFF").c_str(), "Y"))
        {
            chip8topia.toggleTurboMode();
        }

        if (ImGui::MenuItem(chip8topia.getChip8Emulator().getIsPaused() ? "Resume" : "Pause", "P"))
        {
            Chip8topiaInputHandler::getInstance().m_PauseEmulationEvent.trigger();
        }

        if (ImGui::MenuItem("Restart", "L"))
        {
            Chip8topiaInputHandler::getInstance().m_RestartEmulationEvent.trigger();
        }

        ImGui::EndMenu();
    }
}

void Chip8EmulationUi::drawEmulationWindows(Chip8topia& chip8topia) {
    m_Chip8SettingsMenuItem.drawWindow();
}

void Chip8EmulationUi::drawEmulationSettings() {
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Emulation Settings");

    ImGui::Separator();

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Console version");
    ImGui::Selectable("Chip8", false);
    ImGui::Selectable("SChip", false);

    ImGui::Separator();

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Emulation Speed");
    ImGui::Selectable("600HZ", false);
    ImGui::Selectable("400HZ", false);
    ImGui::Selectable("200HZ", false);
}

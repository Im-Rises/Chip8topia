#include "Chip8EmulationUi.h"

#include <imgui.h>
#include <fmt/format.h>

#include "../../Chip8topia/Chip8topia.h"
#include "../../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

void Chip8EmulationUi::drawEmulationMenu(Chip8topia& chip8topia) {
    // TODO: Add the ability to change the Chip8 version and frequency
    if (ImGui::BeginMenu("Engine/Emulation"))
    {
        if (ImGui::MenuItem("Change Chip8 version", "V"))
        {
        }

        if (ImGui::MenuItem("Change Chip8 frequency", "F"))
        {
        }

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

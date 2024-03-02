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

        //        if (ImGui::MenuItem(chip8topia.getChip8Emulator().getIsPaused() ? "Resume" : "Pause", "P"))
        //        {
        //            Chip8topiaInputHandler::getInstance().m_PauseEmulationEvent.trigger();
        //        }

        if (ImGui::MenuItem("Restart", "L"))
        {
            Chip8topiaInputHandler::getInstance().m_RestartEmulationEvent.trigger();
        }

        ImGui::EndMenu();
    }
}

void Chip8EmulationUi::drawEmulationWindows(Chip8topia& chip8topia) {
    m_Chip8SettingsMenuItem.drawWindow(&chip8topia);
}

void Chip8EmulationUi::drawEmulationSettings(Chip8topia* chip8topia) {
    Chip8Emulator& emulator = chip8topia->getChip8Emulator();

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Emulation Settings");

    ImGui::Separator();

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Console version");
    ImGui::Text("Current core: %d", emulator.getCoreType());

    if (ImGui::Selectable("Chip8", m_selectedCore == Chip8CoreType::Chip8))
    {
        m_selectedCore = Chip8CoreType::Chip8;
    }
    if (ImGui::Selectable("SChip 1.1 (Legacy)", m_selectedCore == Chip8CoreType::SChip11))
    {
        m_selectedCore = Chip8CoreType::SChip11;
    }
    if (ImGui::Selectable("SChipC (Modern)", m_selectedCore == Chip8CoreType::SChipC))
    {
        m_selectedCore = Chip8CoreType::SChipC;
    }
    //    if (ImGui::Selectable("Xo-Chip", emulator.getCoreType() == Chip8CoreType::XoChip))
    //    {
    //    }

    ImGui::Separator();

    ImGui::TextColored(ImVec4(1.0F, 1.0F, 0.0F, 1.0F), "Emulation Speed");
    if (ImGui::Selectable("600HZ", m_selectedFrequency == Chip8Frequency::FREQ_600_HZ))
    {
    }

    if (ImGui::Selectable("1200HZ", m_selectedFrequency == Chip8Frequency::FREQ_1200_HZ))
    {
    }

    if (ImGui::Selectable("1800HZ", m_selectedFrequency == Chip8Frequency::FREQ_1800_HZ))
    {
    }

    ImGui::Separator();
    if (ImGui::Button("Apply"))
    {
        emulator.switchCore(m_selectedCore);
        m_Chip8SettingsMenuItem.m_isOpen = false;
    }
}

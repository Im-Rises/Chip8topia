#include "Chip8EmulationUi.h"

#include <imgui.h>
#include <fmt/format.h>

#include "../../Chip8topia/Chip8topia.h"
#include "../../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

void Chip8EmulationUi::drawEmulationMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("Engine/Emulation"))
    {
        for (auto& menuItem : m_menuItems)
        {
            menuItem.drawMenuItem();
        }

        if (ImGui::MenuItem(fmt::format("Toggle turbo mode : {}", chip8topia.getIsTurboMode() ? "ON " : "OFF").c_str(), "Y"))
        {
            chip8topia.toggleTurboMode();
        }

        if (ImGui::MenuItem("Restart", "L"))
        {
            Chip8topiaInputHandler::getInstance().m_RestartEmulationEvent.trigger();
        }

        ImGui::EndMenu();
    }
}

void Chip8EmulationUi::drawEmulationWindows(Chip8topia& chip8topia) {
    for (auto& menuItem : m_menuItems)
    {
        menuItem.drawWindow(&chip8topia);
    }
}

void Chip8EmulationUi::closeAllWindows() {
    for (auto& menuItem : m_menuItems)
    {
        menuItem.m_isOpen = false;
    }
}

void Chip8EmulationUi::drawEmulationStats(Chip8topia& chip8topia) {
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Screen size: %dx%d", chip8topia.getWindowWidth(), chip8topia.getWindowHeight());
    ImGui::Text("Frame time: %.3f ms", 1000.0F / ImGui::GetIO().Framerate);
    ImGui::Text("Clock count this frame: %u", chip8topia.getChip8Emulator().getClockCountThisFrame());
}

void Chip8EmulationUi::drawEmulationSettings(Chip8topia* chip8topia) {
    Chip8Emulator& emulator = chip8topia->getChip8Emulator();

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Emulation Settings");

    ImGui::Separator();

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Console version");
    ImGui::Text("Current core: %hhu", static_cast<uint8>(emulator.getCoreType()));

    if (ImGui::Selectable("Chip8", m_selectedCore == Chip8CoreType::Chip8))
    {
        m_selectedCore = Chip8CoreType::Chip8;
    }
    if (ImGui::Selectable("SChip 1.1 (legacy)", m_selectedCore == Chip8CoreType::SChip11Legacy))
    {
        m_selectedCore = Chip8CoreType::SChip11Legacy;
    }
    if (ImGui::Selectable("SChip 1.1 (modern)", m_selectedCore == Chip8CoreType::SChip11Modern))
    {
        m_selectedCore = Chip8CoreType::SChip11Modern;
    }
    if (ImGui::Selectable("SChipC", m_selectedCore == Chip8CoreType::SChipC))
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
        m_selectedFrequency = Chip8Frequency::FREQ_600_HZ;
    }

    if (ImGui::Selectable("1200HZ", m_selectedFrequency == Chip8Frequency::FREQ_1200_HZ))
    {
        m_selectedFrequency = Chip8Frequency::FREQ_1200_HZ;
    }

    if (ImGui::Selectable("1800HZ", m_selectedFrequency == Chip8Frequency::FREQ_1800_HZ))
    {
        m_selectedFrequency = Chip8Frequency::FREQ_1800_HZ;
    }

    ImGui::Separator();
    if (ImGui::Button("Apply"))
    {
        emulator.switchCoreFrequency(m_selectedCore, m_selectedFrequency);
        m_menuItems[1].m_isOpen = false;
    }
}

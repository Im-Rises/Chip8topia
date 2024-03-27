#include "Chip8topiaEmulationUi.h"

#include <imgui.h>
#include <fmt/format.h>

#include "../../Chip8topia/Chip8topia.h"

void Chip8topiaEmulationUi::drawEmulationMenu(Chip8topia& chip8topia)
{
    if (ImGui::BeginMenu(ICON_FA_GAMEPAD " Emulation"))
    {
        for (auto& menuItem : m_menuItems)
        {
            menuItem.drawMenuItem();
        }

        if (ImGui::MenuItem(fmt::format(ICON_FA_ROCKET " Toggle turbo mode : {}", chip8topia.getIsTurboMode() ? "ON " : "OFF").c_str(), "Y", chip8topia.getIsTurboMode()))
        {
            chip8topia.toggleTurboMode();
        }

        static constexpr const char* const PAUSE_TEXT = ICON_FA_PAUSE " Pause";
        static constexpr const char* const RESUME_TEXT = ICON_FA_PLAY " Play";
        const bool isPaused = chip8topia.getChip8Emulator().getIsBreak();
        if (ImGui::MenuItem(isPaused ? RESUME_TEXT : PAUSE_TEXT, "P", isPaused))
        {
            Chip8topiaInputHandler::getInstance().m_TogglePauseEmulationEvent.trigger();
        }

        if (ImGui::MenuItem(ICON_FA_REPEAT " Restart", "L"))
        {
            Chip8topiaInputHandler::getInstance().m_RestartEmulationEvent.trigger();
        }

        ImGui::EndMenu();
    }
}

void Chip8topiaEmulationUi::drawEmulationWindows(Chip8topia& chip8topia)
{
    for (auto& menuItem : m_menuItems)
    {
        menuItem.drawWindow(&chip8topia);
    }
}

void Chip8topiaEmulationUi::closeAllWindows()
{
    for (auto& menuItem : m_menuItems)
    {
        menuItem.m_isOpen = false;
    }
}

void Chip8topiaEmulationUi::drawEmulationStats(Chip8topia& chip8topia)
{
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    //    ImGui::Text("Screen size: %dx%d", chip8topia.getWindowWidth(), chip8topia.getWindowHeight());
    ImGui::Text("Frame time: %.3f ms", 1000.0F / ImGui::GetIO().Framerate);
}

void Chip8topiaEmulationUi::drawEmulationSettings(Chip8topia* chip8topia)
{
    Chip8Emulator& emulator = chip8topia->getChip8Emulator();

    ImGui::Text("Current core: %s", emulator.getConsoleName().c_str());
    ImGui::Text("Current frequency: %uHZ", static_cast<unsigned int>(emulator.getFrequency()));

    ImGui::NewLine();
    ImGui::Separator();

    ImGui::TextColored(ImVec4(1.0F, 1.0F, 0.0F, 1.0F), "Console version");

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
    if (ImGui::Selectable("Xo-Chip", m_selectedCore == Chip8CoreType::XoChip))
    {
        m_selectedCore = Chip8CoreType::XoChip;
    }

    ImGui::NewLine();
    ImGui::Separator();

    ImGui::TextColored(ImVec4(1.0F, 1.0F, 0.0F, 1.0F), "Emulation Speed");
    if (ImGui::Selectable("600HZ", m_selectedFrequency == Chip8Frequency::Freq600Hz))
    {
        m_selectedFrequency = Chip8Frequency::Freq600Hz;
    }

    if (ImGui::Selectable("1200HZ", m_selectedFrequency == Chip8Frequency::Freq1200Hz))
    {
        m_selectedFrequency = Chip8Frequency::Freq1200Hz;
    }

    if (ImGui::Selectable("1800HZ", m_selectedFrequency == Chip8Frequency::Freq1800Hz))
    {
        m_selectedFrequency = Chip8Frequency::Freq1800Hz;
    }

    ImGui::NewLine();
    ImGui::Separator();
    if (ImGui::Button("Apply"))
    {
        emulator.switchCoreFrequency(m_selectedCore, m_selectedFrequency);
        m_menuItems[0].m_isOpen = false;
    }
}

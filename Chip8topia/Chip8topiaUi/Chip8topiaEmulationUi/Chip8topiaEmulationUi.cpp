#include "Chip8topiaEmulationUi.h"

#include <imgui.h>
#include <fmt/format.h>

#include "../../Chip8topia/Chip8topia.h"

void Chip8topiaEmulationUi::drawEmulationMenu(Chip8topia& chip8topia)
{
    if (ImGui::BeginMenu(ICON_FA_GAMEPAD " Emulation"))
    {
        ImGui::MenuItem(CHIP8_SETTINGS_STRING, nullptr, &m_emulationSettingsOpen);

        ImGui::MenuItem(CHIP8_STATS_STRING, nullptr, &m_emulationStatsOpen);

        if (ImGui::MenuItem(fmt::format(ICON_FA_ROCKET " Toggle turbo mode : {}", chip8topia.getIsTurboMode() ? "ON " : "OFF").c_str(), "Y", chip8topia.getIsTurboMode()))
        {
            chip8topia.toggleTurboMode();
        }

        const bool isPaused = chip8topia.getChip8Emulator().getIsBreak();
        if (ImGui::MenuItem(isPaused ? ICON_FA_PLAY " Play" : ICON_FA_PAUSE " Pause", "P", isPaused))
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

void Chip8topiaEmulationUi::drawEmulationWindows(Chip8topia& chip8topia, bool isMainBarOpen)
{
    drawEmulationSettings(&chip8topia);
    drawEmulationStats(chip8topia, isMainBarOpen);
}

void Chip8topiaEmulationUi::closeAllWindows()
{
    m_emulationSettingsOpen = false;
#if !defined(BUILD_DEBUG)
    m_emulationStatsOpen = false;
#endif
}

void Chip8topiaEmulationUi::drawEmulationStats(Chip8topia& chip8topia, bool isMainBarOpen)
{
    if (m_emulationStatsOpen)
    {
        m_performanceMonitor.drawWindow(chip8topia, isMainBarOpen);
    }
}

void Chip8topiaEmulationUi::drawEmulationSettings(Chip8topia* chip8topia)
{
    Chip8Emulator& emulator = chip8topia->getChip8Emulator();

    if (!m_emulationSettingsOpen)
    {
        return;
    }

    if (ImGui::Begin(CHIP8_SETTINGS_STRING, &m_emulationSettingsOpen))
    {
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
            m_emulationSettingsOpen = false;
        }

        ImGui::End();
    }
}

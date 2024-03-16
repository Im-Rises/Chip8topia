#include "Chip8VideoUi.h"

#include "../Chip8Emulator/Chip8Emulator.h"

void Chip8VideoUi::drawVideoMenu()
{
    if (ImGui::BeginMenu(ICON_FA_TV " Video"))
    {
        for (auto& menuItem : m_menuItem)
        {
            menuItem.drawMenuItem();
        }

        ImGui::EndMenu();
    }
}

void Chip8VideoUi::drawVideoWindows(Chip8Emulator& emulator)
{
    for (auto& menuItem : m_menuItem)
    {
        menuItem.drawWindow(&emulator);
    }
}

void Chip8VideoUi::closeAllWindows()
{
    for (auto& menuItem : m_menuItem)
    {
        menuItem.m_isOpen = false;
    }
}

void Chip8VideoUi::drawBackgroundColor(Chip8Emulator* emulator)
{
    ImGui::ColorPicker4("Background color", reinterpret_cast<float*>(&emulator->getChip8VideoEmulation().getBackgroundColor()));
}

void Chip8VideoUi::drawMainPlaneColor(Chip8Emulator* emulator)
{
    ImGui::ColorPicker4("Draw color", reinterpret_cast<float*>(&emulator->getChip8VideoEmulation().getForegroundColor()));
}

void Chip8VideoUi::drawSecondaryPlaneColor(Chip8Emulator* emulator)
{
    ImGui::ColorPicker4("Draw color", reinterpret_cast<float*>(&emulator->getChip8VideoEmulation().getForegroundColor()));
}

void Chip8VideoUi::drawPixelsCommonColor(Chip8Emulator* emulator)
{
    ImGui::ColorPicker4("Draw color", reinterpret_cast<float*>(&emulator->getChip8VideoEmulation().getForegroundColor()));
}

#include "Chip8topiaVideoUi.h"

#include "../Chip8Emulator/Chip8Emulator.h"

void Chip8topiaVideoUi::drawVideoMenu()
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

void Chip8topiaVideoUi::drawVideoWindows(Chip8Emulator& emulator)
{
    for (auto& menuItem : m_menuItem)
    {
        menuItem.drawWindow(&emulator);
    }
}

void Chip8topiaVideoUi::closeAllWindows()
{
    for (auto& menuItem : m_menuItem)
    {
        menuItem.m_isOpen = false;
    }
}

void Chip8topiaVideoUi::drawBackgroundColor(Chip8Emulator* emulator)
{
    ImGui::ColorPicker4("Background color", reinterpret_cast<float*>(&emulator->getChip8VideoEmulation().getBackgroundColor()));
}

void Chip8topiaVideoUi::drawMainPlaneColor(Chip8Emulator* emulator)
{
    ImGui::ColorPicker4("Draw color", reinterpret_cast<float*>(&emulator->getChip8VideoEmulation().getMainPlaneColor()));
}

void Chip8topiaVideoUi::drawSubPlaneColor(Chip8Emulator* emulator)
{
    ImGui::ColorPicker4("Draw color", reinterpret_cast<float*>(&emulator->getChip8VideoEmulation().getSubPlaneColor()));
}

void Chip8topiaVideoUi::drawPixelsCommonColor(Chip8Emulator* emulator)
{
    ImGui::ColorPicker4("Draw color", reinterpret_cast<float*>(&emulator->getChip8VideoEmulation().getPixelsCommonColor()));
}

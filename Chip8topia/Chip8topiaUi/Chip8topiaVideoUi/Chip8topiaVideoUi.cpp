#include "Chip8topiaVideoUi.h"

#include "../../Chip8Emulator/Chip8Emulator/Chip8VideoEmulation.h"
#include "../../Chip8Emulator/Chip8Emulator.h"

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

void Chip8topiaVideoUi::drawPlanesColorEditor(Chip8Emulator& emulator)
{
    Chip8VideoEmulation& videoEmulation = emulator.getChip8VideoEmulation();


    ImGui::TextColored(ImColor(1.0F, 1.0F, 0.0F, 1.0F), "Color Mode");
    ImGui::Selectable("Grayscale", videoEmulation.getColorMode() == EmulationColorMode::Grayscale);
    if (ImGui::IsItemClicked())
    {
        videoEmulation.resetToGrayscaleColors();
    }

    //    ImGui::Selectable("BW", videoEmulation.getColorMode() == EmulationColorMode::BW);
    //    if (ImGui::IsItemClicked())
    //    {
    //        videoEmulation.resetToBWColors();
    //    }

    ImGui::Selectable("Color", videoEmulation.getColorMode() == EmulationColorMode::Color);
    if (ImGui::IsItemClicked())
    {
        videoEmulation.resetToColorColors();
    }

    ImGui::NewLine();
    ImGui::Separator();

    ImGui::TextColored(ImColor(1.0F, 1.0F, 0.0F, 1.0F), "Colors");
    for (auto i = 0; i < PpuBase::COLOR_COUNT; i++)
    {
        ImGui::PushID(i);
        ImGui::ColorEdit4("##Color", reinterpret_cast<float*>(&videoEmulation.getColor(i)));
        ImGui::PopID();
    }
}

void Chip8topiaVideoUi::drawPlanes(Chip8Emulator& emulator)
{
    static constexpr const ImVec2 LORES_TEXTURE_SIZE = ImVec2(PpuBase::SCREEN_LORES_MODE_WIDTH, PpuBase::SCREEN_LORES_MODE_HEIGHT);
    static constexpr const ImVec2 HIRES_TEXTURE_SIZE = ImVec2(PpuBase::SCREEN_HIRES_MODE_WIDTH, PpuBase::SCREEN_HIRES_MODE_HEIGHT);

    ImGui::Text("Lores Planes");
    for (auto i = 0; i < PpuBase::PLANE_COUNT; i++)
    {
        ImGui::PushID(i);
        ImGui::Image(reinterpret_cast<ImTextureID>(emulator.getChip8VideoEmulation().getLoresPlaneTexture(i)), LORES_TEXTURE_SIZE);
        ImGui::PopID();
        ImGui::SameLine();
    }

    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::Text("Hires Planes");
    for (auto i = 0; i < PpuBase::PLANE_COUNT; i++)
    {
        ImGui::PushID(i);
        ImGui::Image(reinterpret_cast<ImTextureID>(emulator.getChip8VideoEmulation().getHiresPlaneTexture(i)), HIRES_TEXTURE_SIZE);
        ImGui::PopID();
        ImGui::SameLine();
    }
}

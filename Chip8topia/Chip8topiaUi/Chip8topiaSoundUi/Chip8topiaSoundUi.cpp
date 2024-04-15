#include "Chip8topiaSoundUi.h"

#include "../../Chip8Emulator/Chip8Emulator/Chip8SoundEmulation.h"
#include "../../Chip8Emulator/Chip8Emulator.h"

void Chip8topiaSoundUi::drawSoundMenu(Chip8Emulator& emulator)
{
    if (ImGui::BeginMenu(ICON_FA_MUSIC " Sound"))
    {
        if (ImGui::MenuItem("Sound state", nullptr, nullptr))
        {
            m_isSoundStateWindowOpen = !m_isSoundStateWindowOpen;
        }

        drawSoundFrequency(emulator);
        drawSoundVolume(emulator);

        ImGui::EndMenu();
    }
}

void Chip8topiaSoundUi::drawSoundWindows(Chip8Emulator& emulator)
{
    drawSoundState(emulator);
}

void Chip8topiaSoundUi::closeAllWindows()
{
    m_isSoundStateWindowOpen = false;
}

void Chip8topiaSoundUi::drawSoundState(Chip8Emulator& emulator)
{
    if (!m_isSoundStateWindowOpen)
    {
        return;
    }

    if (ImGui::Begin("Sound state", &m_isSoundStateWindowOpen))
    {
        if (emulator.getChip8SoundEmulation().getIsPlaying())
        {
            ImGui::Text("Playing");
        }
        else
        {
            ImGui::Text("Stopped");
        }

        ImGui::Text("ST: %d", emulator.getChip8Core()->getCpu()->getST());

        ImGui::End();
    }
}

void Chip8topiaSoundUi::drawSoundFrequency(Chip8Emulator& emulator)
{
    // TODO: Refactor
    int* frequency = emulator.getChip8SoundEmulation().getFrequencyPtr();
    const int frequencyValue = *frequency;
    ImGui::Text("Frequency");
    ImGui::SameLine();
    ImGui::DragInt("##FrequencySound", frequency, 1.0F, 0.0F, 10000.0F);
    if (frequencyValue != *frequency)
    {
        emulator.getChip8SoundEmulation().initSoundBuffer();
    }
}

void Chip8topiaSoundUi::drawSoundVolume(Chip8Emulator& emulator)
{
    // TODO: Refactor
    float* volume = emulator.getChip8SoundEmulation().getVolumePtr();
    const float volumeValue = *volume;
    ImGui::Text("Volume   ");
    ImGui::SameLine();
    ImGui::DragFloat("##VolumeSound", volume, 0.01F, 0.0F, 1.0F);
    if (volumeValue != *volume)
    {
        emulator.getChip8SoundEmulation().initSoundBuffer();
    }
}

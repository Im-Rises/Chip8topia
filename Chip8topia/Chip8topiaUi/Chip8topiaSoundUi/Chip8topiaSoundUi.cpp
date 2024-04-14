#include "Chip8topiaSoundUi.h"

#include "../../Chip8Emulator/Chip8Emulator/Chip8SoundEmulation.h"
#include "../../Chip8Emulator/Chip8Emulator.h"

void Chip8topiaSoundUi::drawSoundMenu()
{
    if (ImGui::BeginMenu(ICON_FA_MUSIC " Sound"))
    {
        for (auto& menuItem : m_menuItem)
        {
            menuItem.drawMenuItem();
        }

        ImGui::EndMenu();
    }
}

void Chip8topiaSoundUi::drawSoundWindows(Chip8Emulator& emulator)
{
    // Window for sound is muted or not and volume value (integrated bar in the item menu)
    // Window with the sound state (ST and if playing or not)
}

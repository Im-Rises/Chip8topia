#include "Chip8SoundEmulation.h"

#include <sdl.h>
#include "../Chip8CoreBase/Chip8CoreBase.h"

void Chip8SoundEmulation::update(Chip8CoreBase* chip8Core)
{
    //    // get ST
    //    const auto st = chip8Core->getCpu()->getST();
    //
    //    // if ST > 0, play sound
    //    if (st > 0)
    //    {
    //        // play sound
    //        SDL_PauseAudio(0);
    //    }
    //    else
    //    {
    //        // pause sound
    //        SDL_PauseAudio(1);
    //    }
}

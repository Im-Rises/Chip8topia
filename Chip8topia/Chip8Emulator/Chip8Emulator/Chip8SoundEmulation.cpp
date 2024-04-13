#include "Chip8SoundEmulation.h"

#include <SDL.h>

#include "../Chip8CoreBase/Chip8CoreBase.h"

Chip8SoundEmulation::Chip8SoundEmulation()
{
    //    SDL_AudioSpec spec = {
    //        .freq = FREQUENCY_RESOLUTION,
    //        .format = AUDIO_S16SYS, // Signed 16 bit integer format
    //        .channels = 1,
    //        .samples = 4096,         // The size of each "chunk"
    //        .callback = play_buffer, // user-defined function that provides the audio data
    //        .userdata = nullptr      // an argument to the callback function (we dont need any)
    //    };
    //
    //    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    //
    //    //    for (int i = 0; i < BUFFER_LEN; i++)
    //    //    {
    //    //        buffer[i] = format(tone(440, i), 0.5);
    //    //    }
}

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

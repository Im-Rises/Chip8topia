#include "Chip8SoundEmulation.h"

#include <SDL.h>
#include <iostream>

#include "../Chip8CoreBase/Chip8CoreBase.h"

// Thanks to https://gist.github.com/jacobsebek/10867cb10cdfccf1d6cfdd24fa23ee96 !!!

Sint16 format(double sample, double amplitude)
{
    // 32567 is the maximum value of a 16 bit signed integer (2^15-1)
    return (Sint16)(sample * 32567 * amplitude);
}

double tone(double hz, unsigned long time)
{
    return sin(time * hz * M_PI * 2 / Chip8SoundEmulation::FREQUENCY_RESOLUTION);
}

double saw(double hz, unsigned long time)
{
    return fmod(time * hz / Chip8SoundEmulation::FREQUENCY_RESOLUTION, 1) * 2 - 1;
}

double square(double hz, unsigned long time)
{
    double sine = tone(hz, time);
    return sine > 0.0 ? 1.0 : -1.0;
}

Chip8SoundEmulation::Chip8SoundEmulation() : m_buffer{},
                                             m_spec{
                                                 .freq = FREQUENCY_RESOLUTION,
                                                 .format = AUDIO_S16SYS,
                                                 .channels = 1,
                                                 .samples = 4096,
                                                 .callback = Chip8SoundEmulation::soundPlayerCallback,
                                                 .userdata = this,
                                             },
                                             m_bufferPosition(0),
                                             m_volume(0.5),
                                             m_isPlaying(false)
{
    m_dev = SDL_OpenAudioDevice(nullptr, 0, &m_spec, nullptr, 0);

    for (int i = 0; i < BUFFER_LEN; i++)
    {
        m_buffer[i] = format(square(440, i), m_volume);
    }
}

Chip8SoundEmulation::~Chip8SoundEmulation()
{
    SDL_CloseAudioDevice(m_dev);
}

void Chip8SoundEmulation::update(const std::unique_ptr<Chip8CoreBase>& chip8Core)
{
    const auto st = chip8Core->getCpu()->getST();

    if (st > 0)
    {
        if (!m_isPlaying)
        {
            SDL_PauseAudioDevice(m_dev, 0);
            m_isPlaying = true;
        }
    }
    else
    {
        if (m_isPlaying)
        {
            SDL_PauseAudioDevice(m_dev, 1);
            m_isPlaying = false;
        }
    }
}

void Chip8SoundEmulation::stop()
{
    SDL_PauseAudioDevice(m_dev, 1);
    m_isPlaying = false;
}

void Chip8SoundEmulation::soundPlayerCallback(void* userdata, unsigned char* stream, int len)
{
    auto* chip8SoundEmulation = static_cast<Chip8SoundEmulation*>(userdata);

    chip8SoundEmulation->soundPlayer(stream, len);
}

void Chip8SoundEmulation::soundPlayer(unsigned char* stream, int len)
{
    SDL_memset(stream, m_spec.silence, len);

    len /= 2;

    if (m_bufferPosition + len >= BUFFER_LEN)
    {
        m_bufferPosition = 0;
    }

    SDL_memcpy(stream, &m_buffer[m_bufferPosition], len * 2);

    m_bufferPosition += len;
}

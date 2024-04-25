#include "Chip8SoundEmulation.h"

#include <SDL.h>
#include <iostream>

#include "../Chip8CoreBase/Chip8CoreBase.h"

// Thanks to https://gist.github.com/jacobsebek/10867cb10cdfccf1d6cfdd24fa23ee96 !!!

Sint16 format(double sample, double amplitude)
{
    // 32567 is the maximum value of a 16 bit signed integer (2^15-1)
    return static_cast<Sint16>(sample * 32567 * amplitude);
}

// Generate a sine wave
double tone(double hz, unsigned long time)
{
    return sin(time * hz * M_PI * 2 / Chip8SoundEmulation::FREQUENCY_RESOLUTION);
}

// Generate a sawtooth wave
double saw(double hz, unsigned long time)
{
    return fmod(time * hz / Chip8SoundEmulation::FREQUENCY_RESOLUTION, 1) * 2 - 1;
}

// Generate a square wave
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
                                             m_isPlaying(false),
                                             m_squareSoundFrequency(440),
                                             m_waveType(WaveType::Square)
{
    m_dev = SDL_OpenAudioDevice(nullptr, 0, &m_spec, nullptr, 0);
    initSoundBuffer(square);
}

Chip8SoundEmulation::~Chip8SoundEmulation()
{
    SDL_CloseAudioDevice(m_dev);
}

void Chip8SoundEmulation::initSoundBuffer(std::function<double(double, unsigned long)> waveFunction)
{
    for (int i = 0; i < BUFFER_LEN; i++)
    {
        m_buffer[i] = format(waveFunction(m_squareSoundFrequency, i), m_volume);
    }
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

void Chip8SoundEmulation::setWaveType(WaveType waveType)
{
    m_waveType = waveType;

    switch (m_waveType)
    {
    case WaveType::Sine:
        initSoundBuffer(tone);
        break;
    case WaveType::Saw:
        initSoundBuffer(saw);
        break;
    case WaveType::Square:
        initSoundBuffer(square);
        break;
    }
}

void Chip8SoundEmulation::soundPlayerCallback(void* userdata, unsigned char* stream, int streamLength)
{
    auto* chip8SoundEmulation = static_cast<Chip8SoundEmulation*>(userdata);

    chip8SoundEmulation->soundPlayer(stream, streamLength);
}

void Chip8SoundEmulation::soundPlayer(unsigned char* stream, int streamLength)
{
    static constexpr size_t SAMPLE_SIZE = sizeof(Sint16);
    const int sampleLength = streamLength / SAMPLE_SIZE;

    SDL_memset(stream, m_spec.silence, streamLength);

    if (m_bufferPosition + sampleLength >= BUFFER_LEN)
    {
        m_bufferPosition = 0;
    }

    // TODO: Correct this code to have no concurrency issues!!! Really important!!!
    try
    {
        SDL_memcpy(stream, &m_buffer[m_bufferPosition], streamLength);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (const char* const e)
    {
        std::cerr << e << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown error" << '\n';
    }

    m_bufferPosition += sampleLength;
}

auto Chip8SoundEmulation::getIsPlaying() const -> bool
{
    return m_isPlaying;
}

auto Chip8SoundEmulation::getFrequencyPtr() -> int*
{
    return &m_squareSoundFrequency;
}

auto Chip8SoundEmulation::getVolumePtr() -> float*
{
    return &m_volume;
}

auto Chip8SoundEmulation::getWaveType() const -> WaveType
{
    return m_waveType;
}

#include "Chip8SoundEmulation.h"

#include <SDL.h>
#include "../Chip8CoreBase/Chip8CoreBase.h"

// Thanks to https://gist.github.com/jacobsebek/10867cb10cdfccf1d6cfdd24fa23ee96 !!!

auto format(double sample, double amplitude) -> uint8
{
    return static_cast<uint8>(sample * Chip8SoundEmulation::MAX_AMPLITUDE * amplitude);
}

// Generate a sine wave
auto tone(double hz, unsigned long time) -> double
{
    return sin(time * hz * M_PI * 2 / Chip8SoundEmulation::FREQUENCY_RESOLUTION) * 0.5 + 0.5;
}

// Generate a sawtooth wave
auto saw(double hz, unsigned long time) -> double
{
    return 2 * (time * hz / Chip8SoundEmulation::FREQUENCY_RESOLUTION - floor(0.5 + time * hz / Chip8SoundEmulation::FREQUENCY_RESOLUTION));
}

// Generate a square wave
auto square(double hz, unsigned long time) -> double
{
    double sine = tone(hz, time);
    return sine > 0.5 ? 1.0 : 0.0;
}

Chip8SoundEmulation::Chip8SoundEmulation() : m_buffer{},
                                             m_spec{
                                                 .freq = FREQUENCY_RESOLUTION,
                                                 .format = AUDIO_U8,
                                                 .channels = 1,
                                                 .samples = SAMPLE_SIZE,
                                                 .callback = Chip8SoundEmulation::soundPlayerCallback,
                                                 .userdata = this,
                                             },
                                             m_bufferPosition(0),
                                             m_volume(DEFAULT_WAVE_AMPLITUDE),
                                             m_isPlaying(false),
                                             m_squareSoundFrequency(DEFAULT_WAVE_FREQUENCY),
                                             m_waveType(WaveType::Square)
{
    m_dev = SDL_OpenAudioDevice(nullptr, 0, &m_spec, nullptr, 0);

    initSoundBuffer(square);
}

Chip8SoundEmulation::~Chip8SoundEmulation()
{
    SDL_CloseAudioDevice(m_dev);
}

void Chip8SoundEmulation::reset()
{
    stop();
}

void Chip8SoundEmulation::setWaveType(WaveType waveType)
{
    stop();

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

void Chip8SoundEmulation::update(const std::unique_ptr<Chip8CoreBase>& chip8Core)
{
    const auto st = chip8Core->getCpu()->getST();

    if (st > 0)
    {
        play();
    }
    else
    {
        stop();
    }
}

void Chip8SoundEmulation::initSoundBuffer(const std::function<double(double, unsigned long)>& waveFunction)
{
    stop();

    for (int i = 0; i < BUFFER_LEN; i++)
    {
        m_buffer[i] = format(waveFunction(m_squareSoundFrequency, i), m_volume);
    }
}

void Chip8SoundEmulation::stop()
{
    if (m_isPlaying)
    {
        SDL_PauseAudioDevice(m_dev, 1);
        m_isPlaying = false;
    }
}

void Chip8SoundEmulation::play()
{
    if (!m_isPlaying)
    {
        SDL_PauseAudioDevice(m_dev, 0);
        m_isPlaying = true;
    }
}

void Chip8SoundEmulation::soundPlayerCallback(void* userdata, unsigned char* stream, int streamLength)
{
    auto* chip8SoundEmulation = static_cast<Chip8SoundEmulation*>(userdata);

    if (chip8SoundEmulation->m_bufferPosition + streamLength >= BUFFER_LEN)
    {
        chip8SoundEmulation->m_bufferPosition = 0;
    }

    for (int i = 0; i < streamLength; i++)
    {
        stream[i] = chip8SoundEmulation->m_buffer[chip8SoundEmulation->m_bufferPosition];
        chip8SoundEmulation->m_bufferPosition++;
    }
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

auto Chip8SoundEmulation::getBuffer() -> std::array<uint8, BUFFER_LEN>&
{
    return m_buffer;
}

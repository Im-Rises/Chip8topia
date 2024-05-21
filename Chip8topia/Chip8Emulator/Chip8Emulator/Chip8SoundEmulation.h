#pragma once

#include <array>
#include <memory>
#include <functional>
#include <binaryLib/binaryLib.h>
#include <SDL.h>

enum class WaveType : uint8
{
    Sine,
    Saw,
    Square
};

class Chip8CoreBase;
class Chip8SoundEmulation
{
public:
    static constexpr int FREQUENCY_RESOLUTION = 48000;
    static constexpr int SAMPLE_SIZE = 4096;
    static constexpr int BUFFER_DURATION = 1;
    static constexpr int BUFFER_LEN = BUFFER_DURATION * FREQUENCY_RESOLUTION;
    static constexpr int MAX_AMPLITUDE = 255;
    static constexpr int DEFAULT_WAVE_FREQUENCY = 440;
    static constexpr double DEFAULT_WAVE_AMPLITUDE = 0.5;

public:
    Chip8SoundEmulation();
    Chip8SoundEmulation(const Chip8SoundEmulation&) = delete;
    Chip8SoundEmulation(Chip8SoundEmulation&&) = delete;
    auto operator=(const Chip8SoundEmulation&) -> Chip8SoundEmulation& = delete;
    auto operator=(Chip8SoundEmulation&&) -> Chip8SoundEmulation& = delete;
    ~Chip8SoundEmulation();

public:
    void reset();
    void setWaveType(WaveType waveType);
    void update(const std::unique_ptr<Chip8CoreBase>& chip8Core);

private:
    void initSoundBuffer(const std::function<double(double, unsigned long)>& waveFunction);

    void stop();
    void play();

    static void soundPlayerCallback(void* userdata, unsigned char* stream, int streamLength);

public:
    [[nodiscard]] auto getIsPlaying() const -> bool;
    [[nodiscard]] auto getFrequencyPtr() -> int*;
    [[nodiscard]] auto getVolumePtr() -> float*;
    [[nodiscard]] auto getWaveType() const -> WaveType;
    [[nodiscard]] auto getBuffer() -> std::array<uint8, BUFFER_LEN>&;

private:
    SDL_AudioSpec m_spec;
    SDL_AudioDeviceID m_dev;
    std::array<uint8, BUFFER_LEN> m_buffer;
    int m_bufferPosition;
    int m_squareSoundFrequency;
    float m_volume;
    bool m_isPlaying;
    WaveType m_waveType;
};

#pragma once

#include <array>
#include <memory>
#include <functional>
#include <binaryLib/binaryLib.h>
#include <SDL.h>

// TODO: Need format to move code of SDL that shouldn't be here elsewhere...
// TODO: Check if a concurrency issue is present

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

private:
    static constexpr int BUFFER_DURATION = 1;
    static constexpr int BUFFER_LEN = BUFFER_DURATION * FREQUENCY_RESOLUTION;

public:
    Chip8SoundEmulation();
    Chip8SoundEmulation(const Chip8SoundEmulation&) = delete;
    Chip8SoundEmulation(Chip8SoundEmulation&&) = delete;
    auto operator=(const Chip8SoundEmulation&) -> Chip8SoundEmulation& = delete;
    auto operator=(Chip8SoundEmulation&&) -> Chip8SoundEmulation& = delete;
    ~Chip8SoundEmulation();

public:
    void initSoundBuffer(std::function<double(double, unsigned long)> waveFunction);
    void update(const std::unique_ptr<Chip8CoreBase>& chip8Core);
    void stop();
    void setWaveType(WaveType waveType);

private:
    static void soundPlayerCallback(void* userdata, unsigned char* stream, int streamLength);
    void soundPlayer(unsigned char* stream, int streamLength);

public:
    [[nodiscard]] auto getIsPlaying() const -> bool;
    [[nodiscard]] auto getFrequencyPtr() -> int*;
    [[nodiscard]] auto getVolumePtr() -> float*;
    [[nodiscard]] auto getWaveType() const -> WaveType;

private:
    SDL_AudioSpec m_spec;
    SDL_AudioDeviceID m_dev;
    std::array<Sint16, BUFFER_LEN> m_buffer;
    int m_bufferPosition;
    int m_squareSoundFrequency;
    float m_volume;
    bool m_isPlaying;
    WaveType m_waveType;
};

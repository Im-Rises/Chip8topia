#pragma once

#include <array>
#include <memory>
#include <SDL.h>

// TODO: Need format to move code of SDL that shouldn't be here elsewhere...

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
    void update(const std::unique_ptr<Chip8CoreBase>& chip8Core);
    void stop();

private:
    static void soundPlayerCallback(void* userdata, unsigned char* stream, int len);
    void soundPlayer(unsigned char* stream, int len);

private:
    SDL_AudioSpec m_spec;
    SDL_AudioDeviceID m_dev;
    std::array<Sint16, BUFFER_LEN> m_buffer;
    int m_bufferPosition;
    float m_volume;
    bool m_isPlaying;
};

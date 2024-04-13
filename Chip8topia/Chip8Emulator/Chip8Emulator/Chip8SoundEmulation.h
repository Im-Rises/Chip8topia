#pragma once

class Chip8CoreBase;
class Chip8SoundEmulation
{
private:
    static constexpr int FREQUENCY_RESOLUTION = 48000;

public:
    Chip8SoundEmulation();
    Chip8SoundEmulation(const Chip8SoundEmulation&) = delete;
    Chip8SoundEmulation(Chip8SoundEmulation&&) = delete;
    auto operator=(const Chip8SoundEmulation&) -> Chip8SoundEmulation& = delete;
    auto operator=(Chip8SoundEmulation&&) -> Chip8SoundEmulation& = delete;
    ~Chip8SoundEmulation() = default;

public:
    void update(Chip8CoreBase* chip8Core);
};

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <binaryLib/binaryLib.h>

#include "Core/Cpu.h"

class Chip8Core {
public:
    static constexpr unsigned int SCREEN_AND_TIMERS_FREQUENCY = 60;

public:
    Chip8Core();
    Chip8Core(const Chip8Core&) = delete;
    Chip8Core(Chip8Core&&) = delete;
    auto operator=(const Chip8Core&) -> Chip8Core& = delete;
    auto operator=(Chip8Core&&) -> Chip8Core& = delete;
    ~Chip8Core() = default;

public:
    void readRom(const std::vector<uint8>& rom);
    void clock();
    void updateKey(const uint8 key, const bool pressed);
    void reset();

public:
    auto getCpu() -> Cpu&;
    auto getPpu() -> std::shared_ptr<Ppu>;
    auto getInput() -> std::shared_ptr<Input>;

private:
    Cpu m_cpu;
    std::shared_ptr<Ppu> m_ppu;
    std::shared_ptr<Input> m_input;

    unsigned int m_clockCounter;
};

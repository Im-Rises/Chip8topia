#pragma once

#include <string>
#include <memory>

#include <binaryLib/binaryLib.h>

#include "Core/Cpu.h"

class Chip8Core {
public:
    Chip8Core();
    Chip8Core(const Chip8Core&) = delete;
    Chip8Core(Chip8Core&&) = delete;
    auto operator=(const Chip8Core&) -> Chip8Core& = delete;
    auto operator=(Chip8Core&&) -> Chip8Core& = delete;
    ~Chip8Core() = default;

public:
    void readRom(const uint8* rom, const size_t romSize);
    void clock();
    void updateKey(const uint8 key, const bool pressed);

private:
    Cpu m_cpu;
    std::shared_ptr<Ppu> m_ppu;
    std::shared_ptr<Input> m_input;
};

#pragma once

#include <binaryLib/binaryLib.h>
#include <array>
#include <string>
#include <bitset>
#include "../../Chip8Emulator/Chip8Core/Core/Cpu.h"

class Chip8Disassembler {
public:
    Chip8Disassembler() = default;
    Chip8Disassembler(const Chip8Disassembler&) = delete;
    Chip8Disassembler(Chip8Disassembler&&) = delete;
    auto operator=(const Chip8Disassembler&) -> Chip8Disassembler& = delete;
    auto operator=(Chip8Disassembler&&) -> Chip8Disassembler& = delete;
    ~Chip8Disassembler() = default;

public:
    void drawAssembly(const std::array<uint8, Cpu::MEMORY_SIZE>& memory, uint16 pc);

    void drawAssemblyControls();

private:
    std::bitset<Cpu::MEMORY_SIZE> m_breakpoints;
};

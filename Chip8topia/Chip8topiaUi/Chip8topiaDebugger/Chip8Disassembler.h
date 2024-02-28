#pragma once

#include <binaryLib/binaryLib.h>
#include <array>
#include <string>
#include <bitset>
#include "../../Chip8Emulator/ChipCores/Chip8Core/Core/Chip8Cpu.h"

class Chip8Disassembler {
public:
    Chip8Disassembler() = default;
    Chip8Disassembler(const Chip8Disassembler&) = delete;
    Chip8Disassembler(Chip8Disassembler&&) = delete;
    auto operator=(const Chip8Disassembler&) -> Chip8Disassembler& = delete;
    auto operator=(Chip8Disassembler&&) -> Chip8Disassembler& = delete;
    ~Chip8Disassembler() = default;

public:
    void drawDisassembly(const std::array<uint8, Chip8Cpu::MEMORY_SIZE>& memory, uint16 pc);

    void drawDisassemblyControls();

private:
    void clearBreakpoints();

private:
    std::bitset<Chip8Cpu::MEMORY_SIZE> m_breakpoints;
    bool m_followPC = true;
    uint16 m_previousPC = 0;
};

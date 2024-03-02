#pragma once

#include <binaryLib/binaryLib.h>
#include <string>
#include "../../Chip8Emulator/ChipCores/Chip8Core/Core/Chip8Cpu.h"

class Chip8Emulator;
class Chip8Disassembler {
public:
    Chip8Disassembler() = default;
    Chip8Disassembler(const Chip8Disassembler&) = delete;
    Chip8Disassembler(Chip8Disassembler&&) = delete;
    auto operator=(const Chip8Disassembler&) -> Chip8Disassembler& = delete;
    auto operator=(Chip8Disassembler&&) -> Chip8Disassembler& = delete;
    ~Chip8Disassembler() = default;

public:
    void drawDisassembly(Chip8Emulator* emulator);
    void drawDisassemblyControls(Chip8Emulator* emulator);
    void drawBreakpoints(Chip8Emulator* emulator);

private:
    bool m_followPC = true;
    uint16 m_previousPC = 0;

    bool m_requestMoveToPC = false;
    uint16 m_requestedPC = 0;
};

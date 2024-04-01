#pragma once

#include <binaryLib/binaryLib.h>
#include <string>
#include "../../Chip8Emulator/ChipCores/Chip8Core/Core/Chip8Cpu.h"

class Chip8Emulator;
class Chip8topiaDisassembler
{
public:
    Chip8topiaDisassembler() = default;
    Chip8topiaDisassembler(const Chip8topiaDisassembler&) = delete;
    Chip8topiaDisassembler(Chip8topiaDisassembler&&) = delete;
    auto operator=(const Chip8topiaDisassembler&) -> Chip8topiaDisassembler& = delete;
    auto operator=(Chip8topiaDisassembler&&) -> Chip8topiaDisassembler& = delete;
    ~Chip8topiaDisassembler() = default;

public:
    void drawDisassembly(Chip8Emulator* emulator);
    void drawDisassemblyControls(Chip8Emulator* emulator);
    void drawBreakpoints(Chip8Emulator* emulator);

private:
    void requestMoveToPc(uint16 pc);
    void requestMoveToNextPc();

    void setScollToPc(uint16 pc);

private:
    bool m_followPc = true;
    uint16 m_previousPc = 0;

    bool m_requestMoveToPc = false;
    uint16 m_requestedPc = 0;

    bool m_requestMoveToNextPc = false;
};

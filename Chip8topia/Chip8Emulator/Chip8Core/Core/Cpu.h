#pragma once

#include "binaryLib/binaryLib.h"
#include <array>

class Cpu {
public:
    static constexpr uint16 START_ADDRESS = 0x200;
    static constexpr uint16 FONTSET_START_ADDRESS = 0x50;
    static constexpr uint16 FONTSET_SIZE = 80;

public:
    Cpu();
    Cpu(const Cpu& other) = delete;
    Cpu(Cpu&& other) = delete;
    auto operator=(const Cpu& other) -> Cpu& = delete;
    auto operator=(Cpu&& other) -> Cpu& = delete;
    ~Cpu() = default;

public:
    void clock();

private:
    void computeOpcode(uint16 opcode);
    uint16 fetchOpcode();

    void clockTimers();

private:
    struct OpcodeDecoder {
        uint16 opcode;
        uint16 id;
    };

    // Should I use a list of switch case instade of decoding the opcode by trying every possible opcode ?
    std::array<OpcodeDecoder, 35> m_opcodeDecoders{

    };

    std::array<uint8, 0x1000> m_memory;
    uint16 m_pc;

    std::array<uint16, 16> m_stack;
    uint8 m_sp;

    std::array<uint8, 16> m_V;
    uint16 m_I;

    uint8 gameTimer;
    uint8 audioTimer;
};

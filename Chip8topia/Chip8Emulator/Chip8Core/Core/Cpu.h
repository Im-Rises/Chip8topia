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
    void clockTimers();

private:
    uint16 fetchOpcode();
    void computeOpcode(uint16 opcode);

    void CLS();
    void RET();
    void SYS(uint16 address);

    void JP(uint16 address);
    void CALL(uint16 address);
    void SE(uint8 Vx, uint8 byte);
    void SNE(uint8 Vx, uint8 byte);
    void SE(uint8 Vx, uint8 Vy);
    void LD(uint8 Vx, uint8 byte);
    void ADD(uint8 Vx, uint8 byte);

    void LD(uint8 Vx, uint8 Vy);
    void OR(uint8 Vx, uint8 Vy);
    void AND(uint8 Vx, uint8 Vy);
    void XOR(uint8 Vx, uint8 Vy);
    void ADD(uint8 Vx, uint8 Vy);
    void SUB(uint8 Vx, uint8 Vy);
    void SHR(uint8 Vx, uint8 Vy);
    void SUBN(uint8 Vx, uint8 Vy);
    void SHL(uint8 Vx, uint8 Vy);

    void SNE(uint8 Vx, uint8 Vy);
    void LD(uint16 address);
    void JP(uint8 V0, uint16 address);
    void RND(uint8 Vx, uint8 byte);
    void DRW(uint8 Vx, uint8 Vy, uint8 nibble);

    void SKP(uint8 Vx);
    void SKNP(uint8 Vx);

    void LD(uint8 Vx, uint8 DT);
    void LD(uint8 Vx);

    void LD(uint8 DT, uint8 Vx);
    void LD(uint8 ST, uint8 Vx);
    void ADD(uint8 I, uint8 Vx);

    void LD(uint8 F, uint8 Vx);
    void LD(uint8 B, uint8 Vx);
    void LD(uint8 I, uint8 Vx);
    void LD(uint8 Vx, uint8 I);

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

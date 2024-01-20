#pragma once

#include "binaryLib/binaryLib.h"
#include <array>
#include <memory>

class Ppu;

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
    auto fetchOpcode() -> uint16;
    void computeOpcode(const uint16 opcode);

    inline void CLS();
    inline void RET();
    inline void SYS(const uint16 address);
    inline void JP(const uint16 address);
    inline void CALL(const uint16 address);
    inline void SE_Vx_byte(const uint8 x, const uint8 byte);
    inline void SNE_Vx_byte(const uint8 x, const uint8 byte);
    inline void SE_Vx_Vy(const uint8 x, const uint8 y);
    inline void LD_Vx_byte(const uint8 x, const uint8 byte);
    inline void ADD_Vx_byte(const uint8 x, const uint8 byte);
    inline void LD_Vx_Vy(const uint8 x, const uint8 y);
    inline void OR_Vx_Vy(const uint8 x, const uint8 y);
    inline void AND_Vx_Vy(const uint8 x, const uint8 y);
    inline void XOR_Vx_Vy(const uint8 x, const uint8 y);
    inline void ADD_Vx_Vy(const uint8 x, const uint8 y);
    inline void SUB_Vx_Vy(const uint8 x, const uint8 y);
    inline void SHR_Vx_Vy(const uint8 x, const uint8 y); // TODO: Check if Vy is used
    inline void SUBN_Vx_Vy(const uint8 x, const uint8 y);
    inline void SHL_Vx_Vy(const uint8 x, const uint8 y); // TODO: Check if Vy is used
    inline void SNE_Vx_Vy(const uint8 x, const uint8 y);
    inline void LD_I_addr(const uint16 address);
    inline void JP_V0_addr(const uint16 address);
    inline void RND_Vx_byte(const uint8 x, const uint8 byte);
    inline void DRW_Vx_Vy_nibble(const uint8 x, const uint8 y, const uint8 nibble);
    inline void SKP_Vx(const uint8 x);
    inline void SKNP_Vx(const uint8 x);
    inline void LD_Vx_DT(const uint8 x);
    inline void LD_Vx_K(const uint8 x);
    inline void LD_DT_Vx(const uint8 x);
    inline void LD_ST_Vx(const uint8 x);
    inline void ADD_I_Vx(const uint8 x);
    inline void LD_F_Vx(const uint8 x);
    inline void LD_B_Vx(const uint8 x);
    inline void LD_I_Vx(const uint8 x);
    inline void LD_Vx_I(const uint8 x);

private:
    std::array<uint8, 0x1000> m_memory;
    uint16 m_pc;

    std::array<uint16, 16> m_stack;
    uint8 m_sp;

    std::array<uint8, 16> m_V;
    uint16 m_I;

    uint8 gameTimer;
    uint8 audioTimer;

    std::shared_ptr<Ppu> m_ppu;
};

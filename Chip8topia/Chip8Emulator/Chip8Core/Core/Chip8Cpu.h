#pragma once

#include "../../Chip8CoreBase/Core/CpuBase.h"


class Chip8Cpu final : public CpuBase {
public:
    Chip8Cpu();
    Chip8Cpu(const Chip8Cpu& other) = delete;
    Chip8Cpu(Chip8Cpu&& other) = delete;
    auto operator=(const Chip8Cpu& other) -> Chip8Cpu& = delete;
    auto operator=(Chip8Cpu&& other) -> Chip8Cpu& = delete;
    ~Chip8Cpu() final = default;

public:
    void reset() final;
    void requestDisableHalt() { m_requestDisableHalt = true; }

private:
    void computeOpcode(const uint16 opcode) final;

    void SYS(const uint16 address);
    void OR_Vx_Vy(const uint8 x, const uint8 y) final;
    void AND_Vx_Vy(const uint8 x, const uint8 y) final;
    void XOR_Vx_Vy(const uint8 x, const uint8 y) final;
    void SHR_Vx_Vy(const uint8 x, const uint8 y);
    void SUBN_Vx_Vy(const uint8 x, const uint8 y);
    void SHL_Vx_Vy(const uint8 x, const uint8 y);
    void JP_V0_addr(const uint16 address);
    void DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) final;
    void LD_aI_Vx(const uint8 x) final;
    void LD_Vx_aI(const uint8 x) final;

private:
    bool m_isHalted;
    bool m_requestDisableHalt;
};

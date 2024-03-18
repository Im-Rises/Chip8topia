#pragma once

#include "../../../Chip8CoreBase/Core/CpuBase.h"


class Chip8Cpu final : public CpuBase
{
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

    void OR_Vx_Vy(const uint8 x, const uint8 y) final;                   // 8xy1
    void AND_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xy2
    void XOR_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xy3
    void DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) final; // Dxyn
    void LD_aI_Vx(const uint8 x) final;                                  // Fx55
    void LD_Vx_aI(const uint8 x) final;                                  // Fx65

private:
    bool m_isHalted;
    bool m_requestDisableHalt;
};

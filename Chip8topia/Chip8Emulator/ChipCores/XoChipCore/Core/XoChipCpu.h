#pragma once

#include "../../../Chip8CoreBase/Core/CpuBase.h"

class XoChipPpu;
class XoChipCpu final : public CpuBase
{
public:
    XoChipCpu();
    XoChipCpu(const XoChipCpu&) = delete;
    XoChipCpu(XoChipCpu&&) = delete;
    auto operator=(const XoChipCpu&) -> XoChipCpu& = delete;
    auto operator=(XoChipCpu&&) -> XoChipCpu& = delete;
    ~XoChipCpu() final = default;

public:
    void reset() final;

private:
    void computeOpcode(const uint16 opcode) final;

    void SCR(const uint8 n) final;                                       // 00FB
    void SCL(const uint8 n) final;                                       // 00FC
    void LORES() final;                                                  // 00FE
    void HIRES() final;                                                  // 00FF
    void SE_Vx_nn(const uint8 x, const uint8 nn) final;                  // 3xnn
    void SNE_Vx_nn(const uint8 x, const uint8 nn) final;                 // 4xnn
    void SE_Vx_Vy(const uint8 x, const uint8 y) final;                   // 5xy0
    void SV_RNG_Vx_Vy(uint8 x, uint8 y);                                 // 5xy2
    void LD_RNG_Vx_Vy(uint8 x, uint8 y);                                 // 5xy3
    void SNE_Vx_Vy(const uint8 x, const uint8 y) final;                  // 9xy0
    void DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) final; // Dxyn
    void SKP_Vx(const uint8 x) final;                                    // Ex9E
    void SKNP_Vx(const uint8 x) final;                                   // ExA1
    void LD_I_NNNN();                                                    // F000
    void SET_PLN(const uint8 x);                                         // FX01
    void LD_AUDIO_aI();                                                  // F002
    void SET_PITCH_x(const uint8 x);                                     // FX3A
    void LD_aI_Vx(const uint8 x) final;                                  // Fx55
    void LD_Vx_aI(const uint8 x) final;                                  // Fx65
};

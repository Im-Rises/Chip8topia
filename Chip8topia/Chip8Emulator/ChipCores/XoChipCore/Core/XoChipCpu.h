#pragma once

#include "../../../Chip8CoreBase/Core/CpuBase.h"

class XoChipPpu;
class XoChipCpu final : public CpuBase {
public:
    XoChipCpu();
    XoChipCpu(const XoChipCpu&) = delete;
    XoChipCpu(XoChipCpu&&) = delete;
    auto operator=(const XoChipCpu&) -> XoChipCpu& = delete;
    auto operator=(XoChipCpu&&) -> XoChipCpu& = delete;
    ~XoChipCpu() final = default;

    void setPpu(std::shared_ptr<PpuBase> ppu) final;

public:
    void reset() final;

private:
    void computeOpcode(const uint16 opcode) final;

    void EXIT();                                                         // 00FD
    void SCD(const uint8 n);                                             // 00CN
    void SCU(const uint8 n);                                             // 00DN
    void SCR(const uint8 n);                                             // 00FB
    void SCL(const uint8 n);                                             // 00FC
    void LORES();                                                        // 00FE
    void HIRES();                                                        // 00FF
    void SV_RNG_Vx_Vy(const uint8 x, const uint8 y);                     // 5xy2
    void LD_RNG_Vx_Vy(const uint8 x, const uint8 y);                     // 5xy3
    void OR_Vx_Vy(const uint8 x, const uint8 y) final;                   // 8xy1
    void AND_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xy2
    void XOR_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xy3
    void LD_I_NNNN();                                                    // F000
    void SET_PLN(const uint8 x);                                         // FX01
    void LD_AUDIO_aI();                                                  // F002
    void SHR_Vx_Vy(const uint8 x, const uint8 y);                        // 8xy6
    void SUBN_Vx_Vy(const uint8 x, const uint8 y);                       // 8xy7
    void SHL_Vx_Vy(const uint8 x, const uint8 y);                        // 8xyE
    void JP_V0_addr(const uint16 address);                               // Bxnn
    void DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) final; // Dxyn
    void LD_HF_Vx(const uint8 x);                                        // Fx30
    void SET_PITCH_x(const uint8 x);                                     // FX3A
    void LD_aI_Vx(const uint8 x) final;                                  // Fx55
    void LD_Vx_aI(const uint8 x) final;                                  // Fx65
    void LD_R_Vx(const uint8 x);                                         // Fx75
    void LD_Vx_R(const uint8 x);                                         // Fx85

private:
    XoChipPpu* m_ppuCasted;
    std::array<uint8, REGISTER_V_SIZE> m_savedV;
};

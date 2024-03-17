#pragma once

#include "../../../Chip8CoreBase/Core/CpuBase.h"

class SChipCPpu;
class SChipCCpu final : public CpuBase
{
public:
    SChipCCpu();
    SChipCCpu(const SChipCCpu&) = delete;
    SChipCCpu(SChipCCpu&&) = delete;
    auto operator=(const SChipCCpu&) -> SChipCCpu& = delete;
    auto operator=(SChipCCpu&&) -> SChipCCpu& = delete;
    ~SChipCCpu() final = default;

    void setPpu(std::shared_ptr<PpuBase> ppu) final;

public:
    void reset() final;

private:
    void computeOpcode(const uint16 opcode) final;

    void SCD(const uint8 n);                                             // 00CN
    void SCR(const uint8 n);                                             // 00FB
    void SCL(const uint8 n);                                             // 00FC
    void LORES();                                                        // 00FE
    void HIRES();                                                        // 00FF
    void OR_Vx_Vy(const uint8 x, const uint8 y) final;                   // 8xy1
    void AND_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xy2
    void XOR_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xy3
    void DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) final; // Dxyn
    void LD_aI_Vx(const uint8 x) final;                                  // Fx55
    void LD_Vx_aI(const uint8 x) final;                                  // Fx65
    void LD_R_Vx(const uint8 x) final;                                   // Fx75
    void LD_Vx_R(const uint8 x) final;                                   // Fx85

private:
    SChipCPpu* m_ppuCasted;
};

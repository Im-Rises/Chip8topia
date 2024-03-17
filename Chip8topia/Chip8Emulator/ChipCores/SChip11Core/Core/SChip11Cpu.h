#pragma once

#include "../../../Chip8CoreBase/Core/CpuBase.h"

class SChip11Ppu;
class SChip11Cpu final : public CpuBase
{
public:
    explicit SChip11Cpu(bool isModernMode);
    SChip11Cpu(const SChip11Cpu&) = delete;
    SChip11Cpu(SChip11Cpu&&) = delete;
    auto operator=(const SChip11Cpu&) -> SChip11Cpu& = delete;
    auto operator=(SChip11Cpu&&) -> SChip11Cpu& = delete;
    ~SChip11Cpu() final = default;

    void setPpu(std::shared_ptr<PpuBase> ppu) final;

public:
    void reset() final;
    void requestDisableHalt() { m_requestDisableHalt = true; }

    [[nodiscard]] auto getIsModernMode() const -> bool { return m_isModernMode; }

private:
    void computeOpcode(const uint16 opcode) final;

    void SCD(const uint8 n);                                             // 00CN
    void SCR(const uint8 n);                                             // 00FB
    void SCL(const uint8 n);                                             // 00FC
    void LORES();                                                        // 00FE
    void HIRES();                                                        // 00FF
    void LD_aI_Vx(const uint8 x) final;                                  // Fx55
    void LD_Vx_aI(const uint8 x) final;                                  // Fx65
    void SHR_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xy6
    void SHL_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xyE
    void DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) final; // Dxyn
    void LD_R_Vx(const uint8 x) final;                                   // Fx75
    void LD_Vx_R(const uint8 x) final;                                   // Fx85

private:
    SChip11Ppu* m_ppuCasted;
    bool m_isHalted;
    bool m_requestDisableHalt;
    bool m_isModernMode;
};

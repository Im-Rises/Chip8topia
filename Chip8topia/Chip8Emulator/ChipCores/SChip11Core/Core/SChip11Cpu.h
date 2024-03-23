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

public:
    void reset() final;
    void requestDisableHalt() { m_requestDisableHalt = true; }

    [[nodiscard]] auto getIsModernMode() const -> bool { return m_isModernMode; }

private:
    void computeOpcode(const uint16 opcode) final;

    void SCD(const uint8 n) final;                                       // 00CN
    void SCR(const uint8 n) final;                                       // 00FB
    void SCL(const uint8 n) final;                                       // 00FC
    void SHR_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xy6
    void SHL_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xyE
    void DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) final; // Dxyn
    void LD_R_Vx(const uint8 x) final;                                   // Fx75
    void LD_Vx_R(const uint8 x) final;                                   // Fx85

private:
    bool m_isHalted;
    bool m_requestDisableHalt;
    bool m_isModernMode;
};

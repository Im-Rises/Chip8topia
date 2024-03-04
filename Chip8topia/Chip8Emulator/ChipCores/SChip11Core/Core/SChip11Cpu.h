#pragma once

#include "../../../Chip8CoreBase/Core/CpuBase.h"

class SChip11Ppu;
class SChip11Cpu final : public CpuBase {
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

    void EXIT();                                                         // 00FD
    void OR_Vx_Vy(const uint8 x, const uint8 y) final;                   // 8xy1
    void AND_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xy2
    void XOR_Vx_Vy(const uint8 x, const uint8 y) final;                  // 8xy3
    void DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) final; // Dxyn
    void LD_aI_Vx(const uint8 x) final;                                  // Fx55
    void LD_Vx_aI(const uint8 x) final;                                  // Fx65
    void SHR_Vx_Vy(const uint8 x, const uint8 y);                        // 8xy6
    void SUBN_Vx_Vy(const uint8 x, const uint8 y);                       // 8xy7
    void SHL_Vx_Vy(const uint8 x, const uint8 y);                        // 8xyE

    void SCD(const uint8 n);                              // 00CN
    void SCR(const uint8 n);                              // 00FB
    void SCL(const uint8 n);                              // 00FC
    void LORES();                                         // 00FE
    void HIRES();                                         // 00FF
    void JP_Vx_addr(const uint8 x, const uint16 address); // Bxnn
    void LD_HF_Vx(const uint8 x);                         // Fx30
    void LD_R_Vx(const uint8 x);                          // Fx75
    void LD_Vx_R(const uint8 x);                          // Fx85

private:
    // TODO: Change to something else
    //    SChip11Ppu* m_ppuCasted;

    std::array<uint8, REGISTER_V_SIZE> m_savedV;

    bool m_isHalted;
    bool m_requestDisableHalt;
    bool m_isModernMode;
};

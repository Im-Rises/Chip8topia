#include "SChip11Cpu.h"

#include "SChip11Ppu.h"

SChip11Cpu::SChip11Cpu(bool isModernMode) : m_isModernMode(isModernMode), m_isHalted(false), m_requestDisableHalt(false)
{
    std::copy(SCHIP_FONTSET.begin(), SCHIP_FONTSET.end(), m_memory.begin());
}

void SChip11Cpu::reset()
{
    CpuBase::reset();
    std::copy(SCHIP_FONTSET.begin(), SCHIP_FONTSET.end(), m_memory.begin());
    m_isHalted = false;
    m_requestDisableHalt = false;
}

void SChip11Cpu::computeOpcode(const uint16 opcode)
{
    const uint8 nibble4 = (opcode & 0xF000) >> 12;
    const uint8 nibble3 = (opcode & 0x0F00) >> 8;
    const uint8 nibble2 = (opcode & 0x00F0) >> 4;
    const uint8 nibble1 = (opcode & 0x000F);

    switch (nibble4)
    {
    case 0x0:
    {
        switch (nibble2)
        {
        case 0xC: SCD(nibble1); break; // 00CN
        case 0xE:
        {
            switch (nibble1)
            {
            case 0x0: CLS(); break; // 00E0
            case 0xE: RET(); break; // 00EE
            }
            break;
        }
        case 0xF:
        {
            switch (nibble1)
            {
            case 0xB: SCR(nibble1); break; // 00FB
            case 0xC: SCL(nibble1); break; // 00FC
            case 0xD: EXIT(); break;       // 00FD
            case 0xE: LORES(); break;      // 00FE
            case 0xF: HIRES(); break;      // 00FF
            }
            break;
        }
        }
        break;
    }

    case 0x1: JP_addr(opcode & 0x0FFF); break;            // 1NNN
    case 0x2: CALL_addr(opcode & 0x0FFF); break;          // 2NNN
    case 0x3: SE_Vx_nn(nibble3, opcode & 0x00FF); break;  // 3XNN
    case 0x4: SNE_Vx_nn(nibble3, opcode & 0x00FF); break; // 4XNN
    case 0x5: SE_Vx_Vy(nibble3, nibble2); break;          // 5XY0
    case 0x6: LD_Vx_nn(nibble3, opcode & 0x00FF); break;  // 6XNN
    case 0x7: ADD_Vx_nn(nibble3, opcode & 0x00FF); break; // 7XNN
    case 0x8:
    {
        switch (nibble1)
        {
        case 0x0: LD_Vx_Vy(nibble3, nibble2); break;   // 8XY0
        case 0x1: OR_Vx_Vy(nibble3, nibble2); break;   // 8XY1
        case 0x2: AND_Vx_Vy(nibble3, nibble2); break;  // 8XY2
        case 0x3: XOR_Vx_Vy(nibble3, nibble2); break;  // 8XY3
        case 0x4: ADD_Vx_Vy(nibble3, nibble2); break;  // 8XY4
        case 0x5: SUB_Vx_Vy(nibble3, nibble2); break;  // 8XY5
        case 0x6: SHR_Vx_Vy(nibble3, nibble2); break;  // 8XY6
        case 0x7: SUBN_Vx_Vy(nibble3, nibble2); break; // 8XY7
        case 0xE: SHL_Vx_Vy(nibble3, nibble2); break;  // 8XYE
        }
        break;
    }
    case 0x9: SNE_Vx_Vy(nibble3, nibble2); break;            // 9XY0
    case 0xA: LD_I_addr(opcode & 0x0FFF); break;             // ANNN
    case 0xB: JP_xnn_Vx(opcode & 0x0FFF, nibble3); break;    // BXNN
    case 0xC: RND_Vx_nn(nibble3, opcode & 0x00FF); break;    // CXNN
    case 0xD: DRW_Vx_Vy_n(nibble3, nibble2, nibble1); break; // Dxyn
    case 0xE:
    {
        switch (nibble1)
        {
        case 0xE: SKP_Vx(nibble3); break;  // EX9E
        case 0x1: SKNP_Vx(nibble3); break; // EXA1
        }
        break;
    }
    case 0xF:
    {
        switch (nibble2)
        {
        case 0x0:
        {
            switch (nibble1)
            {
            case 0x7: LD_Vx_DT(nibble3); break; // FX07
            case 0xA: LD_Vx_K(nibble3); break;  // FX0A
            }
            break;
        }
        case 0x1:
            switch (nibble1)
            {
            case 0x5: LD_DT_Vx(nibble3); break; // FX15
            case 0x8: LD_ST_Vx(nibble3); break; // FX18
            case 0xE: ADD_I_Vx(nibble3); break; // FX1E
            }
            break;
        case 0x2: LD_F_Vx(nibble3); break; // FX29
        case 0x3:
        {
            switch (nibble1)
            {
            case 0x0: LD_HF_Vx(nibble3); break; // FX30
            case 0x3: LD_B_Vx(nibble3); break;  // FX33
            }
            break;
        }
        case 0x5: LD_aI_Vx(nibble3); break; // FX55
        case 0x6: LD_Vx_aI(nibble3); break; // FX65
        case 0x7: LD_R_Vx(nibble3); break;  // FX75
        case 0x8: LD_Vx_R(nibble3); break;  // FX85
        }
        break;
    }
    }
}

void SChip11Cpu::SCD(const uint8 n)
{
    CpuBase::SCD((m_ppu->getMode() == PpuBase::PpuMode::LORES && !m_isModernMode) ? n / 2 : n);
}

void SChip11Cpu::SCR(const uint8 n)
{
    CpuBase::SCR((m_ppu->getMode() == PpuBase::PpuMode::LORES && !m_isModernMode) ? 2 : 4);
}

void SChip11Cpu::SCL(const uint8 n)
{
    CpuBase::SCL((m_ppu->getMode() == PpuBase::PpuMode::LORES && !m_isModernMode) ? 2 : 4);
}

void SChip11Cpu::SHR_Vx_Vy(const uint8 x, const uint8 y)
{
    const uint8 flag = m_V[x] & 0x1;
    m_V[x] >>= 1;
    m_V[0xF] = flag;
}

void SChip11Cpu::SHL_Vx_Vy(const uint8 x, const uint8 y)
{
    const uint8 flag = (m_V[x] & 0x80) >> 7;
    m_V[x] <<= 1;
    m_V[0xF] = flag;
}

void SChip11Cpu::DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n)
{
    if (m_ppu->getMode() == PpuBase::PpuMode::LORES && !m_isModernMode)
    {
        m_isHalted = true;

        if (m_requestDisableHalt)
        {
            m_isHalted = false;
            m_requestDisableHalt = false;
        }

        if (m_isHalted)
        {
            m_pc -= 2;
            return;
        }
    }

    m_V[0xF] = static_cast<uint8>(m_ppu->drawSprite(m_V[x], m_V[y], n, m_memory, m_I));
}

void SChip11Cpu::LD_R_Vx(const uint8 x)
{
    CpuBase::LD_R_Vx(x > 7 ? 7 : x);
}

void SChip11Cpu::LD_Vx_R(const uint8 x)
{
    CpuBase::LD_Vx_R(x > 7 ? 7 : x);
}

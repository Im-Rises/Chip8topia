#include "SChipCCpu.h"

#include "SChipCPpu.h"

#define TRIGGER_COMPUTE_OPCODE_ERROR(opcode) TRIGGER_EMULATION_ERROR("SChipCCpu::computeOpcode: Invalid opcode 0x{:04X}", opcode)

SChipCCpu::SChipCCpu()
{
    std::copy(SCHIP_FONTSET.begin(), SCHIP_FONTSET.end(), m_memory.begin());
}

void SChipCCpu::reset()
{
    CpuBase::reset();
    std::copy(SCHIP_FONTSET.begin(), SCHIP_FONTSET.end(), m_memory.begin());
}

void SChipCCpu::computeOpcode(const uint16 opcode)
{
    const uint8 nibble4 = (opcode & 0xF000) >> 12;
    const uint8 nibble3 = (opcode & 0x0F00) >> 8;
    const uint8 nibble2 = (opcode & 0x00F0) >> 4;
    const uint8 nibble1 = (opcode & 0x000F);

    switch (nibble4)
    {
    case 0x0:
    {
        switch (opcode)
        {
        case 0x00E0: CLS(); break;        // 00E0
        case 0x00EE: RET(); break;        // 00EE
        case 0x00FB: SCR(nibble1); break; // 00FB
        case 0x00FC: SCL(nibble1); break; // 00FC
        case 0x00FD: EXIT(); break;       // 00FD
        case 0x00FE: LORES(); break;      // 00FE
        case 0x00FF: HIRES(); break;      // 00FF
        default:
        {
            switch (opcode & 0xFFF0)
            {
            case 0x00C0: SCD(nibble1); break; // 00CN
            default: TRIGGER_COMPUTE_OPCODE_ERROR(opcode); break;
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
    case 0x5:
    {
        switch (nibble1)
        {
        case 0x0: SE_Vx_Vy(nibble3, nibble2); break; // 5XY0
        default: TRIGGER_COMPUTE_OPCODE_ERROR(opcode); break;
        }
        break;
    }
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
        default: TRIGGER_COMPUTE_OPCODE_ERROR(opcode); break;
        }
        break;
    }
    case 0x9:
    {
        switch (nibble1)
        {
        case 0x0: SNE_Vx_Vy(nibble3, nibble2); break; // 9XY0
        default: TRIGGER_COMPUTE_OPCODE_ERROR(opcode); break;
        }
        break;
    }
    case 0xA: LD_I_addr(opcode & 0x0FFF); break;             // ANNN
    case 0xB: JP_nnn_V0(opcode & 0x0FFF); break;             // BNNN
    case 0xC: RND_Vx_nn(nibble3, opcode & 0x00FF); break;    // CXNN
    case 0xD: DRW_Vx_Vy_n(nibble3, nibble2, nibble1); break; // DXYN
    case 0xE:
    {
        switch (opcode & 0x00FF)
        {
        case 0x9E: SKP_Vx(nibble3); break;  // EX9E
        case 0xA1: SKNP_Vx(nibble3); break; // EXA1
        default: TRIGGER_COMPUTE_OPCODE_ERROR(opcode); break;
        }
        break;
    }
    case 0xF:
    {
        switch (opcode & 0x00FF)
        {
        case 0x07: LD_Vx_DT(nibble3); break; // FX07
        case 0x0A: LD_Vx_K(nibble3); break;  // FX0A
        case 0x15: LD_DT_Vx(nibble3); break; // FX15
        case 0x18: LD_ST_Vx(nibble3); break; // FX18
        case 0x1E: ADD_I_Vx(nibble3); break; // FX1E
        case 0x29: LD_F_Vx(nibble3); break;  // FX29
        case 0x30: LD_HF_Vx(nibble3); break; // FX30
        case 0x33: LD_B_Vx(nibble3); break;  // FX33
        case 0x55: LD_aI_Vx(nibble3); break; // FX55
        case 0x65: LD_Vx_aI(nibble3); break; // FX65
        case 0x75: LD_R_Vx(nibble3); break;  // FX75
        case 0x85: LD_Vx_R(nibble3); break;  // FX85
        default: TRIGGER_COMPUTE_OPCODE_ERROR(opcode); break;
        }
    }
    default: TRIGGER_COMPUTE_OPCODE_ERROR(opcode); break;
    }
}

void SChipCCpu::SCD(const uint8 n)
{
    CpuBase::SCD(m_ppu->getMode() == PpuBase::PpuMode::LORES ? n / 2 : n);
}

void SChipCCpu::SCR(const uint8 n)
{
    CpuBase::SCR(m_ppu->getMode() == PpuBase::PpuMode::LORES ? 2 : 4);
}

void SChipCCpu::SCL(const uint8 n)
{
    CpuBase::SCL(m_ppu->getMode() == PpuBase::PpuMode::LORES ? 2 : 4);
}

void SChipCCpu::LORES()
{
    m_ppu->clearAllPlanes();
    CpuBase::LORES();
}
void SChipCCpu::HIRES()
{
    m_ppu->clearAllPlanes();
    CpuBase::HIRES();
}

void SChipCCpu::OR_Vx_Vy(const uint8 x, const uint8 y)
{
    CpuBase::OR_Vx_Vy(x, y);
    m_V[0xF] = 0;
}

void SChipCCpu::AND_Vx_Vy(const uint8 x, const uint8 y)
{
    CpuBase::AND_Vx_Vy(x, y);
    m_V[0xF] = 0;
}

void SChipCCpu::XOR_Vx_Vy(const uint8 x, const uint8 y)
{
    CpuBase::XOR_Vx_Vy(x, y);
    m_V[0xF] = 0;
}

void SChipCCpu::DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n)
{
    m_V[0xF] = static_cast<uint8>(m_ppu->drawSprite(m_V[x], m_V[y], n, m_memory, m_I));
}

void SChipCCpu::LD_aI_Vx(const uint8 x)
{
    CpuBase::LD_aI_Vx(x);
    m_I += x + 1;
}

void SChipCCpu::LD_Vx_aI(const uint8 x)
{
    CpuBase::LD_Vx_aI(x);
    m_I += x + 1;
}

void SChipCCpu::LD_R_Vx(const uint8 x)
{
    CpuBase::LD_R_Vx(x > 7 ? 7 : x);
}

void SChipCCpu::LD_Vx_R(const uint8 x)
{
    CpuBase::LD_Vx_R(x > 7 ? 7 : x);
}

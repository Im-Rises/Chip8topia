#include "Chip8Cpu.h"

#include "Chip8Ppu.h"

Chip8Cpu::Chip8Cpu() : m_isHalted(false),
                       m_requestDisableHalt(false)
{
    std::copy(CHIP8_FONTSET.begin(), CHIP8_FONTSET.end(), m_memory.begin());
}

void Chip8Cpu::reset()
{
    CpuBase::reset();
    std::copy(CHIP8_FONTSET.begin(), CHIP8_FONTSET.end(), m_memory.begin());
    m_isHalted = false;
    m_requestDisableHalt = false;
}

void Chip8Cpu::computeOpcode(const uint16 opcode)
{
    const uint8 nibble4 = (opcode & 0xF000) >> 12;
    const uint8 nibble3 = (opcode & 0x0F00) >> 8;
    const uint8 nibble2 = (opcode & 0x00F0) >> 4;
    const uint8 nibble1 = (opcode & 0x000F);

    switch (nibble4)
    {
    case 0x0:
    {
        switch (nibble1)
        {
        case 0x0: CLS(); break;               // 00E0
        case 0xE: RET(); break;               // 00EE
        default: SYS(opcode & 0x0FFF); break; // 0NNN
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
    case 0xB: JP_nnn_V0(opcode & 0x0FFF); break;             // BNNN
    case 0xC: RND_Vx_nn(nibble3, opcode & 0x00FF); break;    // CXNN
    case 0xD: DRW_Vx_Vy_n(nibble3, nibble2, nibble1); break; // DXYN
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
        {
            switch (nibble1)
            {
            case 0x5: LD_DT_Vx(nibble3); break; // FX15
            case 0x8: LD_ST_Vx(nibble3); break; // FX18
            case 0xE: ADD_I_Vx(nibble3); break; // FX1E
            }
            break;
        }
        case 0x2: LD_F_Vx(nibble3); break;  // FX29
        case 0x3: LD_B_Vx(nibble3); break;  // FX33
        case 0x5: LD_aI_Vx(nibble3); break; // FX55
        case 0x6: LD_Vx_aI(nibble3); break; // FX65
        }
        break;
    }
    }
}

void Chip8Cpu::OR_Vx_Vy(const uint8 x, const uint8 y)
{
    CpuBase::OR_Vx_Vy(x, y);
    m_V[0xF] = 0;
}

void Chip8Cpu::AND_Vx_Vy(const uint8 x, const uint8 y)
{
    CpuBase::AND_Vx_Vy(x, y);
    m_V[0xF] = 0;
}

void Chip8Cpu::XOR_Vx_Vy(const uint8 x, const uint8 y)
{
    CpuBase::XOR_Vx_Vy(x, y);
    m_V[0xF] = 0;
}

void Chip8Cpu::DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n)
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

    m_V[0xF] = static_cast<uint8>(m_ppu->drawSprite(m_V[x], m_V[y], n, m_memory, m_I));
}

void Chip8Cpu::LD_aI_Vx(const uint8 x)
{
    CpuBase::LD_aI_Vx(x);
    m_I += x + 1;
}

void Chip8Cpu::LD_Vx_aI(const uint8 x)
{
    CpuBase::LD_Vx_aI(x);
    m_I += x + 1;
}

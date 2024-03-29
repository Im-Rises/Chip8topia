#include "XoChipCpu.h"

#include "XoChipPpu.h"

#include "../../../Chip8CoreBase/Core/Input.h"

XoChipCpu::XoChipCpu()
{
    std::copy(XO_CHIP_FONTSET.begin(), XO_CHIP_FONTSET.end(), m_memory.begin());
}

void XoChipCpu::reset()
{
    CpuBase::reset();
    std::copy(XO_CHIP_FONTSET.begin(), XO_CHIP_FONTSET.end(), m_memory.begin());
}

void XoChipCpu::computeOpcode(const uint16 opcode)
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
        case 0xD: SCU(nibble1); break; // 00DN
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
    case 0x5:
    {
        switch (nibble1)
        {
        case 0x0: SE_Vx_Vy(nibble3, nibble2); break;     // 5XY0
        case 0x2: SV_RNG_Vx_Vy(nibble3, nibble2); break; // 5XY2
        case 0x3: LD_RNG_Vx_Vy(nibble3, nibble2); break; // 5XY3
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
        }
        break;
    }
    case 0x9: SNE_Vx_Vy(nibble3, nibble2); break;            // 9XY0
    case 0xA: LD_I_addr(opcode & 0x0FFF); break;             // ANNN
    case 0xB: JP_nnn_V0(opcode & 0x0FFF); break;             // BNNN
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
            case 0x0: LD_I_NNNN(); break;       // F000
            case 0x1: SET_PLN(nibble3); break;  // FX01
            case 0x2: LD_AUDIO_aI(); break;     // F002
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
            case 0x0: LD_HF_Vx(nibble3); break;    // FX30
            case 0x3: LD_B_Vx(nibble3); break;     // FX33
            case 0xA: SET_PITCH_x(nibble3); break; // FX3A
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

void XoChipCpu::LORES()
{
    m_ppu->clearScreen();
    CpuBase::LORES();
}

void XoChipCpu::HIRES()
{
    m_ppu->clearScreen();
    CpuBase::HIRES();
}

void XoChipCpu::SE_Vx_nn(const uint8 x, const uint8 nn)
{
    if (m_V[x] == nn)
    {
        readNextWord() == 0xF000 ? m_pc += 4 : m_pc += 2;
    }
}

void XoChipCpu::SNE_Vx_nn(const uint8 x, const uint8 nn)
{
    if (m_V[x] != nn)
    {
        readNextWord() == 0xF000 ? m_pc += 4 : m_pc += 2;
    }
}

void XoChipCpu::SE_Vx_Vy(const uint8 x, const uint8 y)
{
    if (m_V[x] == m_V[y])
    {
        readNextWord() == 0xF000 ? m_pc += 4 : m_pc += 2;
    }
}

void XoChipCpu::SV_RNG_Vx_Vy(const uint8 x, const uint8 y)
{
    // TODO:Add a check if y is greater than x
    int range = y - x;
    for (int i = 0; i <= range; i++)
    {
        m_memory[m_I + i] = m_V[x + i];
    }
}

void XoChipCpu::LD_RNG_Vx_Vy(const uint8 x, const uint8 y)
{
    // TODO:Add a check if y is greater than x
    int range = y - x;
    for (int i = 0; i <= range; i++)
    {
        m_V[x + i] = m_memory[m_I + i];
    }
}

void XoChipCpu::SNE_Vx_Vy(const uint8 x, const uint8 y)
{
    if (m_V[x] != m_V[y])
    {
        readNextWord() == 0xF000 ? m_pc += 4 : m_pc += 2;
    }
}

void XoChipCpu::DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n)
{
    m_V[0xF] = static_cast<uint8>(m_ppu->drawSprite(m_V[x], m_V[y], n, m_memory, m_I));
}

void XoChipCpu::SKP_Vx(const uint8 x)
{
    if (m_input->isKeyPressed(m_V[x]))
    {
        readNextWord() == 0xF000 ? m_pc += 4 : m_pc += 2;
    }
}
void XoChipCpu::SKNP_Vx(const uint8 x)
{
    if (!m_input->isKeyPressed(m_V[x]))
    {
        readNextWord() == 0xF000 ? m_pc += 4 : m_pc += 2;
    }
}

void XoChipCpu::LD_I_NNNN()
{
    m_I = fetchWord();
}

void XoChipCpu::SET_PLN(const uint8 x)
{
    m_ppu->setPlane(x & 0b11);
}

void XoChipCpu::LD_AUDIO_aI()
{
    // TODO: Implement xo chip sound
}

void XoChipCpu::SET_PITCH_x(const uint8 x)
{
    // TODO: Implement xo chip sound
}

void XoChipCpu::LD_aI_Vx(const uint8 x)
{
    CpuBase::LD_aI_Vx(x);
    m_I += x + 1;
}

void XoChipCpu::LD_Vx_aI(const uint8 x)
{
    CpuBase::LD_Vx_aI(x);
    m_I += x + 1;
}

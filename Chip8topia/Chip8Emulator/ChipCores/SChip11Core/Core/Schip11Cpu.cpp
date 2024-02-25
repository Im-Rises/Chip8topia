#include "Schip11Cpu.h"

#include "Schip11Ppu.h"

Schip11Cpu::Schip11Cpu() : m_ppuCasted(dynamic_cast<Schip11Ppu*>(m_ppu.get())) {
}

void Schip11Cpu::reset() {
    CpuBase::reset();
}

void Schip11Cpu::computeOpcode(const uint16 opcode) {
    const uint8 nibble4 = (opcode & 0xF000) >> 12;
    const uint8 nibble3 = (opcode & 0x0F00) >> 8;
    const uint8 nibble2 = (opcode & 0x00F0) >> 4;
    const uint8 nibble1 = (opcode & 0x000F);

    switch (nibble4)
    {
    case 0x0: {
        switch (nibble2)
        {
        case 0xC: SCD(nibble1); break;
        case 0xE: {
            switch (nibble1)
            {
            case 0x0: CLS(); break;
            case 0xE: RET(); break;
            }
            break;
        }
        case 0xF: {
            switch (nibble1)
            {
            case 0xB: SCR(nibble1); break;
            case 0xC: SCL(nibble1); break;
            case 0xD: EXIT(); break;
            case 0xE: LORES(); break;
            case 0xF: HIRES(); break;
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
    case 0x8: {
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
        default: /* Invalid opcode */ break;
        }
        break;
    }
    case 0x9: SNE_Vx_Vy(nibble3, nibble2); break;            // 9XY0
    case 0xA: LD_I_addr(opcode & 0x0FFF); break;             // ANNN
    case 0xB: JP_Vx_addr(nibble3, opcode & 0x0FFF); break;   // Bxnn
    case 0xC: RND_Vx_nn(nibble3, opcode & 0x00FF); break;    // CXNN
    case 0xD: DRW_Vx_Vy_n(nibble3, nibble2, nibble1); break; // Dxyn
    case 0xE: {
        switch (nibble1)
        {
        case 0xE: SKP_Vx(nibble3); break;  // EX9E
        case 0x1: SKNP_Vx(nibble3); break; // EXA1
        }
        break;
    }
    case 0xF: {
        switch (nibble2)
        {
        case 0x0:
            switch (nibble1)
            {
            case 0x7: LD_Vx_DT(nibble3); break; // FX07
            case 0xA: LD_Vx_K(nibble3); break;  // FX0A
            default: break;
            }
            break;
        case 0x1:
            switch (nibble1)
            {
            case 0x5: LD_DT_Vx(nibble3); break; // FX15
            case 0x8: LD_ST_Vx(nibble3); break; // FX18
            case 0xE: ADD_I_Vx(nibble3); break; // FX1E
            default: break;
            }
            break;
        case 0x2: LD_F_Vx(nibble3); break; // FX29
        case 0x3: {
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
        default: /* Invalid opcode */ break;
        }
        break;
    }
    }
}

void Schip11Cpu::EXIT() {
    m_pc -= 2;
}

void Schip11Cpu::OR_Vx_Vy(const uint8 x, const uint8 y) {
    m_V[x] |= m_V[y];
}

void Schip11Cpu::AND_Vx_Vy(const uint8 x, const uint8 y) {
    m_V[x] &= m_V[y];
}

void Schip11Cpu::XOR_Vx_Vy(const uint8 x, const uint8 y) {
    m_V[x] ^= m_V[y];
}

void Schip11Cpu::DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) {
    m_V[0xF] = static_cast<uint8>(m_ppu->drawSprite(m_V[x], m_V[y], n, m_memory, m_I));
}

void Schip11Cpu::LD_aI_Vx(const uint8 x) {
    for (int i = 0; i <= x; i++)
    {
        m_memory[m_I + i] = m_V[i];
    }
}

void Schip11Cpu::LD_Vx_aI(const uint8 x) {
    for (int i = 0; i <= x; i++)
    {
        m_V[i] = m_memory[m_I + i];
    }
}

void Schip11Cpu::SHR_Vx_Vy(const uint8 x, const uint8 y) {
    const uint8 flag = m_V[x] & 0x1;
    m_V[x] >>= 1;
    m_V[0xF] = flag;
}

void Schip11Cpu::SUBN_Vx_Vy(const uint8 x, const uint8 y) {
    const auto flag = static_cast<uint8>(m_V[y] >= m_V[x]);
    m_V[x] = m_V[y] - m_V[x];
    m_V[0xF] = flag;
}

void Schip11Cpu::SHL_Vx_Vy(const uint8 x, const uint8 y) {
    const uint8 flag = (m_V[x] & 0x80) >> 7;
    m_V[x] <<= 1;
    m_V[0xF] = flag;
}

void Schip11Cpu::SCD(const uint8 n) {
    m_ppuCasted->scrollDown(n);
}

void Schip11Cpu::SCR(const uint8 n) {
    m_ppuCasted->scrollRight(n);
}

void Schip11Cpu::SCL(const uint8 n) {
    m_ppuCasted->scrollLeft(n);
}

void Schip11Cpu::LORES() {
    m_ppu->setMode(PpuBase::PpuMode::LORES);
}

void Schip11Cpu::HIRES() {
    m_ppu->setMode(PpuBase::PpuMode::HIRES);
}

void Schip11Cpu::JP_Vx_addr(const uint8 x, const uint16 address) {
    m_pc = m_V[x] + address;
}

void Schip11Cpu::LD_HF_Vx(const uint8 x) {
    // TODO: Check if this is correct
    m_I = m_V[x] * 10;
}

void Schip11Cpu::LD_R_Vx(const uint8 x) {
    for (int i = 0; i <= x; i++)
    {
        m_savedV[i] = m_V[i];
    }
}

void Schip11Cpu::LD_Vx_R(const uint8 x) {
    for (int i = 0; i <= x; i++)
    {
        m_V[i] = m_savedV[i];
    }
}

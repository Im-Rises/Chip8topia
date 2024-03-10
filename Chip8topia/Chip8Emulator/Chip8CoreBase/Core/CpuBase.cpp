#include "CpuBase.h"

#include "PpuBase.h"
#include "Input.h"

CpuBase::CpuBase() : m_pc(START_ADDRESS),
                     m_sp(0),
                     m_I(0),
                     m_DT(0),
                     m_ST(0),
                     m_memory{},
                     m_V{},
                     m_stack{},
                     m_u8NumberRandomGenerator(0, 0xFF) {
}

void CpuBase::reset() {
    m_pc = START_ADDRESS;
    m_sp = 0;
    m_I = 0;
    m_DT = 0;
    m_ST = 0;
    //    m_memory = {};
    //    std::copy(FONTSET.begin(), FONTSET.end(), m_memory.begin());
    m_V = {};
    m_stack = {};
}

void CpuBase::readRom(const std::vector<uint8>& rom) {
    for (int i = 0; i < rom.size(); i++)
    {
        m_memory[START_ADDRESS + i] = rom[i];
    }
}

void CpuBase::clock() {
    computeOpcode(fetchOpcode());
}

void CpuBase::clockTimers() {
    if (m_DT > 0)
    {
        m_DT--;
    }

    if (m_ST > 0)
    {
        m_ST--;
    }
}

auto CpuBase::fetchOpcode() -> uint16 {
    const uint16 opcode = (m_memory[m_pc] << 8) | (m_memory[m_pc + 1]);
    m_pc += 2;
    return opcode;
}

void CpuBase::CLS() {
    m_ppu->clearScreen();
}

void CpuBase::RET() {
    m_pc = m_stack[--m_sp];
}

void CpuBase::JP_addr(const uint16 addr) {
    m_pc = addr;
}

void CpuBase::CALL_addr(const uint16 addr) {
    m_stack[m_sp++] = m_pc;
    m_pc = addr;
}

void CpuBase::SE_Vx_nn(const uint8 x, const uint8 nn) {
    if (m_V[x] == nn)
    {
        m_pc += 2;
    }
}

void CpuBase::SNE_Vx_nn(const uint8 x, const uint8 nn) {
    if (m_V[x] != nn)
    {
        m_pc += 2;
    }
}

void CpuBase::SE_Vx_Vy(const uint8 x, const uint8 y) {
    if (m_V[x] == m_V[y])
    {
        m_pc += 2;
    }
}

void CpuBase::LD_Vx_nn(const uint8 x, const uint8 nn) {
    m_V[x] = nn;
}

void CpuBase::ADD_Vx_nn(const uint8 x, const uint8 nn) {
    m_V[x] += nn;
}

void CpuBase::LD_Vx_Vy(const uint8 x, const uint8 y) {
    m_V[x] = m_V[y];
}

void CpuBase::ADD_Vx_Vy(const uint8 x, const uint8 y) {
    m_V[x] += m_V[y];
    m_V[0xF] = static_cast<uint8>(m_V[x] < m_V[y]);
}

void CpuBase::SUB_Vx_Vy(const uint8 x, const uint8 y) {
    const auto flag = static_cast<uint8>(m_V[x] >= m_V[y]);
    m_V[x] -= m_V[y];
    m_V[0xF] = flag;
}

void CpuBase::SNE_Vx_Vy(const uint8 x, const uint8 y) {
    if (m_V[x] != m_V[y])
    {
        m_pc += 2;
    }
}

void CpuBase::LD_I_addr(const uint16 addr) {
    m_I = addr;
}

void CpuBase::RND_Vx_nn(const uint8 x, const uint8 nn) {
    m_V[x] = m_u8NumberRandomGenerator.generateRandomNumber() & nn;
}

void CpuBase::SKP_Vx(const uint8 x) {
    if (m_input->isKeyPressed(m_V[x]))
    {
        m_pc += 2;
    }
}

void CpuBase::SKNP_Vx(const uint8 x) {
    if (!m_input->isKeyPressed(m_V[x]))
    {
        m_pc += 2;
    }
}

void CpuBase::LD_Vx_DT(const uint8 x) {
    m_V[x] = m_DT;
}

void CpuBase::LD_Vx_K(const uint8 x) {
    if (m_input->isAnyKeyPressed())
    {
        m_V[x] = x;
    }
    else
    {
        m_pc -= 2;
    }
}
void CpuBase::LD_DT_Vx(const uint8 x) {
    m_DT = m_V[x];
}

void CpuBase::LD_ST_Vx(const uint8 x) {
    m_ST = m_V[x];
}

void CpuBase::ADD_I_Vx(const uint8 x) {
    m_I += m_V[x];
    m_V[0xF] = static_cast<uint8>(m_I < m_V[x]);
}

void CpuBase::LD_F_Vx(const uint8 x) {
    m_I = m_V[x] * 5;
}

void CpuBase::LD_B_Vx(const uint8 x) {
    m_memory[m_I] = m_V[x] / 100;
    m_memory[m_I + 1] = (m_V[x] / 10) % 10;
    m_memory[m_I + 2] = (m_V[x] % 100) % 10;
}

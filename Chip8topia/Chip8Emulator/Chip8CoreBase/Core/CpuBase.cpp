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
                     m_savedV{},
                     m_stack{},
                     m_u8NumberRandomGenerator(0, 0xFF)
{
}

#if defined(BUILD_PARAM_SAFE)
void CpuBase::setErrorCallback(const std::function<void(const std::string&)>& errorCallback)
{
    m_errorCallback = errorCallback;
}
#endif

void CpuBase::reset()
{
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

void CpuBase::readRom(const std::vector<uint8>& rom)
{
    for (int i = 0; i < rom.size(); i++)
    {
        m_memory[START_ADDRESS + i] = rom[i];
    }
}

void CpuBase::clock()
{
    computeOpcode(fetchOpcode());
}

void CpuBase::clockTimers()
{
    if (m_DT > 0)
    {
        m_DT--;
    }

    if (m_ST > 0)
    {
        m_ST--;
    }
}

auto CpuBase::fetchOpcode() -> uint16
{
    return fetchWord();
}

auto CpuBase::fetchWord() -> uint16
{
    const uint16 word = (m_memory[m_pc] << 8) | (m_memory[m_pc + 1]);
    m_pc += 2;
    return word;
}

auto CpuBase::readNextWord() -> uint16
{
    return (m_memory[m_pc] << 8) | (m_memory[m_pc + 1]);
}

void CpuBase::SCD(const uint8 n)
{
    m_ppu->scrollDown(n);
}

void CpuBase::SCU(const uint8 n)
{
    m_ppu->scrollUp(n);
}

void CpuBase::CLS()
{
    m_ppu->clearScreen();
}

void CpuBase::RET()
{
    m_pc = m_stack[--m_sp];
}

void CpuBase::SCR(const uint8 n)
{
    m_ppu->scrollRight(n);
}

void CpuBase::SCL(const uint8 n)
{
    m_ppu->scrollLeft(n);
}

void CpuBase::EXIT()
{
    m_pc -= 2;
    // TODO Call the error callback here and return (exit the program)
}

void CpuBase::LORES()
{
    m_ppu->setMode(PpuBase::PpuMode::LORES);
}

void CpuBase::HIRES()
{
    m_ppu->setMode(PpuBase::PpuMode::HIRES);
}

void CpuBase::SYS(const uint16 /*address*/)
{
    // This opcode is only used on the old computers on which Chip-8 was originally implemented.
    // It is ignored by modern interpreters.
    m_pc += 2;
}

void CpuBase::JP_addr(const uint16 addr)
{
    m_pc = addr;
}

void CpuBase::CALL_addr(const uint16 addr)
{
    m_stack[m_sp++] = m_pc;
    m_pc = addr;
}

void CpuBase::SE_Vx_nn(const uint8 x, const uint8 nn)
{
    if (m_V[x] == nn)
    {
        m_pc += 2;
    }
}

void CpuBase::SNE_Vx_nn(const uint8 x, const uint8 nn)
{
    if (m_V[x] != nn)
    {
        m_pc += 2;
    }
}

void CpuBase::SE_Vx_Vy(const uint8 x, const uint8 y)
{
    if (m_V[x] == m_V[y])
    {
        m_pc += 2;
    }
}

void CpuBase::LD_Vx_nn(const uint8 x, const uint8 nn)
{
    m_V[x] = nn;
}

void CpuBase::ADD_Vx_nn(const uint8 x, const uint8 nn)
{
    m_V[x] += nn;
}

void CpuBase::LD_Vx_Vy(const uint8 x, const uint8 y)
{
    m_V[x] = m_V[y];
}

void CpuBase::OR_Vx_Vy(const uint8 x, const uint8 y)
{
    m_V[x] |= m_V[y];
}

void CpuBase::AND_Vx_Vy(const uint8 x, const uint8 y)
{
    m_V[x] &= m_V[y];
}

void CpuBase::XOR_Vx_Vy(const uint8 x, const uint8 y)
{
    m_V[x] ^= m_V[y];
}

void CpuBase::ADD_Vx_Vy(const uint8 x, const uint8 y)
{
    m_V[x] += m_V[y];
    m_V[0xF] = static_cast<uint8>(m_V[x] < m_V[y]);
}

void CpuBase::SUB_Vx_Vy(const uint8 x, const uint8 y)
{
    const auto flag = static_cast<uint8>(m_V[x] >= m_V[y]);
    m_V[x] -= m_V[y];
    m_V[0xF] = flag;
}

void CpuBase::SHR_Vx_Vy(const uint8 x, const uint8 y)
{
    const uint8 flag = m_V[y] & 0x1;
    m_V[x] = m_V[y] >> 1;
    m_V[0xF] = flag;
}

void CpuBase::SUBN_Vx_Vy(const uint8 x, const uint8 y)
{
    const auto flag = static_cast<uint8>(m_V[y] >= m_V[x]);
    m_V[x] = m_V[y] - m_V[x];
    m_V[0xF] = flag;
}

void CpuBase::SHL_Vx_Vy(const uint8 x, const uint8 y)
{
    const uint8 flag = (m_V[x] & 0x80) >> 7;
    m_V[x] = m_V[y] << 1;
    m_V[0xF] = flag;
}

void CpuBase::SNE_Vx_Vy(const uint8 x, const uint8 y)
{
    if (m_V[x] != m_V[y])
    {
        m_pc += 2;
    }
}

void CpuBase::LD_I_addr(const uint16 addr)
{
    m_I = addr;
}

void CpuBase::JP_nnn_V0(const uint16 address)
{
    m_pc = address + m_V[0];
}

void CpuBase::JP_xnn_Vx(const uint16 address, const uint8 x)
{
    m_pc = address + m_V[x];
}

void CpuBase::RND_Vx_nn(const uint8 x, const uint8 nn)
{
    m_V[x] = m_u8NumberRandomGenerator.generateRandomNumber() & nn;
}

void CpuBase::SKP_Vx(const uint8 x)
{
    if (m_input->isKeyPressed(m_V[x]))
    {
        m_pc += 2;
    }
}

void CpuBase::SKNP_Vx(const uint8 x)
{
    if (!m_input->isKeyPressed(m_V[x]))
    {
        m_pc += 2;
    }
}

void CpuBase::LD_Vx_DT(const uint8 x)
{
    m_V[x] = m_DT;
}

void CpuBase::LD_Vx_K(const uint8 x)
{
    int key = m_input->getKeyWaitReleased();
    if (key != -1)
    {
        m_V[x] = static_cast<uint8>(key);
    }
    else
    {
        m_pc -= 2;
    }
}
void CpuBase::LD_DT_Vx(const uint8 x)
{
    m_DT = m_V[x];
}

void CpuBase::LD_ST_Vx(const uint8 x)
{
    m_ST = m_V[x];
}

void CpuBase::ADD_I_Vx(const uint8 x)
{
    m_I += m_V[x];
}

void CpuBase::LD_F_Vx(const uint8 x)
{
    m_I = m_V[x] * 5;
}

void CpuBase::LD_HF_Vx(const uint8 x)
{
    m_I = (m_V[x] * 10) + 0x50;
}

void CpuBase::LD_B_Vx(const uint8 x)
{
    m_memory[m_I] = m_V[x] / 100;
    m_memory[m_I + 1] = (m_V[x] / 10) % 10;
    m_memory[m_I + 2] = (m_V[x] % 100) % 10;
}

void CpuBase::LD_aI_Vx(const uint8 x)
{
    for (int i = 0; i <= x; i++)
    {
        m_memory[m_I + i] = m_V[i];
    }
}

void CpuBase::LD_Vx_aI(const uint8 x)
{
    for (int i = 0; i <= x; i++)
    {
        m_V[i] = m_memory[m_I + i];
    }
}

void CpuBase::LD_R_Vx(const uint8 x)
{
    for (int i = 0; i <= x; i++)
    {
        m_savedV[i] = m_V[i];
    }
}

void CpuBase::LD_Vx_R(const uint8 x)
{
    for (int i = 0; i <= x; i++)
    {
        m_V[i] = m_savedV[i];
    }
}

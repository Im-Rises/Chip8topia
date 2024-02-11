#include "Cpu.h"

#include <random>
#include <utility>

#include "Input.h"
#include "Ppu.h"

Cpu::Cpu() : m_pc(START_ADDRESS),
             m_sp(0),
             m_I(0),
             m_DT(0),
             m_ST(0),
             m_memory{},
             m_V{},
             m_stack{},
             m_isHalted(false),
             m_requestDisableHalt(false) {
    std::copy(FONTSET.begin(), FONTSET.end(), m_memory.begin());
}

void Cpu::setPpu(std::shared_ptr<Ppu> ppu) {
    m_ppu = std::move(ppu);
}

void Cpu::setInput(std::shared_ptr<Input> input) {
    m_input = std::move(input);
}

void Cpu::reset() {
    m_pc = START_ADDRESS;
    m_sp = 0;
    m_I = 0;
    m_DT = 0;
    m_ST = 0;
    m_memory = {}; // TODO: Maybe for the ram we can reset everything except the rom location so it can be reloaded
    std::copy(FONTSET.begin(), FONTSET.end(), m_memory.begin());
    m_V = {};
    m_stack = {};
}

void Cpu::readRom(const std::vector<uint8>& rom) {
    for (int i = 0; i < rom.size(); i++)
    {
        m_memory[START_ADDRESS + i] = rom[i];
    }
}

void Cpu::clock() {
    computeOpcode(fetchOpcode());
}

void Cpu::clockTimers() {
    if (m_DT > 0)
    {
        m_DT--;
    }

    if (m_ST > 0)
    {
        m_ST--;
    }
}

auto Cpu::fetchOpcode() -> uint16 {
    const uint16 opcode = (m_memory[m_pc] << 8) | (m_memory[m_pc + 1]);
    m_pc += 2;
    return opcode;
}

void Cpu::computeOpcode(const uint16 opcode) {
    // TODO: Maybe add a security when reaching an unknown opcode

    const uint8 nibble4 = (opcode & 0xF000) >> 12;
    const uint8 nibble3 = (opcode & 0x0F00) >> 8;
    const uint8 nibble2 = (opcode & 0x00F0) >> 4;
    const uint8 nibble1 = (opcode & 0x000F);

    switch (nibble4)
    {
    case 0x0: {
        switch (nibble1)
        {
        case 0x0: CLS(); break;               // 00E0
        case 0xE: RET(); break;               // 00EE
        default: SYS(opcode & 0x0FFF); break; // 0NNN
        }
        break;
    }
    case 0x1: JP(opcode & 0x0FFF); break;                 // 1NNN
    case 0x2: CALL(opcode & 0x0FFF); break;               // 2NNN
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
    case 0xB: JP_V0_addr(opcode & 0x0FFF); break;            // BNNN
    case 0xC: RND_Vx_nn(nibble3, opcode & 0x00FF); break;    // CXNN
    case 0xD: DRW_Vx_Vy_n(nibble3, nibble2, nibble1); break; // DXYN
    case 0xE: {
        switch (nibble1)
        {
        case 0xE: SKP_Vx(nibble3); break;  // EX9E
        case 0x1: SKNP_Vx(nibble3); break; // EXA1
        default: /* Invalid opcode */ break;
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
            case 0xA: LD_Vx_x(nibble3); break;  // FX0A
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
        case 0x3: LD_B_Vx(nibble3); break; // FX33
        case 0x5: LD_I_Vx(nibble3); break; // FX55
        case 0x6: LD_Vx_I(nibble3); break; // FX65
        default: /* Invalid opcode */ break;
        }
        break;
    }
    }
}

void Cpu::CLS() {
    m_ppu->clearScreen();
}

void Cpu::RET() {
    m_pc = m_stack[--m_sp];
}

void Cpu::SYS(const uint16 /*address*/) {
    // This opcode is only used on the old computers on which Chip-8 was originally implemented.
    // It is ignored by modern interpreters.
    m_pc += 2;
}

void Cpu::JP(const uint16 address) {
    m_pc = address;
}

void Cpu::CALL(const uint16 address) {
    m_stack[m_sp++] = m_pc;
    m_pc = address;
}

void Cpu::SE_Vx_nn(const uint8 x, const uint8 nn) {
    if (m_V[x] == nn)
    {
        m_pc += 2;
    }
}

void Cpu::SNE_Vx_nn(const uint8 x, const uint8 nn) {
    if (m_V[x] != nn)
    {
        m_pc += 2;
    }
}

void Cpu::SE_Vx_Vy(const uint8 x, const uint8 y) {
    if (m_V[x] == m_V[y])
    {
        m_pc += 2;
    }
}

void Cpu::LD_Vx_nn(const uint8 x, const uint8 nn) {
    m_V[x] = nn;
}

void Cpu::ADD_Vx_nn(const uint8 x, const uint8 nn) {
    m_V[x] += nn;
}

void Cpu::LD_Vx_Vy(const uint8 x, const uint8 y) {
    m_V[x] = m_V[y];
}

void Cpu::OR_Vx_Vy(const uint8 x, const uint8 y) {
    m_V[x] |= m_V[y];
    m_V[0xF] = 0;
}

void Cpu::AND_Vx_Vy(const uint8 x, const uint8 y) {
    m_V[x] &= m_V[y];
    m_V[0xF] = 0;
}

void Cpu::XOR_Vx_Vy(const uint8 x, const uint8 y) {
    m_V[x] ^= m_V[y];
    m_V[0xF] = 0;
}

void Cpu::ADD_Vx_Vy(const uint8 x, const uint8 y) {
    m_V[x] += m_V[y];
    m_V[0xF] = static_cast<uint8>(m_V[x] < m_V[y]);
}

void Cpu::SUB_Vx_Vy(const uint8 x, const uint8 y) {
    const auto flag = static_cast<uint8>(m_V[x] >= m_V[y]);
    m_V[x] -= m_V[y];
    m_V[0xF] = flag;
}

void Cpu::SHR_Vx_Vy(const uint8 x, const uint8 y) {
    const uint8 flag = m_V[y] & 0x1;
    m_V[x] = m_V[y] >> 1;
    m_V[0xF] = flag;
}

void Cpu::SUBN_Vx_Vy(const uint8 x, const uint8 y) {
    const auto flag = static_cast<uint8>(m_V[y] >= m_V[x]);
    m_V[x] = m_V[y] - m_V[x];
    m_V[0xF] = flag;
}

void Cpu::SHL_Vx_Vy(const uint8 x, const uint8 y) {
    const uint8 flag = (m_V[x] & 0x80) >> 7;
    m_V[x] = m_V[y] << 1;
    m_V[0xF] = flag;
}

void Cpu::SNE_Vx_Vy(const uint8 x, const uint8 y) {
    if (m_V[x] != m_V[y])
    {
        m_pc += 2;
    }
}

void Cpu::LD_I_addr(const uint16 address) {
    m_I = address;
}

void Cpu::JP_V0_addr(const uint16 address) {
    m_pc = m_V[0] + address;
}

auto generateRandomNumber(int min, int max) -> int {
    // TODO: Maybe move this to a separate class and stop initializing the random number generator every time

    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(min, max);

    // Generate and return a random number within the specified range
    return distribution(gen);
}

void Cpu::RND_Vx_nn(const uint8 x, const uint8 nn) {
    m_V[x] = generateRandomNumber(0, 255) & nn; // TODO: Check the value can reach 0 to 255 inclusive
}

void Cpu::DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) {
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

void Cpu::SKP_Vx(const uint8 x) {
    if (m_input->isKeyPressed(m_V[x]))
    {
        m_pc += 2;
    }
}

void Cpu::SKNP_Vx(const uint8 x) {
    if (!m_input->isKeyPressed(m_V[x]))
    {
        m_pc += 2;
    }
}

void Cpu::LD_Vx_DT(const uint8 x) {
    m_V[x] = m_DT;
}

void Cpu::LD_Vx_x(const uint8 x) {
    if (m_input->isAnyKeyPressed())
    {
        m_V[x] = x;
    }
    else
    {
        m_pc -= 2;
    }
}

void Cpu::LD_DT_Vx(const uint8 x) {
    m_DT = m_V[x];
}

void Cpu::LD_ST_Vx(const uint8 x) {
    m_ST = m_V[x];
}

void Cpu::ADD_I_Vx(const uint8 x) {
    m_I += m_V[x];
    m_V[0xF] = static_cast<uint8>(m_I < m_V[x]);
}

void Cpu::LD_F_Vx(const uint8 x) {
    m_I = m_V[x] * 5;
}

void Cpu::LD_B_Vx(const uint8 x) {
    m_memory[m_I] = m_V[x] / 100;
    m_memory[m_I + 1] = (m_V[x] / 10) % 10;
    m_memory[m_I + 2] = (m_V[x] % 100) % 10;
}

void Cpu::LD_I_Vx(const uint8 x) {
    for (int i = 0; i <= x; i++)
    {
        m_memory[m_I + i] = m_V[i];
    }
    m_I += x + 1;
}

void Cpu::LD_Vx_I(const uint8 x) {
    for (int i = 0; i <= x; i++)
    {
        m_V[i] = m_memory[m_I + i];
    }
    m_I += x + 1;
}

#include "Cpu.h"

Cpu::Cpu() : m_pc(START_ADDRESS),
             m_sp(0),
             m_I(0),
             gameTimer(0),
             audioTimer(0) {
    m_stack.fill(0);
    m_V.fill(0);
    m_memory.fill(0);
}

void Cpu::clock() {
    //    const uint16 opcode = fetchOpcode();
    //    computeOpcode(opcode);
    //    clockTimers();
}

void Cpu::computeOpcode(uint16 opcode) {
}

uint16 Cpu::fetchOpcode() {
    const uint16 opcode = (m_memory[m_pc] << 8) | (m_memory[m_pc + 1]);
    m_pc += 2;
    return opcode;
}

void Cpu::clockTimers() {
    if (gameTimer > 0)
    {
        gameTimer--;
    }
    if (audioTimer > 0)
    {
        audioTimer--;
    }
}

#include "XoChipCpu.h"

#include "XoChipPpu.h"

XoChipCpu::XoChipCpu() : m_savedV{} {
    //    std::copy(XO_FONTSET.begin(), XO_FONTSET.end(), m_memory.begin());
}

void XoChipCpu::setPpu(std::shared_ptr<PpuBase> ppu) {
    CpuBase::setPpu(ppu);
    m_ppuCasted = dynamic_cast<XoChipPpu*>(ppu.get());
}

void XoChipCpu::reset() {
    CpuBase::reset();
    m_savedV.fill(0);
}

void XoChipCpu::computeOpcode(const uint16 opcode) {
    const uint8 nibble4 = (opcode & 0xF000) >> 12;
    const uint8 nibble3 = (opcode & 0x0F00) >> 8;
    const uint8 nibble2 = (opcode & 0x00F0) >> 4;
    const uint8 nibble1 = (opcode & 0x000F);
}

void XoChipCpu::EXIT() {
}

void XoChipCpu::SCD(const uint8 n) {
}

void XoChipCpu::SCU(const uint8 n) {
}

void XoChipCpu::SCR(const uint8 n) {
}

void XoChipCpu::SCL(const uint8 n) {
}

void XoChipCpu::LORES() {
}

void XoChipCpu::HIRES() {
}

void XoChipCpu::SV_RNG_Vx_Vy(const uint8 x, const uint8 y) {
}

void XoChipCpu::LD_RNG_Vx_Vy(const uint8 x, const uint8 y) {
}

void XoChipCpu::OR_Vx_Vy(const uint8 x, const uint8 y) {
}

void XoChipCpu::AND_Vx_Vy(const uint8 x, const uint8 y) {
}

void XoChipCpu::XOR_Vx_Vy(const uint8 x, const uint8 y) {
}

void XoChipCpu::LD_I_NNNN() {
}

void XoChipCpu::SET_PLN(const uint8 x) {
}

void XoChipCpu::LD_AUDIO_aI() {
}

void XoChipCpu::SHR_Vx_Vy(const uint8 x, const uint8 y) {
}

void XoChipCpu::SUBN_Vx_Vy(const uint8 x, const uint8 y) {
}

void XoChipCpu::SHL_Vx_Vy(const uint8 x, const uint8 y) {
}

void XoChipCpu::JP_V0_addr(const uint16 address) {
}

void XoChipCpu::DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) {
}

void XoChipCpu::LD_HF_Vx(const uint8 x) {
}

void XoChipCpu::SET_PITCH_x(const uint8 x) {
}

void XoChipCpu::LD_aI_Vx(const uint8 x) {
}

void XoChipCpu::LD_Vx_aI(const uint8 x) {
}

void XoChipCpu::LD_R_Vx(const uint8 x) {
}

void XoChipCpu::LD_Vx_R(const uint8 x) {
}

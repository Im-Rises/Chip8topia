#include "CpuDisassembly.h"

auto CpuDisassembly::disassembleOpcode(const uint16 opcode) -> std::string {
    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode & 0x00FF)
        {
        case 0x00E0: return "CLS";
        case 0x00EE: return "RET";
        default: return "SYS";
        }
    case 0x1000: return "JP";
    case 0x2000: return "CALL";
    case 0x3000: return "SE Vx, byte";
    case 0x4000: return "SNE Vx, byte";
    case 0x5000: return "SE Vx, Vy";
    case 0x6000: return "LD Vx, byte";
    case 0x7000: return "ADD Vx, byte";
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0000: return "LD Vx, Vy";
        case 0x0001: return "OR Vx, Vy";
        case 0x0002: return "AND Vx, Vy";
        case 0x0003: return "XOR Vx, Vy";
        case 0x0004: return "ADD Vx, Vy";
        case 0x0005: return "SUB Vx, Vy";
        case 0x0006: return "SHR Vx, Vy";
        case 0x0007: return "SUBN Vx, Vy";
        case 0x000E: return "SHL Vx, Vy";
        default: return "Invalid opcode";
        }
    case 0x9000:
        return "SNE Vx, Vy";
    case 0xA000:
        return "LD I, addr";
    case 0xB000: return "JP V0, addr";
    case 0xC000: return "RND Vx, byte";
    case 0xD000: return "DRW Vx, Vy, nibble";
    case 0xE000:
        switch (opcode & 0x00FF)
        {
        case 0x009E: return "SKP Vx";
        case 0x00A1: return "SKNP Vx";
        default: return "Invalid opcode";
        }
    case 0xF000:
        switch (opcode & 0x00FF)
        {
        case 0x0007: return "LD Vx, DT";
        case 0x000A: return "LD Vx, K";
        case 0x0015: return "LD DT, Vx";
        case 0x0018: return "LD ST, Vx";
        case 0x001E: return "ADD I, Vx";
        case 0x0029: return "LD F, Vx";
        case 0x0033: return "LD B, Vx";
        case 0x0055: return "LD [I], Vx";
        case 0x0065: return "LD Vx, [I]";
        default: return "Invalid opcode";
        }
    default:
        return "Invalid opcode";
    }
}

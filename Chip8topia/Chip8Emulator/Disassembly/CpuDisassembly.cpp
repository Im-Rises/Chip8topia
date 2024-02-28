#include "CpuDisassembly.h"

#include <fmt/format.h>

// static constexpr const char* const INVALID_OPCODE = "Invalid opcode";
static constexpr const char* const INVALID_OPCODE = "";

auto CpuDisassembly::disassembleOpcode(const uint16 opcode) -> std::string {
    const uint8 nibble4 = (opcode & 0xF000) >> 12;
    const uint8 nibble3 = (opcode & 0x0F00) >> 8;
    const uint8 nibble2 = (opcode & 0x00F0) >> 4;
    const uint8 nibble1 = (opcode & 0x000F);

    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode & 0x00FF)
        {
        case 0x00E0: return "CLS";
        case 0x00EE: return "RET";
        default: return "SYS";
        }
    case 0x1000: return fmt::format("JP 0x{:03X}", opcode & 0x0FFF);
    case 0x2000: return fmt::format("CALL 0x{:03X}", opcode & 0x0FFF);
    case 0x3000: return fmt::format("SE V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);
    case 0x4000: return fmt::format("SNE V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);
    case 0x5000: return fmt::format("SE V{:X}, V{:X}", nibble3, nibble2);
    case 0x6000: return fmt::format("LD V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);
    case 0x7000: return fmt::format("ADD V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0000: return fmt::format("LD V{:X}, V{:X}", nibble3, nibble2);
        case 0x0001: return fmt::format("OR V{:X}, V{:X}", nibble3, nibble2);
        case 0x0002: return fmt::format("AND V{:X}, V{:X}", nibble3, nibble2);
        case 0x0003: return fmt::format("XOR V{:X}, V{:X}", nibble3, nibble2);
        case 0x0004: return fmt::format("ADD V{:X}, V{:X}", nibble3, nibble2);
        case 0x0005: return fmt::format("SUB V{:X}, V{:X}", nibble3, nibble2);
        case 0x0006: return fmt::format("SHR V{:X}, V{:X}", nibble3, nibble2);
        case 0x0007: return fmt::format("SUBN V{:X}, V{:X}", nibble3, nibble2);
        case 0x000E: return fmt::format("SHL V{:X}, V{:X}", nibble3, nibble2);
        default: return INVALID_OPCODE;
        }
    case 0x9000:
        return fmt::format("SNE V{:X}, V{:X}", nibble3, nibble2);
    case 0xA000:
        return fmt::format("LD I, 0x{:03X}", opcode & 0x0FFF);
    case 0xB000: return fmt::format("JP V0, 0x{:03X}", opcode & 0x0FFF);
    case 0xC000: return fmt::format("RND V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);
    case 0xD000: return fmt::format("DRW V{:X}, V{:X}, 0x{:X}", nibble3, nibble2, nibble1);
    case 0xE000:
        switch (opcode & 0x00FF)
        {
        case 0x009E: return fmt::format("SKP V{:X}", nibble3);
        case 0x00A1: return fmt::format("SKNP V{:X}", nibble3);
        default: return INVALID_OPCODE;
        }
    case 0xF000:
        switch (opcode & 0x00FF)
        {
        case 0x0007: return fmt::format("LD V{:X}, DT", nibble3);
        case 0x000A: return fmt::format("LD V{:X}, K", nibble3);
        case 0x0015: return fmt::format("LD DT, V{:X}", nibble3);
        case 0x0018: return fmt::format("LD ST, V{:X}", nibble3);
        case 0x001E: return fmt::format("ADD I, V{:X}", nibble3);
        case 0x0029: return fmt::format("LD F, V{:X}", nibble3);
        case 0x0033: return fmt::format("LD B, V{:X}", nibble3);
        case 0x0055: return fmt::format("LD [I], V{:X}", nibble3);
        case 0x0065: return fmt::format("LD V{:X}, [I]", nibble3);
        default: return INVALID_OPCODE;
        }
    default:
        return "Invalid opcode";
    }
}

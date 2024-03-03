#include "Chip8CpuDisassembly.h"

#include <fmt/format.h>

#include "disassemblySettings.h"

auto Chip8CpuDisassembly::disassembleOpcode(const uint16 opcode) -> std::string {
    const uint8 nibble4 = (opcode & 0xF000) >> 12;
    const uint8 nibble3 = (opcode & 0x0F00) >> 8;
    const uint8 nibble2 = (opcode & 0x00F0) >> 4;
    const uint8 nibble1 = (opcode & 0x000F);

    switch (nibble4)
    {
    case 0x0:
        switch (nibble1)
        {
        case 0x0: return "CLS";
        case 0xE: return "RET";
        default: return "SYS";
        }
    case 0x1: return fmt::format("JP 0x{:03X}", opcode & 0x0FFF);
    case 0x2: return fmt::format("CALL 0x{:03X}", opcode & 0x0FFF);
    case 0x3: return fmt::format("SE V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);
    case 0x4: return fmt::format("SNE V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);
    case 0x5: return fmt::format("SE V{:X}, V{:X}", nibble3, nibble2);
    case 0x6: return fmt::format("LD V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);
    case 0x7: return fmt::format("ADD V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);
    case 0x8:
        switch (nibble1)
        {
        case 0x0: return fmt::format("LD V{:X}, V{:X}", nibble3, nibble2);
        case 0x1: return fmt::format("OR V{:X}, V{:X}", nibble3, nibble2);
        case 0x2: return fmt::format("AND V{:X}, V{:X}", nibble3, nibble2);
        case 0x3: return fmt::format("XOR V{:X}, V{:X}", nibble3, nibble2);
        case 0x4: return fmt::format("ADD V{:X}, V{:X}", nibble3, nibble2);
        case 0x5: return fmt::format("SUB V{:X}, V{:X}", nibble3, nibble2);
        case 0x6: return fmt::format("SHR V{:X}, V{:X}", nibble3, nibble2);
        case 0x7: return fmt::format("SUBN V{:X}, V{:X}", nibble3, nibble2);
        case 0xE: return fmt::format("SHL V{:X}, V{:X}", nibble3, nibble2);
        default: return INVALID_OPCODE;
        }
    case 0x9:
        return fmt::format("SNE V{:X}, V{:X}", nibble3, nibble2);
    case 0xA:
        return fmt::format("LD I, 0x{:03X}", opcode & 0x0FFF);
    case 0xB: return fmt::format("JP V0, 0x{:03X}", opcode & 0x0FFF);
    case 0xC: return fmt::format("RND V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);
    case 0xD: return fmt::format("DRW V{:X}, V{:X}, 0x{:X}", nibble3, nibble2, nibble1);
    case 0xE:
        switch (nibble1)
        {
        case 0xE: return fmt::format("SKP V{:X}", nibble3);
        case 0x1: return fmt::format("SKNP V{:X}", nibble3);
        default: return INVALID_OPCODE;
        }
    case 0xF:
        switch (nibble2)
        {
        case 0x0:
            switch (nibble1)
            {
            case 0x0007: return fmt::format("LD V{:X}, DT", nibble3);
            case 0x000A: return fmt::format("LD V{:X}, K", nibble3);
            default: return INVALID_OPCODE;
            }
            break;
        case 0x1:
            switch (nibble1)
            {
            case 0x5: return fmt::format("LD DT, V{:X}", nibble3);
            case 0x8: return fmt::format("LD ST, V{:X}", nibble3);
            case 0xE: return fmt::format("ADD I, V{:X}", nibble3);
            }
            break;
        case 0x2: return fmt::format("LD F, V{:X}", nibble3);
        case 0x3: return fmt::format("LD B, V{:X}", nibble3);
        case 0x5: return fmt::format("LD [I], V{:X}", nibble3);
        case 0x6: return fmt::format("LD V{:X}, [I]", nibble3);
        default: return INVALID_OPCODE;
        }
    default:
        return "Invalid opcode";
    }
}

#include "SChipCCpuDisassembly.h"

#include <fmt/format.h>

#include "disassemblySettings.h"

auto SChipCCpuDisassembly::disassembleOpcode(const uint16 opcode) -> std::string {
    const uint8 nibble4 = (opcode & 0xF000) >> 12;
    const uint8 nibble3 = (opcode & 0x0F00) >> 8;
    const uint8 nibble2 = (opcode & 0x00F0) >> 4;
    const uint8 nibble1 = (opcode & 0x000F);

    switch (nibble4)
    {
    case 0x0: {
        switch (nibble2)
        {
        case 0xC: return fmt::format("SCD {}", nibble1); // 00CN
        case 0xE: {
            switch (nibble1)
            {
            case 0x0: return "CLS";  // 00E0
            case 0xE: return "RET;"; // 00EE
            }
            break;
        }
        case 0xF: {
            switch (nibble1)
            {
            case 0xB: return fmt::format("SCR {}", nibble1); // 00FB
            case 0xC: return fmt::format("SCL {}", nibble1); // 00FC
            case 0xD: return "EXIT";                         // 00FD
            case 0xE: return "LORES";                        // 00FE
            case 0xF: return "HIRES";                        // 00FF
            }
            break;
        }
        }
        break;
    }

    case 0x1: return fmt::format("JP {}", opcode & 0x0FFF);                // 1NNN
    case 0x2: return fmt::format("CALL {}", opcode & 0x0FFF);              // 2NNN
    case 0x3: return fmt::format("SE V{}, {}", nibble3, opcode & 0x00FF);  // 3XNN
    case 0x4: return fmt::format("SNE V{}, {}", nibble3, opcode & 0x00FF); // 4XNN
    case 0x5: return fmt::format("SE V{}, V{}", nibble3, nibble2);         // 5XY0
    case 0x6: return fmt::format("LD V{}, {}", nibble3, opcode & 0x00FF);  // 6XNN
    case 0x7: return fmt::format("ADD V{}, {}", nibble3, opcode & 0x00FF); // 7XNN
    case 0x8: {
        switch (nibble1)
        {
        case 0x0: return fmt::format("LD V{}, V{}", nibble3, nibble2);   // 8XY0
        case 0x1: return fmt::format("OR V{}, V{}", nibble3, nibble2);   // 8XY1
        case 0x2: return fmt::format("AND V{}, V{}", nibble3, nibble2);  // 8XY2
        case 0x3: return fmt::format("XOR V{}, V{}", nibble3, nibble2);  // 8XY3
        case 0x4: return fmt::format("ADD V{}, V{}", nibble3, nibble2);  // 8XY4
        case 0x5: return fmt::format("SUB V{}, V{}", nibble3, nibble2);  // 8XY5
        case 0x6: return fmt::format("SHR V{}, V{}", nibble3, nibble2);  // 8XY6
        case 0x7: return fmt::format("SUBN V{}, V{}", nibble3, nibble2); // 8XY7
        case 0xE: return fmt::format("SHL V{}, V{}", nibble3, nibble2);  // 8XYE
        default: /* Invalid opcode */ break;
        }
        break;
    }
    case 0x9: return fmt::format("SNE V{}, V{}", nibble3, nibble2);              // 9XY0
    case 0xA: return fmt::format("LD I, {}", opcode & 0x0FFF);                   // ANNN
    case 0xB: return fmt::format("JP V{}, {}", nibble3, opcode & 0x0FFF);        // Bxnn
    case 0xC: return fmt::format("RND V{}, {}", nibble3, opcode & 0x00FF);       // CXNN
    case 0xD: return fmt::format("DRW V{}, V{}, {}", nibble3, nibble2, nibble1); // DXYN
    case 0xE: {
        switch (nibble1)
        {
        case 0xE: return fmt::format("SKP V{}", nibble3);  // EX9E
        case 0x1: return fmt::format("SKNP V{}", nibble3); // EXA1
        }
        break;
    }
    case 0xF: {
        switch (nibble2)
        {
        case 0x0:
            switch (nibble1)
            {
            case 0x7: return fmt::format("LD V{}, DT", nibble3); // FX07
            case 0xA: return fmt::format("LD V{}, K", nibble3);  // FX0A
            default: break;
            }
            break;
        case 0x1:
            switch (nibble1)
            {
            case 0x5: return fmt::format("LD DT, V{}", nibble3); // FX15
            case 0x8: return fmt::format("LD ST, V{}", nibble3); // FX18
            case 0xE: return fmt::format("ADD I, V{}", nibble3); // FX1E
            default: break;
            }
            break;
        case 0x2: return fmt::format("LD F, V{}", nibble3); // FX29
        case 0x3: {
            switch (nibble1)
            {
            case 0x0: return fmt::format("LD HF, V{}", nibble3); // FX30
            case 0x3: return fmt::format("LD B, V{}", nibble3);  // FX33
            }
            break;
        }
        case 0x5: return fmt::format("LD [I], V{}", nibble3); // FX55
        case 0x6: return fmt::format("LD V{}, [I]", nibble3); // FX65
        case 0x7: return fmt::format("LD R, V{}", nibble3);   // FX75
        case 0x8: return fmt::format("LD V{}, R", nibble3);   // FX85
        default: /* Invalid opcode */ break;
        }
        break;
    }
    }

    return INVALID_OPCODE_TEXT;
}
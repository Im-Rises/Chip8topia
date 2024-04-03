#include "SChipCCpuDisassembly.h"

#include <fmt/format.h>

#include "disassemblySettings.h"

auto SChipCCpuDisassembly::disassembleOpcode(const uint16 opcode, bool isLowRes) -> std::string
{
    const uint8 nibble4 = (opcode & 0xF000) >> 12;
    const uint8 nibble3 = (opcode & 0x0F00) >> 8;
    const uint8 nibble2 = (opcode & 0x00F0) >> 4;
    const uint8 nibble1 = (opcode & 0x000F);

    switch (nibble4)
    {
    case 0x0:
    {
        switch (opcode & 0x00FF)
        {
        case 0xE0: return "CLS";                                       // 00E0
        case 0xEE: return "RET";                                       // 00EE
        case 0xFB: return fmt::format("SCR 0x{:X}", isLowRes ? 2 : 4); // 00FB
        case 0xFC: return fmt::format("SCL 0x{:X}", isLowRes ? 2 : 4); // 00FC
        case 0xFD: return "EXIT";                                      // 00FD
        case 0xFE: return "LORES";                                     // 00FE
        case 0xFF: return "HIRES";                                     // 00FF
        default:
        {
            switch (nibble2)
            {
            case 0xC: return fmt::format("SCD 0x{:X}", isLowRes ? nibble1 / 2 : nibble1); // 00CN
            default: return INVALID_OPCODE_TEXT;
            }
        }
        }
        break;
    }
    case 0x1: return fmt::format("JP 0x{:03X}", opcode & 0x0FFF);                  // 1NNN
    case 0x2: return fmt::format("CALL 0x{:03X}", opcode & 0x0FFF);                // 2NNN
    case 0x3: return fmt::format("SE V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);  // 3XNN
    case 0x4: return fmt::format("SNE V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF); // 4XNN
    case 0x5:
    {
        switch (nibble1)
        {
        case 0x0: return fmt::format("SE V{:X}, V{:X}", nibble3, nibble2); // 5XY0
        default: return "INVALID OPCODE";
        }
    }
    case 0x6: return fmt::format("LD V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);  // 6XNN
    case 0x7: return fmt::format("ADD V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF); // 7XNN
    case 0x8:
    {
        switch (nibble1)
        {
        case 0x0: return fmt::format("LD V{:X}, V{:X}", nibble3, nibble2);   // 8XY0
        case 0x1: return fmt::format("OR V{:X}, V{:X}", nibble3, nibble2);   // 8XY1
        case 0x2: return fmt::format("AND V{:X}, V{:X}", nibble3, nibble2);  // 8XY2
        case 0x3: return fmt::format("XOR V{:X}, V{:X}", nibble3, nibble2);  // 8XY3
        case 0x4: return fmt::format("ADD V{:X}, V{:X}", nibble3, nibble2);  // 8XY4
        case 0x5: return fmt::format("SUB V{:X}, V{:X}", nibble3, nibble2);  // 8XY5
        case 0x6: return fmt::format("SHR V{:X}, V{:X}", nibble3, nibble2);  // 8XY6
        case 0x7: return fmt::format("SUBN V{:X}, V{:X}", nibble3, nibble2); // 8XY7
        case 0xE: return fmt::format("SHL V{:X}, V{:X}", nibble3, nibble2);  // 8XYE
        default: return "INVALID OPCODE";
        }
        break;
    }
    case 0x9:
    {
        switch (nibble1)
        {
        case 0x0: return fmt::format("SNE V{:X}, V{:X}", nibble3, nibble2); // 9XY0
        default: return "INVALID OPCODE";
        }
        break;
    }
    case 0xA: return fmt::format("LD I, 0x{:03X}", opcode & 0x0FFF);                       // ANNN
    case 0xB: return fmt::format("JP 0x{:03X}, V0", opcode & 0x0FFF);                      // BNNN
    case 0xC: return fmt::format("RND V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);         // CXNN
    case 0xD: return fmt::format("DRW V{:X}, V{:X}, 0x{:02X}", nibble3, nibble2, nibble1); // DXYN
    case 0xE:
    {
        switch (opcode & 0x00FF)
        {
        case 0x9E: return fmt::format("SKP V{:X}", nibble3);  // EX9E
        case 0xA1: return fmt::format("SKNP V{:X}", nibble3); // EXA1
        default: return "INVALID OPCODE";
        }
        break;
    }
    case 0xF:
    {
        switch (opcode & 0x00FF)
        {
        case 0x07: return fmt::format("LD V{:X}, DT", nibble3);  // FX07
        case 0x0A: return fmt::format("LD V{:X}, K", nibble3);   // FX0A
        case 0x15: return fmt::format("LD DT, V{:X}", nibble3);  // FX15
        case 0x18: return fmt::format("LD ST, V{:X}", nibble3);  // FX18
        case 0x1E: return fmt::format("ADD I, V{:X}", nibble3);  // FX1E
        case 0x29: return fmt::format("LD F, V{:X}", nibble3);   // FX29
        case 0x30: return fmt::format("LD HF, V{:X}", nibble3);  // FX30
        case 0x33: return fmt::format("LD B, V{:X}", nibble3);   // FX33
        case 0x55: return fmt::format("LD [I], V{:X}", nibble3); // FX55
        case 0x65: return fmt::format("LD V{:X}, [I]", nibble3); // FX65
        case 0x75: return fmt::format("LD R, V{:X}", nibble3);   // FX75
        case 0x85: return fmt::format("LD V{:X}, R", nibble3);   // FX85
        default: return INVALID_OPCODE_TEXT;
        }
    }
    default: return INVALID_OPCODE_TEXT;
    }

    //    const uint8 nibble4 = (opcode & 0xF000) >> 12;
    //    const uint8 nibble3 = (opcode & 0x0F00) >> 8;
    //    const uint8 nibble2 = (opcode & 0x00F0) >> 4;
    //    const uint8 nibble1 = (opcode & 0x000F);
    //
    //    switch (nibble4)
    //    {
    //    case 0x0:
    //    {
    //        switch (nibble2)
    //        {
    //        case 0xC: return fmt::format("SCD 0x{:X}", isLowRes ? nibble1 / 2 : nibble1); // 00CN
    //        case 0xE:
    //        {
    //            switch (nibble1)
    //            {
    //            case 0x0: return "CLS";  // 00E0
    //            case 0xE: return "RET;"; // 00EE
    //            }
    //            break;
    //        }
    //        case 0xF:
    //        {
    //            switch (nibble1)
    //            {
    //            case 0xB: return fmt::format("SCR 0x{:X}", isLowRes ? 2 : 4); // 00FB
    //            case 0xC: return fmt::format("SCL 0x{:X}", isLowRes ? 2 : 4); // 00FC
    //            case 0xD: return "EXIT";                                      // 00FD
    //            case 0xE: return "LORES";                                     // 00FE
    //            case 0xF: return "HIRES";                                     // 00FF
    //            }
    //            break;
    //        }
    //        }
    //        break;
    //    }
    //
    //    case 0x1: return fmt::format("JP 0x{:03X}", opcode & 0x0FFF);                  // 1NNN
    //    case 0x2: return fmt::format("CALL 0x{:03X}", opcode & 0x0FFF);                // 2NNN
    //    case 0x3: return fmt::format("SE V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);  // 3XNN
    //    case 0x4: return fmt::format("SNE V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF); // 4XNN
    //    case 0x5: return fmt::format("SE V{:X}, V{:X}", nibble3, nibble2);             // 5XY0
    //    case 0x6: return fmt::format("LD V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);  // 6XNN
    //    case 0x7: return fmt::format("ADD V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF); // 7XNN
    //    case 0x8:
    //    {
    //        switch (nibble1)
    //        {
    //        case 0x0: return fmt::format("LD V{:X}, V{:X}", nibble3, nibble2);   // 8XY0
    //        case 0x1: return fmt::format("OR V{:X}, V{:X}", nibble3, nibble2);   // 8XY1
    //        case 0x2: return fmt::format("AND V{:X}, V{:X}", nibble3, nibble2);  // 8XY2
    //        case 0x3: return fmt::format("XOR V{:X}, V{:X}", nibble3, nibble2);  // 8XY3
    //        case 0x4: return fmt::format("ADD V{:X}, V{:X}", nibble3, nibble2);  // 8XY4
    //        case 0x5: return fmt::format("SUB V{:X}, V{:X}", nibble3, nibble2);  // 8XY5
    //        case 0x6: return fmt::format("SHR V{:X}, V{:X}", nibble3, nibble2);  // 8XY6
    //        case 0x7: return fmt::format("SUBN V{:X}, V{:X}", nibble3, nibble2); // 8XY7
    //        case 0xE: return fmt::format("SHL V{:X}, V{:X}", nibble3, nibble2);  // 8XYE
    //        }
    //        break;
    //    }
    //    case 0x9: return fmt::format("SNE V{:X}, V{:X}", nibble3, nibble2);                    // 9XY0
    //    case 0xA: return fmt::format("LD I, 0x{:03X}", opcode & 0x0FFF);                       // ANNN
    //    case 0xB: return fmt::format("JP 0x{:03X}, V0", opcode & 0x0FFF);                      // BNNN
    //    case 0xC: return fmt::format("RND V{:X}, 0x{:02X}", nibble3, opcode & 0x00FF);         // CXNN
    //    case 0xD: return fmt::format("DRW V{:X}, V{:X}, 0x{:02X}", nibble3, nibble2, nibble1); // DXYN
    //    case 0xE:
    //    {
    //        switch (nibble1)
    //        {
    //        case 0xE: return fmt::format("SKP V{:X}", nibble3);  // EX9E
    //        case 0x1: return fmt::format("SKNP V{:X}", nibble3); // EXA1
    //        }
    //        break;
    //    }
    //    case 0xF:
    //    {
    //        switch (nibble2)
    //        {
    //        case 0x0:
    //            switch (nibble1)
    //            {
    //            case 0x7: return fmt::format("LD V{:X}, DT", nibble3); // FX07
    //            case 0xA: return fmt::format("LD V{:X}, K", nibble3);  // FX0A
    //            default: break;
    //            }
    //            break;
    //        case 0x1:
    //            switch (nibble1)
    //            {
    //            case 0x5: return fmt::format("LD DT, V{:X}", nibble3); // FX15
    //            case 0x8: return fmt::format("LD ST, V{:X}", nibble3); // FX18
    //            case 0xE: return fmt::format("ADD I, V{:X}", nibble3); // FX1E
    //            default: break;
    //            }
    //            break;
    //        case 0x2: return fmt::format("LD F, V{:X}", nibble3); // FX29
    //        case 0x3:
    //        {
    //            switch (nibble1)
    //            {
    //            case 0x0: return fmt::format("LD HF, V{:X}", nibble3); // FX30
    //            case 0x3: return fmt::format("LD B, V{:X}", nibble3);  // FX33
    //            }
    //            break;
    //        }
    //        case 0x5: return fmt::format("LD [I], V{:X}", nibble3); // FX55
    //        case 0x6: return fmt::format("LD V{:X}, [I]", nibble3); // FX65
    //        case 0x7: return fmt::format("LD R, V{:X}", nibble3);   // FX75
    //        case 0x8: return fmt::format("LD V{:X}, R", nibble3);   // FX85
    //        }
    //        break;
    //    }
    //    }
    //
    //    return INVALID_OPCODE_TEXT;
}
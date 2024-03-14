// #include "XoChipCpu.h"
//
// #include "XoChipPpu.h"
//
// XoChipCpu::XoChipCpu() : m_savedV{} {
//     //    std::copy(XO_FONTSET.begin(), XO_FONTSET.end(), m_memory.begin());
// }
//
// void XoChipCpu::setPpu(std::shared_ptr<PpuBase> ppu) {
//     CpuBase::setPpu(ppu);
//     m_ppuCasted = dynamic_cast<XoChipPpu*>(ppu.get());
// }
//
// void XoChipCpu::reset() {
//     CpuBase::reset();
//     m_savedV.fill(0);
// }
//
// void XoChipCpu::computeOpcode(const uint16 opcode) {
//     const uint8 nibble4 = (opcode & 0xF000) >> 12;
//     const uint8 nibble3 = (opcode & 0x0F00) >> 8;
//     const uint8 nibble2 = (opcode & 0x00F0) >> 4;
//     const uint8 nibble1 = (opcode & 0x000F);
//
//     //    switch (nibble4)
//     //    {
//     //    case 0x0: {
//     //        switch (nibble2)
//     //        {
//     //        case 0xC: SCD(nibble1); break; // 00CN
//     //        case 0xE: {
//     //            switch (nibble1)
//     //            {
//     //            case 0x0: CLS(); break; // 00E0
//     //            case 0xE: RET(); break; // 00EE
//     //            }
//     //            break;
//     //        }
//     //        case 0xF: {
//     //            switch (nibble1)
//     //            {
//     //            case 0xB: SCR(nibble1); break; // 00FB
//     //            case 0xC: SCL(nibble1); break; // 00FC
//     //            case 0xD: EXIT(); break;       // 00FD
//     //            case 0xE: LORES(); break;      // 00FE
//     //            case 0xF: HIRES(); break;      // 00FF
//     //            }
//     //            break;
//     //        }
//     //        }
//     //        break;
//     //    }
//     //
//     //    case 0x1: JP_addr(opcode & 0x0FFF); break;            // 1NNN
//     //    case 0x2: CALL_addr(opcode & 0x0FFF); break;          // 2NNN
//     //    case 0x3: SE_Vx_nn(nibble3, opcode & 0x00FF); break;  // 3XNN
//     //    case 0x4: SNE_Vx_nn(nibble3, opcode & 0x00FF); break; // 4XNN
//     //    case 0x5: SE_Vx_Vy(nibble3, nibble2); break;          // 5XY0
//     //    case 0x6: LD_Vx_nn(nibble3, opcode & 0x00FF); break;  // 6XNN
//     //    case 0x7: ADD_Vx_nn(nibble3, opcode & 0x00FF);
// }

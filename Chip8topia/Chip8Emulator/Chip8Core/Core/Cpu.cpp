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

uint16 Cpu::fetchOpcode() {
    const uint16 opcode = (m_memory[m_pc] << 8) | (m_memory[m_pc + 1]);
    m_pc += 2;
    return opcode;
}

void Cpu::computeOpcode(uint16 opcode) {
    const uint8 nibble1 = (opcode & 0xF000) >> 12;
    const uint8 nibble2 = (opcode & 0x0F00) >> 8;
    const uint8 nibble3 = (opcode & 0x00F0) >> 4;
    const uint8 nibble4 = (opcode & 0x000F);

    switch (nibble1)
    {
    case 0x0: {
        switch (nibble4)
        {
        case 0x0:
            // 00E0 - CLS
            // Clear the display.
            break;
        case 0xE:
            // 00EE - RET
            // Return from a subroutine.
            break;
        default:
            // 0NNN - SYS addr
            // Jump to a machine code routine at nnn.
            break;
        }
        break;
    }
    case 0x1: {
        // 1NNN - JP addr
        // Jump to location nnn.
        break;
    }
    case 0x2: {
        // 2NNN - CALL addr
        // Call subroutine at nnn.
        break;
    }
    case 0x3: {
        // 3XNN - SE Vx, byte
        // Skip next instruction if Vx = kk.
        break;
    }
    case 0x4: {
        // 4XNN - SNE Vx, byte
        // Skip next instruction if Vx != kk.
        break;
    }
    case 0x5: {
        // 5XY0 - SE Vx, Vy
        // Skip next instruction if Vx = Vy.
        break;
    }
    case 0x6: {
        // 6XNN - LD Vx, byte
        // Set Vx = kk.
        break;
    }
    case 0x7: {
        // 7XNN - ADD Vx, byte
        // Set Vx = Vx + kk.
        break;
    }
    case 0x8: {
        switch (nibble4)
        {
        case 0x0: {
            // 8XY0 - LD Vx, Vy
            // Set Vx = Vy.
            break;
        }
        case 0x1: {
            // 8XY1 - OR Vx, Vy
            // Set Vx = Vx OR Vy.
            break;
        }
        case 0x2: {
            // 8XY2 - AND Vx, Vy
            // Set Vx = Vx AND Vy.
            break;
        }
        case 0x3: {
            // 8XY3 - XOR Vx, Vy
            // Set Vx = Vx XOR Vy.
            break;
        }
        case 0x4: {
            // 8XY4 - ADD Vx, Vy
            // Set Vx = Vx + Vy, set VF = carry.
            break;
        }
        case 0x5: {
            // 8XY5 - SUB Vx, Vy
            // Set Vx = Vx - Vy, set VF = NOT borrow.
            break;
        }
        case 0x6: {
            // 8XY6 - SHR Vx {, Vy}
            // Set Vx = Vx SHR 1.
            break;
        }
        case 0x7: {
            // 8XY7 - SUBN Vx, Vy
            // Set Vx = Vy - Vx, set VF = NOT borrow.
            break;
        }
        case 0xE: {
            // 8XYE - SHL Vx {, Vy}
            // Set Vx = Vx SHL 1.
            break;
        }
        default:
            break;
        }
        break;
    }
    case 0x9: {
        // 9XY0 - SNE Vx, Vy
        // Skip next instruction if Vx != Vy.
        break;
    }
    case 0xA: {
        // ANNN - LD I, addr
        // Set I = nnn.
        break;
    }
    case 0xB: {
        // BNNN - JP V0, addr
        // Jump to location nnn + V0.
        break;
    }
    case 0xC: {
        // CXNN - RND Vx, byte
        // Set Vx = random byte AND kk.
        break;
    }
    case 0xD: {
        // DXYN - DRW Vx, Vy, nibble
        // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
        break;
    }
    case 0xE: {
        switch (nibble3)
        {
        case 0xE: {
            // EX9E - SKP Vx
            // Skip next instruction if key with the value of Vx is pressed.
            break;
        }
        case 0x1: {
            // EXA1 - SKNP Vx
            // Skip next instruction if key with the value of Vx is not pressed.
            break;
        }
        default:
            break;
        }
        break;
    }
    case 0xF: {
        switch (nibble3)
        {
        case 0x0:
            switch (nibble4)
            {
            case 0x7: {
                // FX07 - LD Vx, DT
                // Set Vx = delay timer value.
                break;
            }
            case 0xA: {
                // FX0A - LD Vx, K
                // Wait for a key press, store the value of the key in Vx.
                break;
            }
            default:
                break;
            }
            break;
        case 0x1:
            switch (nibble4)
            {
            case 0x5: {
                // FX15 - LD DT, Vx
                // Set delay timer = Vx.
                break;
            }
            case 0x8: {
                // FX18 - LD ST, Vx
                // Set sound timer = Vx.
                break;
            }
            case 0xE: {
                // FX1E - ADD I, Vx
                // Set I = I + Vx.
                break;
            }
            default:
                break;
            }
            break;
        case 0x2: {
            // FX29 - LD F, Vx
            // Set I = location of sprite for digit Vx.
            break;
        }
        case 0x3: {
            // FX33 - LD B, Vx
            // Store BCD representation of Vx in memory locations I, I+1, and I+2.
            break;
        }
        case 0x5: {
            // FX55 - LD [I], Vx
            // Store registers V0 through Vx in memory starting at location I.
            break;
        }
        case 0x6: {
            // FX65 - LD Vx, [I]
            // Read registers V0 through Vx from memory starting at location I.
            break;
        }
        default:
            break;
        }
        break;
    }
    }
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

#pragma once

// Send the Chip8Core pointer/reference to the debugger so it can access the registers, memory, etc.

class Chip8topiaDebugger {
public:
    void drawDebuggerMenu();

private:
    void drawRegisters();
    void drawMemory();
    void drawStack();

    void drawKeyboard();

    void drawDisassembler();
};

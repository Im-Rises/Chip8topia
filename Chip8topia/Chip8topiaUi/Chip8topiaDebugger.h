#pragma once

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

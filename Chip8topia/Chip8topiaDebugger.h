#pragma once

class Chip8topiaDebugger {
public:
    void drawDebugger();

private:
    void drawRegisters();
    void drawMemory();
    void drawStack();

    void drawKeyboard();

    void drawDisassembler();
};

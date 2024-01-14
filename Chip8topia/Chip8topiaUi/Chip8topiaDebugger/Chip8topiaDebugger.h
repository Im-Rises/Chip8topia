#pragma once

// Send the Chip8Core pointer/reference to the debugger so it can access the registers, memory, etc.

class Chip8topiaDebugger {
public:
    Chip8topiaDebugger() = default;
    Chip8topiaDebugger(const Chip8topiaDebugger&) = delete;
    Chip8topiaDebugger(Chip8topiaDebugger&&) = delete;
    auto operator=(const Chip8topiaDebugger&) -> Chip8topiaDebugger& = delete;
    auto operator=(Chip8topiaDebugger&&) -> Chip8topiaDebugger& = delete;
    ~Chip8topiaDebugger() = default;

public:
    void drawDebuggerMenu();

private:
    void drawRegisters();
    void drawMemory();
    void drawStack();

    void drawKeyboard();

    void drawDisassembler();
};

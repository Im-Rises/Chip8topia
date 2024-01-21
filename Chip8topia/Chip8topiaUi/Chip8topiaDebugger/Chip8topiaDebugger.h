#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_memory_editor.h>

class Chip8Core;
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

    void drawDebuggerWindows();

private:
    void drawRegisters();
    void drawStack();
    void drawMemory();
    void drawKeyboard();
    void drawDisassembler();

private:
    MemoryEditor m_memoryEditor;

    bool m_isRegistersOpen = false;
    bool m_isStackOpen = false;
    bool m_isMemoryEditorOpen = false;
    bool m_isKeyboardOpen = false;
    bool m_isDisassemblerOpen = false;
};

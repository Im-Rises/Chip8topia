#pragma once

#include <imgui/imgui.h>
#include <imgui_memory_editor/imgui_memory_editor.h>
#include <unordered_set>

class Chip8Core;
class Chip8topiaDebugger {
    enum class DebuggerWindow {
        Registers,
        Stack,
        MemoryEditor,
        Keyboard,
        Disassembler
    };

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

    std::unordered_set<DebuggerWindow> m_openDebuggerWindows{
        DebuggerWindow::Registers,
        DebuggerWindow::Stack,
        DebuggerWindow::MemoryEditor,
        DebuggerWindow::Keyboard,
        DebuggerWindow::Disassembler
    };

    bool m_isRegistersOpen = false;
    bool m_isStackOpen = false;
    bool m_isMemoryEditorOpen = false;
    bool m_isKeyboardOpen = false;
    bool m_isDisassemblerOpen = false;
};

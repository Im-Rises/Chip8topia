#pragma once

#include <imgui/imgui.h>
#include <imgui_memory_editor/imgui_memory_editor.h>
#include <unordered_set>
#include <functional>
#include <array>

class Chip8Core;
class Chip8topiaDebugger {
    //    enum class DebuggerWindow {
    //        Registers,
    //        Stack,
    //        MemoryEditor,
    //        Keyboard,
    //        Disassembler
    //    };

    struct MenuItem {
        const char* name;
        bool isOpen;
        std::function<void(const char* title, bool* p_open)> drawFunction;

        void drawMenuItem() {
            if (ImGui::MenuItem(name))
            {
                isOpen = !isOpen;
            }
        }

        void drawWindow() {
            if (isOpen)
            {
                drawFunction(name, &isOpen);
            }
        }
    };

public:
    Chip8topiaDebugger() = default;
    Chip8topiaDebugger(const Chip8topiaDebugger&) = delete;
    Chip8topiaDebugger(Chip8topiaDebugger&&) = delete;
    auto operator=(const Chip8topiaDebugger&) -> Chip8topiaDebugger& = delete;
    auto operator=(Chip8topiaDebugger&&) -> Chip8topiaDebugger& = delete;
    ~Chip8topiaDebugger() = default;

public:
    //    void drawDebuggerMenu();
    void drawDebugger();
    //    void drawDebuggerWindows();

private:
    void drawRegisters(const char* title, bool* p_open);
    void drawStack();
    void drawMemory();
    void drawKeyboard();
    void drawDisassembler();

private:
    MemoryEditor m_memoryEditor;

    std::array<MenuItem, 1> m_menuItems = {
        MenuItem{ "Registers", false, [this](const char* title, bool* p_open) { drawRegisters(title, p_open); }
//        MenuItem{ "Stack", false, [this]() { drawStack(); } },
//        MenuItem{ "Memory Editor", false, [this]() { drawMemory(); } },
//        MenuItem{ "Keyboard", false, [this]() { drawKeyboard(); } },
//        MenuItem{ "Disassembler", false, [this]() { drawDisassembler(); }
        }
    };

    //    std::unordered_set<DebuggerWindow> m_openDebuggerWindows{
    //        DebuggerWindow::Registers,
    //        DebuggerWindow::Stack,
    //        DebuggerWindow::MemoryEditor,
    //        DebuggerWindow::Keyboard,
    //        DebuggerWindow::Disassembler
    //    };
    //
    //    bool m_isRegistersOpen = false;
    //    bool m_isStackOpen = false;
    //    bool m_isMemoryEditorOpen = false;
    //    bool m_isKeyboardOpen = false;
    //    bool m_isDisassemblerOpen = false;
};

#pragma once

#include <imgui/imgui.h>
#include <imgui_memory_editor/imgui_memory_editor.h>
#include <unordered_set>
#include <functional>
#include <array>

class Chip8Emulator;
class Chip8topiaDebugger {
    struct MenuItem {
        const char* name;
        bool isOpen;
        std::function<void()> drawFunction;

        void drawMenuItem() {
            if (ImGui::MenuItem(name))
            {
                isOpen = !isOpen;
            }
        }

        void drawWindow() {
            if (isOpen)
            {
                if (ImGui::Begin(name, &isOpen))
                {
                    drawFunction();
                    ImGui::End();
                }
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
    void drawDebugger(Chip8Emulator& emulator);

private:
    void drawRegisters();
    void drawStack();
    void drawMemory();
    void drawKeyboard();
    void drawDisassembler();

private:
    MemoryEditor m_memoryEditor;

    std::array<MenuItem, 5> m_menuItems = {
        "Registers", false, [this]() { drawRegisters(); },
        "Stack", false, [this]() { drawStack(); },
        "Memory Editor", false, [this]() { drawMemory(); },
        "Keyboard", false, [this]() { drawKeyboard(); },
        "Disassembler", false, [this]() { drawDisassembler(); }
    };
};

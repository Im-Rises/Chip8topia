#pragma once

#include <imgui/imgui.h>
#include <imgui_memory_editor/imgui_memory_editor.h>
#include <functional>
#include <array>

#include "../../Chip8Emulator/Chip8Emulator.h"
#include "../../Chip8Emulator/Chip8Core/Chip8Core.h"

class Chip8Emulator;
class Chip8topiaDebugger {
    template <typename... Args>
    struct MenuItem {
        const char* m_name;
        bool m_isOpen;
        std::function<void(Args*...)> m_drawFunction;

        void drawMenuItem() {
            if (ImGui::MenuItem(m_name))
            {
                m_isOpen = !m_isOpen;
            }
        }

        void drawWindow(Args*... args) {
            if (m_isOpen)
            {
                if (ImGui::Begin(m_name, &m_isOpen))
                {
                    m_drawFunction(args...);
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
    void drawDebuggerMenu();
    void drawDebuggerWindows(Chip8Emulator& emulator);

private:
    void drawRegisters(Chip8Core* chip8);
    void drawStack(Chip8Core* chip8);
    void drawMemory(Chip8Core* chip8);
    void drawKeypad(Chip8Core* chip8);
    void drawDisassembler(Chip8Core* chip8);

private:
    MemoryEditor m_memoryEditor;

    std::array<MenuItem<Chip8Core>, 5> m_menuItems = {
        "Registers", true, [this](Chip8Core* chip8) { drawRegisters(chip8); },
        "Stack", true, [this](Chip8Core* chip8) { drawStack(chip8); },
        "Memory Editor", true, [this](Chip8Core* chip8) { drawMemory(chip8); },
        "Keyboard", true, [this](Chip8Core* chip8) { drawKeypad(chip8); },
        "Disassembler", true, [this](Chip8Core* chip8) { drawDisassembler(chip8); }
    };
};

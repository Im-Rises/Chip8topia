#pragma once

#include <imgui.h>
#include <imgui_memory_editor/imgui_memory_editor.h>
#include <functional>
#include <array>

#include "../../Chip8Emulator/Chip8Emulator.h"
#include "../../Chip8Emulator/Chip8Core/Chip8Core.h"
#include "Chip8Disassembler.h"

class Chip8Emulator;
class Chip8topiaDebugger {
private:
#if defined(__EMSCRIPTEN__)
    static constexpr auto INITIAL_WINDOW_STATE = false;
#else
    static constexpr auto INITIAL_WINDOW_STATE = true;
#endif

private:
    template <typename... Args>
    struct MenuItem {
        const char* m_name;
        bool m_isOpen;
        std::function<void(Args*...)> m_drawFunction;

        void drawMenuItem() {
            ImGui::MenuItem(m_name, nullptr, &m_isOpen);
        }

        void drawWindow(Args*... args) {
            if (m_isOpen)
            {
                ImGui::Begin(m_name, &m_isOpen);
                m_drawFunction(args...);
                ImGui::End();
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
    void drawAssembly(Chip8Core* chip8);
    void drawDisassemblyControls(Chip8Core* chip8);

private:
    MemoryEditor m_memoryEditor;
    Chip8Disassembler m_disassembler;

    std::array<MenuItem<Chip8Core>, 6> m_menuItems = {
        "Registers", INITIAL_WINDOW_STATE, [this](Chip8Core* chip8) { drawRegisters(chip8); },
        "Stack", INITIAL_WINDOW_STATE, [this](Chip8Core* chip8) { drawStack(chip8); },
        "Memory Editor", INITIAL_WINDOW_STATE, [this](Chip8Core* chip8) { drawMemory(chip8); },
        "Keypad", INITIAL_WINDOW_STATE, [this](Chip8Core* chip8) { drawKeypad(chip8); },
        "Assembly", INITIAL_WINDOW_STATE, [this](Chip8Core* chip8) { drawAssembly(chip8); },
        "Assembly Controls", INITIAL_WINDOW_STATE, [this](Chip8Core* chip8) { drawDisassemblyControls(chip8); }
    };
};

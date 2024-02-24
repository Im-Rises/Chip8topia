#pragma once

#include <imgui.h>
#include <imgui_memory_editor/imgui_memory_editor.h>
#include <array>

#include "../../Chip8Emulator/Chip8Emulator.h"
#include "../../Chip8Emulator/Chip8Core/Chip8Core.h"
#include "../ImGuiHelper/ImGuiHelper.h"
#include "Chip8Disassembler.h"

class Chip8Emulator;
class Chip8topiaDebugger {
private:
#if defined(__EMSCRIPTEN__)
    static constexpr auto INITIAL_WINDOW_STATE = false;
#else
    static constexpr auto INITIAL_WINDOW_STATE = true;
#endif

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
    void drawRegisters(Chip8CoreBase* chip8);
    void drawStack(Chip8CoreBase* chip8);
    void drawMemory(Chip8CoreBase* chip8);
    void drawKeypad(Chip8CoreBase* chip8);
    void drawAssembly(Chip8CoreBase* chip8);
    void drawDisassemblyControls(Chip8CoreBase* chip8);

private:
    MemoryEditor m_memoryEditor;
    Chip8Disassembler m_disassembler;

    std::array<ImGuiMenuItemWindow<Chip8CoreBase>, 6> m_menuItems = {
        "Registers", INITIAL_WINDOW_STATE, [this](Chip8CoreBase* chip8) { drawRegisters(chip8); },
        "Stack", INITIAL_WINDOW_STATE, [this](Chip8CoreBase* chip8) { drawStack(chip8); },
        "Memory Editor", INITIAL_WINDOW_STATE, [this](Chip8CoreBase* chip8) { drawMemory(chip8); },
        "Keypad", INITIAL_WINDOW_STATE, [this](Chip8CoreBase* chip8) { drawKeypad(chip8); },
        "Assembly", INITIAL_WINDOW_STATE, [this](Chip8CoreBase* chip8) { drawAssembly(chip8); },
        "Assembly Controls", INITIAL_WINDOW_STATE, [this](Chip8CoreBase* chip8) { drawDisassemblyControls(chip8); }
    };
};

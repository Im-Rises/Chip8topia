#pragma once

#include <imgui.h>
#include <imgui_memory_editor/imgui_memory_editor.h>
#include <array>

#include "../../Chip8Emulator/Chip8Emulator.h"
#include "../../Chip8Emulator/ChipCores/Chip8Core/Chip8Core.h"
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
    void closeAllWindows();

private:
    void drawRegisters(Chip8CoreBase* chip8);
    void drawStack(Chip8CoreBase* chip8);
    void drawMemory(Chip8CoreBase* chip8);
    void drawKeypad(Chip8CoreBase* chip8);
    void drawDisassembly(Chip8Emulator* emulator);
    void drawDisassemblyControls(Chip8Emulator* emulator);

private:
    MemoryEditor m_memoryEditor;
    Chip8Disassembler m_disassembler;

    std::array<ImGuiMenuItemWindow<Chip8Emulator>, 7> m_menuItems = {
        "Registers", INITIAL_WINDOW_STATE, [this](Chip8Emulator* emulator) { drawRegisters(emulator->getChip8Core()); },
        "Stack", INITIAL_WINDOW_STATE, [this](Chip8Emulator* emulator) { drawStack(emulator->getChip8Core()); },
        "Memory Editor", INITIAL_WINDOW_STATE, [this](Chip8Emulator* emulator) { drawMemory(emulator->getChip8Core()); },
        "Keypad", INITIAL_WINDOW_STATE, [this](Chip8Emulator* emulator) { drawKeypad(emulator->getChip8Core()); },
        "Assembly", INITIAL_WINDOW_STATE, [this](Chip8Emulator* emulator) { drawDisassembly(emulator); },
        "Assembly Controls", INITIAL_WINDOW_STATE, [this](Chip8Emulator* emulator) { drawDisassemblyControls(emulator); },
        "Breakpoints", INITIAL_WINDOW_STATE, [this](Chip8Emulator* emulator) { m_disassembler.drawBreakpoints(emulator); }
    };
};

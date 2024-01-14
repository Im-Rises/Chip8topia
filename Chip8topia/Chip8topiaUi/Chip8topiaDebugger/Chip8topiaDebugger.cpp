#include "Chip8topiaDebugger.h"

#include "imgui/imgui.h"

void Chip8topiaDebugger::drawDebuggerMenu() {
    if (ImGui::BeginMenu("Debugger"))
    {
        drawRegisters();
        drawMemory();
        drawStack();
        drawKeyboard();
        drawDisassembler();

        ImGui::EndMenu();
    }
}

void Chip8topiaDebugger::drawRegisters() {
    if (ImGui::BeginMenu("Registers"))
    {
        ImGui::EndMenu();
    }
}

void Chip8topiaDebugger::drawMemory() {
    if (ImGui::BeginMenu("Memory"))
    {
        ImGui::EndMenu();
    }
}

void Chip8topiaDebugger::drawStack() {
    if (ImGui::BeginMenu("Stack"))
    {
        ImGui::EndMenu();
    }
}

void Chip8topiaDebugger::drawKeyboard() {
    if (ImGui::BeginMenu("Keyboard"))
    {
        ImGui::EndMenu();
    }
}

void Chip8topiaDebugger::drawDisassembler() {
    if (ImGui::BeginMenu("Disassembler"))
    {
        ImGui::EndMenu();
    }
}

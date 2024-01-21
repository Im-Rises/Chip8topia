#include "Chip8topiaDebugger.h"

#include "imgui/imgui.h"
#include <binaryLib/binaryLib.h>

void Chip8topiaDebugger::drawDebuggerMenu() {
    if (ImGui::BeginMenu("Debugger"))
    {
        drawRegisters();
        drawStack();
        drawMemory();
        drawKeyboard();
        drawDisassembler();

        ImGui::EndMenu();
    }
}

void Chip8topiaDebugger::drawRegisters() {
    if (ImGui::BeginMenu("Registers"))
    {
        // Print PC register
        //        ImGui::Text("PC: %04X", m_cpu.getProgramCounter());

        // Print I register
        //        ImGui::Text("I: %04X", m_cpu.getIRegister());

        // Print Game Timer
        //        ImGui::Text("Game Timer: %02X", m_cpu.getGameTimer());

        // Print Sound Timer
        //        ImGui::Text("Sound Timer: %02X", m_cpu.getSoundTimer());

        // Print V register using a for loop
        //       for (auto i = 0; i < 16; ++i)
        //       {
        //           ImGui::Text("V%01X: %02X", i, m_cpu.getVRegister(i));
        //       }
        ImGui::EndMenu();
    }
}

void Chip8topiaDebugger::drawStack() {
    if (ImGui::BeginMenu("Stack"))
    {
        // Print SP register
        //        ImGui::Text("SP: %02X", m_cpu.getStackPointer());
        //

        // Print Stack using a for loop in a table
        //        ImGui::Text("Stack");
        //        ImGui::BeginTable("Stack", 2);
        //        for (auto i = 0; i < 16; ++i)
        //        {
        //            ImGui::TableNextRow();
        //            ImGui::TableSetColumnIndex(0);
        //            ImGui::Text("SP: %02X", i);
        //            ImGui::TableSetColumnIndex(1);
        //            ImGui::Text("0x%04X", m_cpu.getStack(i));
        //        }
        ImGui::EndMenu();
    }
}

void Chip8topiaDebugger::drawMemory() {
    if (ImGui::BeginMenu("Memory"))
    {
        m_memoryEditor.DrawWindow("Memory Editor", m_cpu.getMemory(), 0x1000);
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

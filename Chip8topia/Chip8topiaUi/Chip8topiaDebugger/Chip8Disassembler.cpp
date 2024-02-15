#include "Chip8Disassembler.h"

#include <imgui.h>
#include <string>
#include <format>

#include "../Chip8Emulator/Chip8Core/Core/CpuDisassembly.h"

void Chip8Disassembler::drawAssembly(const std::array<uint8, Cpu::MEMORY_SIZE>& memory, uint16 pc) {
    // TODO: Modify assembly to put real value for X, Y, N, NN, NNN and KK
    ImGuiListClipper clipper;
    clipper.Begin(Cpu::MEMORY_SIZE - 1);
    while (clipper.Step())
    {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            uint16 opcode = (memory[i] << 8) | memory[i + 1];

            if (pc == i)
            {
                ImGui::Text("> 0x%04X: (%04X) %s", i, opcode, CpuDisassembly::disassembleOpcode(opcode).c_str());
            }
            else if (m_breakpoints[i])
            {
                ImGui::Text("* 0x%04X: (%04X) %s", i, opcode, CpuDisassembly::disassembleOpcode(opcode).c_str());
            }
            else
            {
                ImGui::Text("  0x%04X: (%04X) %s", i, opcode, CpuDisassembly::disassembleOpcode(opcode).c_str());
            }

            if (ImGui::IsItemClicked())
            {
                m_breakpoints[i] = !m_breakpoints[i];
            }
        }
    }
}

void Chip8Disassembler::drawAssemblyControls() {
    if (ImGui::Button("Step"))
    {
    }

    ImGui::SameLine();

    if (ImGui::Button("Run"))
    {
    }

    ImGui::SameLine();

    if (ImGui::Button("Break"))
    {
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset"))
    {
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
    }

    ImGui::SameLine();

    if (ImGui::Button("Load"))
    {
    }

    ImGui::SameLine();

    if (ImGui::Button("Save"))
    {
    }
}

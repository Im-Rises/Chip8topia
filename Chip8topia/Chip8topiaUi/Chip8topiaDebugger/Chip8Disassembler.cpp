#include "Chip8Disassembler.h"

#include <imgui.h>
#include <string>
#include <format>

#include "../Chip8Emulator/Chip8Core/Core/CpuDisassembly.h"

void Chip8Disassembler::drawAssembly(const std::array<uint8, Cpu::MEMORY_SIZE>& memory, uint16 pc) {
    // TODO: Optimise to use a string with specific size and only modify its content not change its size every time

    std::string opcodeStringBuffer;
    for (int i = Cpu::START_ADDRESS; i < Cpu::MEMORY_SIZE; i += 2)
    {
        uint16 opcode = (memory[i] << 8) | memory[i + 1];

        if (pc == i)
        {
            opcodeStringBuffer += "> ";
        }
        else if (m_breakpoints[i])
        {
            opcodeStringBuffer += "* ";
        }
        else
        {
            opcodeStringBuffer += "  ";
        }

        opcodeStringBuffer += std::format("0x{:04X}: ({:04X}) {}\n", i, opcode, CpuDisassembly::disassembleOpcode(opcode));
        //        opcodeStringBuffer += std::format("0x{:04X}: ({:04X})\n", i, opcode);

        ImGui::Selectable(opcodeStringBuffer.c_str(), m_breakpoints[i], ImGuiSelectableFlags_AllowDoubleClick);

        if (ImGui::IsItemClicked())
        {
            m_breakpoints[i] = !m_breakpoints[i];
        }

        opcodeStringBuffer.clear();
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

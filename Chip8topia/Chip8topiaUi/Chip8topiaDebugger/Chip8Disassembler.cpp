#include "Chip8Disassembler.h"

#include <imgui.h>
#include <fmt/format.h>

#include "../Chip8Emulator/Chip8Core/Core/CpuDisassembly.h"

void Chip8Disassembler::drawAssembly(const std::array<uint8, Cpu::MEMORY_SIZE>& memory, uint16 pc) {
    // Maybe Change the storage to use real bool not a bitset ? This way we don't need the ImGui::IsItemClicked() and we can use directly the value of the array
    std::string buffer;
    ImGuiListClipper clipper;
    clipper.Begin(Cpu::MEMORY_SIZE - 1);
    while (clipper.Step())
    {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            uint16 opcode = (memory[i] << 8) | memory[i + 1];

            buffer = fmt::format("  0x{:04X}: ({:04X}) {}", i, opcode, CpuDisassembly::disassembleOpcode(opcode));
            if (pc == i)
            {
                buffer[0] = '>';
            }
            else if (m_breakpoints[i])
            {
                buffer[0] = '*';
            }

            ImGui::Selectable(buffer.c_str(), m_breakpoints[i]);

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

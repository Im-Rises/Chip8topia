#include "Chip8Disassembler.h"

#include <imgui.h>
#include <string>
#include <format>

void Chip8Disassembler::disasemble(const std::array<uint8, Cpu::MEMORY_SIZE>& memory, uint16 pc) {
    // TODO: Optimise to use a string with specific size and only modify its content not change its size every time
    std::string opcodeStringBuffer;
    for (int i = 0; i < Cpu::MEMORY_SIZE; i += 2)
    {
        uint16 opcode = (memory[i] << 8) | memory[i + 1];

        if (pc == i)
        {
            opcodeStringBuffer += "> ";
        }

        opcodeStringBuffer += std::format("{:04X} {:04X}\n", i, opcode);

        m_disassembly[i] = opcodeStringBuffer;
        //
        //        ImGui::Text(opcodeStringBuffer.c_str());

        opcodeStringBuffer.clear();
    }
}

void Chip8Disassembler::draw() {
    // TODO: Create disassembly window
    ImGui::BeginChild("Disassembly", ImVec2(0, 0));
    for (int i = 0; i < Cpu::ROM_SIZE; i += 2)
    {
        ImGui::Selectable(m_disassembly[i].c_str(), m_breakpoints[i], ImGuiSelectableFlags_AllowDoubleClick);

        if (ImGui::IsItemClicked())
        {
            m_breakpoints[i] = !m_breakpoints[i];
        }
    }
    ImGui::EndChild();
}

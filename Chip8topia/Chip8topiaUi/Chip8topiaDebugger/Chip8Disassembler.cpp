#include "Chip8Disassembler.h"

#include <imgui.h>
#include <fmt/format.h>
#include <iostream>

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"
#include "../Chip8Emulator/Disassembly/CpuDisassembly.h"

void Chip8Disassembler::drawDisassembly(const std::array<uint8, Chip8Cpu::MEMORY_SIZE>& memory, uint16 pc) {
    // Maybe Change the storage to use real bool not a bitset ? This way we don't need the ImGui::IsItemClicked() and we can use directly the value of the array

    bool currentPcInViewport = false;

    std::string buffer;
    ImGuiListClipper clipper;
    clipper.Begin(Chip8Cpu::MEMORY_SIZE - 1);
    while (clipper.Step())
    {
        // TODO: Correct, it should be 2 bytes per opcode (at the moment we are incrementing by 1 so we are printing opcodes that doesn't exist)
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            uint16 opcode = (memory[i] << 8) | memory[i + 1];

            buffer = fmt::format("  0x{:04X}: ({:04X}) {}", i, opcode, CpuDisassembly::disassembleOpcode(opcode));
            if (pc == i)
            {
                buffer[0] = '>';
                currentPcInViewport = true;
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

    if (m_breakpoints[pc] && m_previousPC != pc)
    {
        std::cout << "Breakpoint hit at " << std::hex << pc << std::endl;
        Chip8topiaInputHandler::getInstance().m_BreakEmulationEvent.trigger();
        ImGui::SetScrollY(pc * (ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y));
    }

    if (m_previousPC != pc && m_followPC && !currentPcInViewport)
    {
        ImGui::SetScrollY(pc * (ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y));
    }

    m_previousPC = pc;
}

void Chip8Disassembler::drawDisassemblyControls() {
    // TODO: Implement the Step, Run, Break, Load and Save buttons

    ImGui::Checkbox("Follow PC", &m_followPC);

    if (ImGui::Button("Step"))
    {
        Chip8topiaInputHandler::getInstance().m_StepEmulationEvent.trigger();
    }

    ImGui::SameLine();

    if (ImGui::Button("Run"))
    {
        Chip8topiaInputHandler::getInstance().m_RunEmulationEvent.trigger();
    }

    ImGui::SameLine();

    if (ImGui::Button("Break"))
    {
        Chip8topiaInputHandler::getInstance().m_BreakEmulationEvent.trigger();
    }

    ImGui::SameLine();

    if (ImGui::Button("Restart"))
    {
        Chip8topiaInputHandler::getInstance().m_RestartEmulationEvent.trigger();
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        clearBreakpoints();
    }

    ImGui::SameLine();

    //    if (ImGui::Button("Load"))
    //    {
    //    }
    //
    //    ImGui::SameLine();
    //
    //    if (ImGui::Button("Save"))
    //    {
    //    }
}

void Chip8Disassembler::clearBreakpoints() {
    m_breakpoints.reset();
}

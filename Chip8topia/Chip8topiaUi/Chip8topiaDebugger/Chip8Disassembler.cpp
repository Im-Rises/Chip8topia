#include "Chip8Disassembler.h"

#include <imgui.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"
#include "../Chip8Emulator/Disassembly/CpuDisassembly.h"

void Chip8Disassembler::drawDisassembly(const std::array<uint8, Chip8Cpu::MEMORY_SIZE>& memory, uint16 pc) {
    // Maybe Change the storage to use real bool not a bitset ? This way we don't need the ImGui::IsItemClicked() and we can use directly the value of the array

    static constexpr int OPCODE_SIZE = 2;

    bool currentPcInViewport = false;

    std::string buffer;
    ImGuiListClipper clipper;
    clipper.Begin(Chip8Cpu::MEMORY_SIZE / OPCODE_SIZE);
    while (clipper.Step())
    {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            const int memoryIndex = i * OPCODE_SIZE;
            uint16 opcode = (memory[memoryIndex] << 8) | memory[(memoryIndex) + 1];

            buffer = fmt::format("  0x{:04X}: ({:04X}) {}", memoryIndex, opcode, CpuDisassembly::disassembleOpcode(opcode));

            if (pc == memoryIndex)
            {
                buffer[0] = '>';
                currentPcInViewport = true;
            }
            else if (m_breakpoints[memoryIndex])
            {
                buffer[0] = '*';
            }

            ImGui::Selectable(buffer.c_str(), m_breakpoints[memoryIndex], ImGuiSelectableFlags_AllowDoubleClick);

            if (ImGui::IsItemClicked())
            {
                m_breakpoints[memoryIndex] = !m_breakpoints[memoryIndex];
            }
        }
    }

    if (m_breakpoints[pc] && m_previousPC != pc)
    {
        spdlog::info("Breakpoint hit at 0x{:04X}", pc);
        Chip8topiaInputHandler::getInstance().m_BreakEmulationEvent.trigger();
        ImGui::SetScrollY((static_cast<float>(pc) / OPCODE_SIZE) * (ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y));
    }

    if (m_previousPC != pc && m_followPC && !currentPcInViewport)
    {
        ImGui::SetScrollY((static_cast<float>(pc) / OPCODE_SIZE) * (ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y));
    }

    m_previousPC = pc;
}

void Chip8Disassembler::drawDisassemblyControls() {
    // TODO: Implement the Step, Run, Break, Load and Save buttons

    ImGui::Checkbox("Follow PC", &m_followPC);

    if (ImGui::Button("Break"))
    {
        Chip8topiaInputHandler::getInstance().m_BreakEmulationEvent.trigger();
    }

    ImGui::SameLine();

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

    if (ImGui::Button("Restart"))
    {
        Chip8topiaInputHandler::getInstance().m_RestartEmulationEvent.trigger();
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear Breakpoints"))
    {
        clearBreakpoints();
    }

    //    ImGui::SameLine();

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

void Chip8Disassembler::drawBreakpoints() {
    // TODO: Add a move to the breakpoint when clicked
    // TODO: Maybe move this to the draw disassembly controls

    if (m_breakpoints.none())
    {
        ImGui::Text("No breakpoints");
        return;
    }

    ImGuiListClipper clipper;
    clipper.Begin(Chip8Cpu::MEMORY_SIZE);
    while (clipper.Step())
    {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            if (m_breakpoints[i])
            {
                if (ImGui::IsItemClicked())
                {
                    ImGui::Text("0x{:04X}", i);
                    m_breakpoints[i] = false;
                }
            }
        }
    }
}

void Chip8Disassembler::clearBreakpoints() {
    m_breakpoints.reset();
}

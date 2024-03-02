#include "Chip8Disassembler.h"

#include <imgui.h>
#include <fmt/format.h>
#if !defined(BUILD_RELEASE)
#include <spdlog/spdlog.h>
#endif

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"
#include "../Chip8Emulator/Disassembly/CpuDisassembly.h"
#include "../../Chip8Emulator/Chip8Emulator.h"

void Chip8Disassembler::drawDisassembly(Chip8Emulator* emulator) {
    // Maybe Change the storage to use real bool not a bitset ? This way we don't need the ImGui::IsItemClicked() and we can use directly the value of the array

    std::array<uint8, CpuBase::MEMORY_SIZE>& memory = emulator->getChip8Core()->getCpu()->getMemory();
    std::bitset<CpuBase::MEMORY_SIZE>& m_breakpoints = emulator->getBreakpoints();
    uint16 pc = emulator->getChip8Core()->getCpu()->getPc();

    static constexpr int OPCODE_SIZE = 2; // TODO: Move to the Chip8BaseCpu class

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

    if (m_previousPc != pc && m_followPc && !currentPcInViewport)
    {
        ImGui::SetScrollY((static_cast<float>(pc) / OPCODE_SIZE) * (ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y));
    }

    if (m_requestMoveToPc)
    {
        m_requestMoveToPc = false;
        ImGui::SetScrollY((static_cast<float>(m_requestedPc) / OPCODE_SIZE) * (ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y));
    }

    m_previousPc = pc;
}

void Chip8Disassembler::drawDisassemblyControls(Chip8Emulator* emulator) {
    Chip8topiaInputHandler& inputHandler = Chip8topiaInputHandler::getInstance();

    ImGui::Checkbox("Follow PC", &m_followPc);

    ImGui::SameLine();

    ImGui::Checkbox("Can break", emulator->getCanBreak());

    if (ImGui::Button("Break"))
    {
        inputHandler.m_BreakEmulationEvent.trigger();
    }

    ImGui::SameLine();

    if (ImGui::Button("Step"))
    {
        inputHandler.m_StepEmulationEvent.trigger();
    }

    ImGui::SameLine();

    if (ImGui::Button("Run"))
    {
        inputHandler.m_RunEmulationEvent.trigger();
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear Breakpoints"))
    {

        inputHandler.m_ClearBreakpointsEvent.trigger();
    }

    ImGui::SameLine();

    if (ImGui::Button("Restart"))
    {
        inputHandler.m_RestartEmulationEvent.trigger();
    }
}

void Chip8Disassembler::drawBreakpoints(Chip8Emulator* emulator) {
    // TODO: Maybe move this to the draw disassembly controls
    // TODO: Add breakpoints move to the breakpoint when clicked

    std::bitset<CpuBase::MEMORY_SIZE>& breakpoints = emulator->getBreakpoints();

    if (ImGui::BeginTable("Breakpoints", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Address");
        ImGui::TableSetupColumn("Move to");
        ImGui::TableSetupColumn("Remove");
        ImGui::TableHeadersRow();

        for (int i = 0; i < CpuBase::MEMORY_SIZE; i++)
        {
            if (breakpoints[i])
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", fmt::format("0x{:04X}", i).c_str());
                ImGui::TableSetColumnIndex(1);
                if (ImGui::Button(fmt::format("Goto##{}", i).c_str()))
                {
                    m_requestMoveToPc = true;
                    m_requestedPc = i;
                    m_followPc = false;
                }
                ImGui::TableSetColumnIndex(2);
                if (ImGui::Button(fmt::format("X##{}", i).c_str()))
                {
                    breakpoints[i] = false;
                }
            }
        }

        ImGui::EndTable();
    }
}

#include "Chip8topiaDisassembler.h"

#include <imgui.h>
#include <fmt/format.h>
#include <IconsFontAwesome6.h>

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"
#include "../Chip8Emulator/Disassembly/Chip8CpuDisassembly.h"
#include "../Chip8Emulator/Disassembly/SChip11CpuDisassembly.h"
#include "../Chip8Emulator/Disassembly/SChipCCpuDisassembly.h"
#include "../Chip8Emulator/Disassembly/XoChipCpuDisassembly.h"
#include "../../Chip8Emulator/Chip8Emulator.h"
#include "../../Chip8Emulator/ChipCores/SChip11Core/Core/SChip11Cpu.h"

void Chip8topiaDisassembler::drawDisassembly(Chip8Emulator* emulator)
{
    const std::array<uint8, CpuBase::MEMORY_SIZE>& memory = emulator->getChip8Core()->getCpu()->getMemory();
    std::set<uint16>& m_breakpoints = emulator->getBreakpoints();
    const uint16 pc = emulator->getChip8Core()->getCpu()->getPc();

    std::function<std::string(const uint16 opcode)> disassembler;
    switch (emulator->getCoreType())
    {
    case Chip8CoreType::Chip8:
        disassembler = Chip8CpuDisassembly::disassembleOpcode;
        break;
    case Chip8CoreType::SChip11Legacy:
    case Chip8CoreType::SChip11Modern:
        disassembler = [&](const uint16 opcode)
        { return SChip11CpuDisassembly::disassembleOpcode(opcode, dynamic_cast<SChip11Cpu*>(emulator->getChip8Core()->getCpu().get())->getIsModernMode(), emulator->getChip8Core()->getPpu()->getMode() == PpuBase::PpuMode::LORES); };
        break;
    case Chip8CoreType::SChipC:
        disassembler = [&](const uint16 opcode)
        { return SChipCCpuDisassembly::disassembleOpcode(opcode, emulator->getChip8Core()->getPpu()->getMode() == PpuBase::PpuMode::LORES); };
        break;
    case Chip8CoreType::XoChip:
        disassembler = XoChipCpuDisassembly::disassembleOpcode;
        break;
    }

    bool currentPcInViewport = false;

    std::string buffer;
    ImGuiListClipper clipper;
    clipper.Begin(Chip8Cpu::MEMORY_SIZE - 1);
    while (clipper.Step())
    {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            const int memoryIndex = i;
            uint16 opcode = (memory[memoryIndex] << 8) | memory[(memoryIndex) + 1];

            buffer = fmt::format("  0x{:04X}: ({:04X}) {}", memoryIndex, opcode, disassembler(opcode));

            const bool breakpointThisPc = m_breakpoints.find(memoryIndex) != m_breakpoints.end();

            if (pc == memoryIndex)
            {
                buffer[0] = '>';
                currentPcInViewport = true;
            }
            else if (breakpointThisPc)
            {
                buffer[0] = '*';
            }

            ImGui::Selectable(buffer.c_str(), breakpointThisPc, ImGuiSelectableFlags_AllowDoubleClick);

            if (ImGui::IsItemClicked())
            {
                if (breakpointThisPc)
                {
                    m_breakpoints.erase(memoryIndex);
                }
                else
                {
                    m_breakpoints.insert(memoryIndex);
                }
            }
        }
    }

    if (m_previousPc != pc && m_followPc && !currentPcInViewport)
    {
        ImGui::SetScrollY((static_cast<float>(pc)) * (ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y));
    }

    if (m_requestMoveToPc)
    {
        m_requestMoveToPc = false;
        m_followPc = false;
        ImGui::SetScrollY((static_cast<float>(m_requestedPc)) * (ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y));
    }

    m_previousPc = pc;
}

void Chip8topiaDisassembler::drawDisassemblyControls(Chip8Emulator* emulator)
{
    Chip8topiaInputHandler& inputHandler = Chip8topiaInputHandler::getInstance();

    ImGui::Text("State: %s", emulator->getIsBreak() ? "Break" : "Running");

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

void Chip8topiaDisassembler::drawBreakpoints(Chip8Emulator* emulator)
{
    std::set<uint16>& breakpoints = emulator->getBreakpoints();

    if (ImGui::BeginTable("Breakpoints", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Address");
        ImGui::TableSetupColumn("Move to");
        ImGui::TableSetupColumn("Remove");
        ImGui::TableHeadersRow();

        int breakpointToRemove = -1;

        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(breakpoints.size()));
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            {
                const uint16 breakpoint = *std::next(breakpoints.begin(), i);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", fmt::format("0x{:04X}", breakpoint).c_str());
                ImGui::TableSetColumnIndex(1);

                ImGui::PushID(breakpoint);
                if (ImGui::Button(ICON_FA_ARROW_RIGHT))
                {
                    m_requestMoveToPc = true;
                    m_requestedPc = breakpoint;
                    m_followPc = false;
                }
                ImGui::PopID();
                ImGui::TableSetColumnIndex(2);
                if (ImGui::Button(fmt::format(ICON_FA_XMARK "##{}", breakpoint).c_str()))
                {
                    breakpointToRemove = breakpoint;
                }
            }
        }

        if (breakpointToRemove >= 0)
        {
            breakpoints.erase(breakpointToRemove);
        }

        ImGui::EndTable();
    }
}

#include "Chip8topiaDisassembler.h"

#include <imgui.h>
#include <fmt/format.h>
#include <IconsFontAwesome6.h>

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"
#include "../Chip8Emulator/Disassembly/disassemblySettings.h"
#include "../Chip8Emulator/Disassembly/Chip8CpuDisassembly.h"
#include "../Chip8Emulator/Disassembly/SChip11CpuDisassembly.h"
#include "../Chip8Emulator/Disassembly/SChipCCpuDisassembly.h"
#include "../Chip8Emulator/Disassembly/XoChipCpuDisassembly.h"
#include "../../Chip8Emulator/Chip8Emulator.h"
#include "../../Chip8Emulator/ChipCores/SChip11Core/Core/SChip11Cpu.h"

void Chip8topiaDisassembler::drawDisassembly(Chip8Emulator* emulator)
{
    const std::array<uint8, CpuBase::MEMORY_SIZE>& memory = emulator->getChip8Core()->getCpu()->getMemory();
    std::array<bool, CpuBase::MEMORY_SIZE>& breakpoints = emulator->getBreakpoints();
    std::set<uint16>& breakpointsList = emulator->getBreakpointsList();

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
    bool pcIsOdd = pc % 2 != 0;

    // TODO: Disassembly:
    //  - Improve this code to not draw the data read for opcode 0xF000
    //  - Optimize and clean up the code
    //  - Change some opcode names to be more descriptive

    std::string buffer;
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>((Chip8Cpu::MEMORY_SIZE - (pcIsOdd ? 1 : 0)) / OPCODE_SIZE));
    while (clipper.Step())
    {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            const int memoryIndex = i * OPCODE_SIZE + (pcIsOdd ? 1 : 0);

            uint16 opcode = (memory[memoryIndex] << 8) | memory[(memoryIndex) + 1];

            buffer = fmt::format("  0x{:04X}: ({:04X}) {}", memoryIndex, opcode, disassembler(opcode));

            const bool breakpointThisPc = breakpoints[memoryIndex];

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
                breakpoints[memoryIndex] = !breakpoints[memoryIndex];

                if (breakpoints[memoryIndex])
                {
                    breakpointsList.insert(memoryIndex);
                }
                else
                {
                    breakpointsList.erase(memoryIndex);
                }
            }
        }
    }

    if (m_previousPc != pc && m_followPc && !currentPcInViewport)
    {
        setScollToPc(pc);
    }

    if (m_requestMoveToPc)
    {
        m_requestMoveToPc = false;
        m_followPc = false;
        setScollToPc(m_requestedPc);
    }

    if (m_requestMoveToNextPc)
    {
        m_requestMoveToNextPc = false;
        m_followPc = false;
        if (!currentPcInViewport)
        {
            setScollToPc(pc);
        }
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
        requestMoveToPc(emulator->getChip8Core()->getCpu()->getPc());
    }

    ImGui::SameLine();

    if (ImGui::Button("Step"))
    {
        inputHandler.m_StepEmulationEvent.trigger();
        requestMoveToNextPc();
    }

    ImGui::SameLine();

    if (ImGui::Button("Run"))
    {
        inputHandler.m_RunEmulationEvent.trigger();
    }

    ImGui::SameLine();

    if (ImGui::Button("Restart"))
    {
        inputHandler.m_RestartEmulationEvent.trigger();
    }

    ImGui::SameLine();

    if (ImGui::Button("Go to PC"))
    {
        requestMoveToPc(emulator->getChip8Core()->getCpu()->getPc());
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear Breakpoints"))
    {
        inputHandler.m_ClearBreakpointsEvent.trigger();
    }
}

void Chip8topiaDisassembler::drawBreakpoints(Chip8Emulator* emulator)
{
    std::set<uint16>& breakpointsList = emulator->getBreakpointsList();

    if (ImGui::BeginTable("Breakpoints", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Address");
        ImGui::TableSetupColumn("Move to");
        ImGui::TableSetupColumn("Remove");
        ImGui::TableHeadersRow();

        //        int breakpointToRemove = -1;

        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(breakpointsList.size()));
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            {
                const uint16 breakpoint = *std::next(breakpointsList.begin(), i);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", fmt::format("0x{:04X}", breakpoint).c_str());
                ImGui::TableSetColumnIndex(1);

                ImGui::PushID(breakpoint);
                if (ImGui::Button(ICON_FA_ARROW_RIGHT))
                {
                    requestMoveToPc(breakpoint);
                    m_followPc = false;
                }
                ImGui::PopID();
                ImGui::TableSetColumnIndex(2);
                if (ImGui::Button(fmt::format(ICON_FA_XMARK "##{}", breakpoint).c_str()))
                {
                    //                    breakpointToRemove = breakpoint;
                    emulator->getBreakpoints()[breakpoint] = false;
                    breakpointsList.erase(breakpoint);
                }
            }
        }

        //        if (breakpointToRemove >= 0)
        //        {
        //            breakpoints[breakpointToRemove] = false;
        //        }

        ImGui::EndTable();
    }
}

void Chip8topiaDisassembler::requestMoveToPc(uint16 address)
{
    m_requestMoveToPc = true;
    m_requestedPc = address;
}

void Chip8topiaDisassembler::requestMoveToNextPc()
{
    m_requestMoveToNextPc = true;
}

void Chip8topiaDisassembler::setScollToPc(uint16 pc)
{
    ImGui::SetScrollY((static_cast<float>(pc) / OPCODE_SIZE) * (ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y));
}

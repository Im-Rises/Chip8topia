#include <iostream>
#include "Chip8topiaDebugger.h"

#include "../../Chip8Emulator/Chip8Core/Core/Input.h"

void Chip8topiaDebugger::drawDebuggerMenu() {
    if (ImGui::BeginMenu("Debugger"))
    {
        for (auto& menuItem : m_menuItems)
        {
            menuItem.drawMenuItem();
        }

        ImGui::EndMenu();
    }
}

void Chip8topiaDebugger::drawDebuggerWindows(Chip8Emulator& emulator) {
    for (auto& menuItem : m_menuItems)
    {
        menuItem.drawWindow(emulator.getChip8Core());
    }
}

void Chip8topiaDebugger::drawRegisters(Chip8Core* chip8) {
    Cpu& cpu = chip8->getCpu();

    //    ImGui::Indent( 10.0f );
    //    ImGui::AlignTextToFramePadding();
    //    ImGui::PushItemWidth(30.0f);

    ImGui::Text("PC:");
    ImGui::SameLine();
    ImGui::InputScalar("##PC", ImGuiDataType_U16, &cpu.getPc(), nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::Text(" I:");
    ImGui::SameLine();
    ImGui::InputScalar("##I", ImGuiDataType_U16, &cpu.getI(), nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);

    for (auto i = 0; i < Cpu::REGISTER_V_SIZE; i++)
    {
        ImGui::Text("V:");
        ImGui::SameLine();
        ImGui::InputScalar("##V", ImGuiDataType_U8, &cpu.getV()[i], nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);
    }

    ImGui::Text("Game Timer:");
    ImGui::SameLine();
    ImGui::InputScalar("##GameTimer", ImGuiDataType_U8, &cpu.getGameTimer(), nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::Text("Sound Timer:");
    ImGui::SameLine();
    ImGui::InputScalar("##SoundTimer", ImGuiDataType_U8, &cpu.getSoundTimer(), nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);
}

void Chip8topiaDebugger::drawStack(Chip8Core* chip8) {
    Cpu& cpu = chip8->getCpu();

    ImGui::Text("SP:");
    ImGui::SameLine();
    ImGui::InputScalar("##SP", ImGuiDataType_U8, &cpu.getSp(), nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::NewLine();

    if (ImGui::BeginTable("Stack", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Address");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();

        for (auto row = 0; row < Cpu::STACK_SIZE; row++)
        {
            ImGui::TableNextRow();

            if (row == 0)
            {
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(-FLT_MIN); // Right-aligned
            }

            ImGui::PushID(row);
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%02X", row);
            ImGui::TableSetColumnIndex(1);
            ImGui::InputScalar("##Stack", ImGuiDataType_U16, &cpu.getStack()[row], nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);
            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

void Chip8topiaDebugger::drawMemory(Chip8Core* chip8) {
    m_memoryEditor.DrawWindow("Memory Editor", &chip8->getCpu().getMemory(), Cpu::MEMORY_SIZE);
}

void Chip8topiaDebugger::drawKeypad(Chip8Core* chip8) {
    static constexpr int WINDOW_SIZE = 50;
    static constexpr int WINDOW_COUNT_PER_LINE = 4;

    int flags = ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap;
    ImGui::SetWindowFontScale(2.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5F, 0.5F));
    for (int i = 0; i < Input::KEY_COUNT; i++)
    {
        ImGui::PushID(i);
        if (ImGui::Selectable(Input::KEY_NAMES[i], chip8->getInput()->isKeyPressed(Input::KEYS[i]), flags, ImVec2(WINDOW_SIZE, WINDOW_SIZE)))
        {
        }

        if (ImGui::IsItemActivated())
        {
            chip8->getInput()->updateKey(Input::KEYS[i], true);
        }
        else if (ImGui::IsItemDeactivated())
        {
            chip8->getInput()->updateKey(Input::KEYS[i], false);
        }
        ImGui::PopID();

        if ((i % WINDOW_COUNT_PER_LINE) < WINDOW_COUNT_PER_LINE - 1)
        {
            ImGui::SameLine();
        }
    }
    ImGui::PopStyleVar();
    ImGui::SetWindowFontScale(1.0F);
}

void Chip8topiaDebugger::drawDisassembler(Chip8Core* chip8) {
    // TODO: Implement disassembler

    //    Disassembler disassembler;
    //    disassembler.disassemble(chip8->getCpu().m_Memory, chip8->getCpu().m_pc);
    //    disassembler.drawDisassembly();
}

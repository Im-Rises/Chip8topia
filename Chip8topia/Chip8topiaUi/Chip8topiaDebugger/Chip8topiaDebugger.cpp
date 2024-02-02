#include "Chip8topiaDebugger.h"

#include "../../Chip8Emulator/Chip8Core/Core/Input.h"

void Chip8topiaDebugger::drawDebugger(Chip8Emulator& emulator) {
    if (ImGui::BeginMenu("Debugger"))
    {
        for (auto& menuItem : m_menuItems)
        {
            menuItem.drawMenuItem();
        }

        ImGui::EndMenu();
    }

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


    ImGui::Text("Stack");
    ImGui::BeginTable("Stack", 3);
    for (auto i = 0; i < 16; ++i)
    {
        //        ImGui::PushID(i);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("SP: %02X", i);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("0x%04X", cpu.getStack()[i]);
        ImGui::TableSetColumnIndex(2);
        ImGui::InputScalar("##Stack", ImGuiDataType_U16, &cpu.getStack()[i], nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);
        //        ImGui::PopID();
    }

    ImGui::EndTable();
}

void Chip8topiaDebugger::drawMemory(Chip8Core* chip8) {
    m_memoryEditor.DrawWindow("Memory Editor", &chip8->getCpu().getMemory(), Cpu::MEMORY_SIZE);
}

void Chip8topiaDebugger::drawKeyboard(Chip8Core* chip8) {
    ImGui::Text("Keyboard");
    ImGui::BeginTable("Keyboard", 3);
    for (auto i = 0; i < Input::KEY_COUNT; i++)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Key: %02X", i);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("0x%04X", chip8->getInput()->isKeyPressed(i) ? 1 : 0);
        ImGui::TableSetColumnIndex(2);
        ImGui::Button("Toggle Key");
        if (ImGui::IsItemClicked())
        {
            // TODO: Toggle key
            // chip8->getInput()->updateKey(i, true);
        }
    }

    ImGui::EndTable();
}

void Chip8topiaDebugger::drawDisassembler(Chip8Core* chip8) {
    // TODO: Implement disassembler

    //    Disassembler disassembler;
    //    disassembler.disassemble(chip8->getCpu().m_Memory, chip8->getCpu().m_pc);
    //    disassembler.drawDisassembly();
}

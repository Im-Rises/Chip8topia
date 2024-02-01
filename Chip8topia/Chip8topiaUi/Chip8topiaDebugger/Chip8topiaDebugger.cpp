#include "Chip8topiaDebugger.h"

#include "../../Chip8Emulator/Chip8Core/Core/Cpu.h"
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

    ImGui::Text("PC: %04X", cpu.getPc());
    //    ImGui::DragInt("PC", &cpu.getPc(), 1, 0, 0xFFFF);

    ImGui::Text("I: %04X", cpu.getI());
    //    ImGui::DragInt("I", &cpu.getI(), 1, 0, 0xFFFF);

    ImGui::Text("Game Timer: %02X", cpu.getGameTimer());
    //    ImGui::DragInt("Game Timer", &cpu.getGameTimer(), 1, 0, 0xFF);

    ImGui::Text("Sound Timer: %02X", cpu.getAudioTimer());
    //    ImGui::DragInt("Sound Timer", &cpu.getAudioTimer(), 1, 0, 0xFF);

    for (auto i = 0; i < 16; ++i)
    {
        ImGui::Text("V%01X: %02X", i, cpu.getV()[i]);
        //        ImGui::DragInt("V%01X", &cpu.getV()[i], 1, 0, 0xFF);
    }
}

void Chip8topiaDebugger::drawStack(Chip8Core* chip8) {
    Cpu& cpu = chip8->getCpu();

    ImGui::Text("SP: %02X", cpu.getSp());
    //    ImGui::DragInt("SP", &cpu.getSp(), 1, 0, 0xFF);


    ImGui::Text("Stack");
    ImGui::BeginTable("Stack", 2);
    for (auto i = 0; i < 16; ++i)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("SP: %02X", i);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("0x%04X", cpu.getStack()[i]);
    }

    ImGui::EndTable();
}

void Chip8topiaDebugger::drawMemory(Chip8Core* chip8) {
    m_memoryEditor.DrawWindow("Memory Editor", &chip8->getCpu().getMemory(), Cpu::MEMORY_SIZE);
}

void Chip8topiaDebugger::drawKeyboard(Chip8Core* chip8) {
    ImGui::Text("Keyboard");
    ImGui::BeginTable("Keyboard", 2);
    for (auto i = 0; i < Input::KEY_COUNT; i++)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Key: %02X", i);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("0x%04X", chip8->getInput()->isKeyPressed(i) ? 1 : 0);
    }

    ImGui::EndTable();
}

void Chip8topiaDebugger::drawDisassembler(Chip8Core* chip8) {
    //    Disassembler disassembler;
    //    disassembler.disassemble(chip8->getCpu().m_Memory, chip8->getCpu().m_pc);
    //    disassembler.drawDisassembly();
}

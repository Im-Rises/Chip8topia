#include "Chip8topiaDebugger.h"

#include <binaryLib/binaryLib.h>

// #include "../../Chip8Emulator/Chip8Core/Chip8Core.h"

void Chip8topiaDebugger::drawDebugger() {
    if (ImGui::BeginMenu("Debugger"))
    {
        for (auto& menuItem : m_menuItems)
        {
            menuItem.drawMenuItem();
        }

        ImGui::EndMenu();
    }

    // if chip8Core is running show window
    for (auto& menuItem : m_menuItems)
    {
        menuItem.drawWindow();
    }
}

// void Chip8topiaDebugger::drawDebuggerWindows() {
//     //    if (m_isRegistersOpen)
//     //    {
//     //        drawRegisters();
//     //    }
//     //
//     //    if (m_isStackOpen)
//     //    {
//     //        drawStack();
//     //    }
//     //
//     //    if (m_isMemoryEditorOpen)
//     //    {
//     //        drawMemory();
//     //    }
//     //
//     //    if (m_isKeyboardOpen)
//     //    {
//     //        drawKeyboard();
//     //    }
//     //
//     //    if (m_isDisassemblerOpen)
//     //    {
//     //        drawDisassembler();
//     //    }
// }

void Chip8topiaDebugger::drawRegisters() {

    // Print PC register
    //        ImGui::Text("PC: %04X", m_cpu.getProgramCounter());
    //        ImGui::DragInt("PC", &m_cpu.getProgramCounter(), 1, 0, 0xFFFF);

    // Print I register
    //        ImGui::Text("I: %04X", m_cpu.getIRegister());
    //        ImGui::DragInt("I", &m_cpu.getIRegister(), 1, 0, 0xFFFF);

    // Print Game Timer
    //        ImGui::Text("Game Timer: %02X", m_cpu.getGameTimer());
    //        ImGui::DragInt("Game Timer", &m_cpu.getGameTimer(), 1, 0, 0xFF);

    // Print Sound Timer
    //        ImGui::Text("Sound Timer: %02X", m_cpu.getSoundTimer());
    //        ImGui::DragInt("Sound Timer", &m_cpu.getSoundTimer(), 1, 0, 0xFF);

    // Print V register using a for loop
    //       for (auto i = 0; i < 16; ++i)
    //       {
    //           ImGui::Text("V%01X: %02X", i, m_cpu.getVRegister(i));
    // ImGui::DragInt("V%01X", &m_cpu.getVRegister(i), 1, 0, 0xFF);
    //       }

}

void Chip8topiaDebugger::drawStack() {

    // Print SP register
    //        ImGui::Text("SP: %02X", m_cpu.getStackPointer());
    //        ImGui::DragInt("SP", &m_cpu.getStackPointer(), 1, 0, 0xFF);
    //

    // Print Stack using a for loop in a table
    //        ImGui::Text("Stack");
    //        ImGui::BeginTable("Stack", 2);
    //        for (auto i = 0; i < 16; ++i)
    //        {
    //            ImGui::TableNextRow();
    //            ImGui::TableSetColumnIndex(0);
    //            ImGui::Text("SP: %02X", i);
    //            ImGui::TableSetColumnIndex(1);
    //            ImGui::Text("0x%04X", m_cpu.getStack(i));
    //        }

}

void Chip8topiaDebugger::drawMemory() {

    //        m_memoryEditor.DrawWindow("Memory Editor", m_cpu.getMemory(), 0x1000);

}

void Chip8topiaDebugger::drawKeyboard() {

    // Print Keyboard using a for loop in a table
    //        ImGui::Text("Keyboard");
    //        ImGui::BeginTable("Keyboard", 2);
    //        for (auto i = 0; i < 16; ++i)
    //        {
    //            ImGui::TableNextRow();
    //            ImGui::TableSetColumnIndex(0);
    //            ImGui::Text("Key: %02X", i);
    //            ImGui::TableSetColumnIndex(1);
    //            ImGui::Text("0x%04X", m_cpu.getKeyboard(i));
    //        }

}
void Chip8topiaDebugger::drawDisassembler() {

}

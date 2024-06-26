#include "Chip8topiaDebugger.h"

#include <IconsFontAwesome6.h>

#include "../../Chip8Emulator/Chip8CoreBase/Core/Input.h"

void Chip8topiaDebugger::drawDebuggerMenu()
{
    if (ImGui::BeginMenu(ICON_FA_BUG " Debugger"))
    {
        for (auto& menuItem : m_menuItems)
        {
            menuItem.drawMenuItem();
        }

        ImGui::EndMenu();
    }
}

void Chip8topiaDebugger::drawDebuggerWindows(Chip8Emulator& emulator)
{
    for (auto& menuItem : m_menuItems)
    {
        menuItem.drawWindow(&emulator);
    }
}

void Chip8topiaDebugger::closeAllWindows()
{
    for (auto& menuItem : m_menuItems)
    {
        menuItem.m_isOpen = false;
    }
}

void Chip8topiaDebugger::drawRegisters(Chip8CoreBase* chip8)
{
    std::unique_ptr<CpuBase>& cpu = chip8->getCpu();

    ImGui::PushItemWidth(-FLT_MIN);
    ImGui::Text("Screen mode:");
    ImGui::SameLine();
    ImGui::Text("%s", chip8->getPpu()->getMode() == PpuBase::PpuMode::LORES ? "Low resolution" : "High resolution");

    ImGui::NewLine();

    ImGui::Text("Plane mask:");
    ImGui::SameLine();
    ImGui::InputScalar("##PlaneMask", ImGuiDataType_U8, &chip8->getPpu()->getPlaneMask(), nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::Text("PC:");
    ImGui::SameLine();
    ImGui::InputScalar("##PC", ImGuiDataType_U16, &cpu->getPc(), nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::Text(" I:");
    ImGui::SameLine();
    ImGui::InputScalar("##I", ImGuiDataType_U16, &cpu->getI(), nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::Text("DT:");
    ImGui::SameLine();
    ImGui::InputScalar("##DT", ImGuiDataType_U8, &cpu->getDT(), nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::Text("ST:");
    ImGui::SameLine();
    ImGui::InputScalar("##ST", ImGuiDataType_U8, &cpu->getST(), nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::PopItemWidth();

    ImGui::AlignTextToFramePadding();
    ImGui::PushItemWidth(30.0F);
    static constexpr int WINDOW_COUNT_PER_LINE = 4;
    for (auto i = 0; i < Chip8Cpu::REGISTER_V_SIZE; i++)
    {
        ImGui::Text("V%X", i);
        ImGui::SameLine();
        ImGui::PushID(i);
        ImGui::InputScalar("##V", ImGuiDataType_U8, &cpu->getV()[i], nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);
        ImGui::PopID();

        if ((i % WINDOW_COUNT_PER_LINE) < WINDOW_COUNT_PER_LINE - 1)
        {
            ImGui::SameLine();
        }
    }

    ImGui::PopItemWidth();
}

void Chip8topiaDebugger::drawStack(Chip8CoreBase* chip8)
{
    std::unique_ptr<CpuBase>& cpu = chip8->getCpu();

    ImGui::Text("SP:");
    ImGui::SameLine();
    ImGui::InputScalar("##SP", ImGuiDataType_U8, &cpu->getSp(), nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::NewLine();

    if (ImGui::BeginTable("Stack", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Depth");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();

        for (auto row = 0; row < Chip8Cpu::STACK_SIZE; row++)
        {
            ImGui::TableNextRow();

            if (row == 0)
            {
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(-FLT_MIN);
            }

            ImGui::PushID(row);
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%02X", row);
            ImGui::TableSetColumnIndex(1);
            ImGui::InputScalar("##Stack", ImGuiDataType_U16, &cpu->getStack()[row], nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);
            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

void Chip8topiaDebugger::drawMemory(Chip8CoreBase* chip8)
{
    m_memoryEditor.DrawWindow("Memory Editor", &chip8->getCpu()->getMemory(), Chip8Cpu::MEMORY_SIZE);
}

void Chip8topiaDebugger::drawKeypad(Chip8CoreBase* chip8)
{
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

void Chip8topiaDebugger::drawDisassembly(Chip8Emulator* emulator)
{
    m_disassembler.drawDisassembly(emulator);
}

void Chip8topiaDebugger::drawDisassemblyControls(Chip8Emulator* emulator)
{
    m_disassembler.drawDisassemblyControls(emulator);
}

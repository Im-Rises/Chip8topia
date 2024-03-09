#include "Chip8Error.h"

#include <imgui.h>

#include "../../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

Chip8Error::Chip8Error() {
    Chip8topiaInputHandler::getInstance().m_ErrorEvent.subscribe(this, &Chip8Error::triggerError);
}

Chip8Error::~Chip8Error() {
    Chip8topiaInputHandler::getInstance().m_ErrorEvent.unsubscribe(this, &Chip8Error::triggerError);
}

void Chip8Error::triggerError(const std::string& message) {
    m_errorQueue.push(message);
}

void Chip8Error::showError() {
    if (m_errorQueue.empty())
    {
        return;
    }

    ImGui::OpenPopup("Error");
    if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("%s", m_errorQueue.front().c_str());
        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            m_errorQueue.pop();
        }
        ImGui::EndPopup();
    }
}

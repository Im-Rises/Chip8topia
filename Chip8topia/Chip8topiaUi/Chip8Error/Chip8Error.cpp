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
    if (m_errorQueue.size() >= MAX_ERRORS)
    {
        clearErrorQueue();
        m_errorQueue.emplace(TOO_MANY_ERRORS_MESSAGE);
    }
    m_errorQueue.push(message);
}

void Chip8Error::showError() {
    if (m_errorQueue.empty())
    {
        return;
    }

    static constexpr int BUTTON_WIDTH = 120;
    ImGui::OpenPopup(ERROR_TITLE);
    if (ImGui::BeginPopupModal(ERROR_TITLE, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("%s", m_errorQueue.front().c_str());
        ImGui::NewLine();
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - BUTTON_WIDTH) * 0.5f);
        if (ImGui::Button("OK", ImVec2(BUTTON_WIDTH, 0)))
        {
            ImGui::CloseCurrentPopup();
            m_errorQueue.pop();
        }
        ImGui::EndPopup();
    }
}

void Chip8Error::clearErrorQueue() {
    while (!m_errorQueue.empty())
    {
        m_errorQueue.pop();
    }
}

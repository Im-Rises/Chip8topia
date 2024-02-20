#pragma once

#include <imgui.h>
#include <functional>

template <typename... Args>
struct ImGuiMenuItemWindow {
    const char* m_name;
    bool m_isOpen;
    std::function<void(Args*...)> m_drawFunction;

    void drawMenuItem() {
        ImGui::MenuItem(m_name, nullptr, &m_isOpen);
    }

    void drawWindow(Args*... args) {
        if (m_isOpen)
        {
            ImGui::Begin(m_name, &m_isOpen);
            m_drawFunction(args...);
            ImGui::End();
        }
    }
};

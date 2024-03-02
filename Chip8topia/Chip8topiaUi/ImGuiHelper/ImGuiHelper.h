#pragma once

#include <imgui.h>
#include <functional>

// TODO: Create common base class for ImGuiMenuItemWindow and ImGuiMenuItemPopup

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

template <typename... Args>
struct ImGuiMenuItemPopup {
    const char* m_name;
    bool m_isOpen;
    std::function<void(Args*...)> m_drawFunction;

    void drawMenuItem() {
        if (ImGui::MenuItem(m_name))
        {
            m_isOpen = true;
        }
    }

    void drawWindow(Args*... args) {
        if (m_isOpen)
        {
            ImGui::OpenPopup(m_name);
            if (ImGui::BeginPopupModal(m_name, &m_isOpen,
                    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
            {
                m_drawFunction(args...);
                ImGui::EndPopup();
            }
        }
    }
};
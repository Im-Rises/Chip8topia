#pragma once

#include <imgui.h>
#include <functional>

template <typename... Args>
class ImGuiMenuItemBase {
public:
    ImGuiMenuItemBase(const char* name, const bool isOpen, std::function<void(Args*...)> drawFunction)
        : m_name(name), m_isOpen(isOpen), m_drawFunction(drawFunction) {}

    ImGuiMenuItemBase(const ImGuiMenuItemBase&) = delete;
    ImGuiMenuItemBase(ImGuiMenuItemBase&&) = delete;
    auto operator=(const ImGuiMenuItemBase&) -> ImGuiMenuItemBase& = delete;
    auto operator=(ImGuiMenuItemBase&&) -> ImGuiMenuItemBase& = delete;
    virtual ~ImGuiMenuItemBase() = default;

public:
    virtual void drawMenuItem() = 0;
    virtual void drawWindow(Args*... args) = 0;

public:
    const char* m_name;
    bool m_isOpen;
    std::function<void(Args*...)> m_drawFunction;
};

template <typename... Args>
class ImGuiMenuItemWindow : public ImGuiMenuItemBase<Args...> {
public:
    ImGuiMenuItemWindow(const char* name, const bool isOpen, std::function<void(Args*...)> drawFunction)
        : ImGuiMenuItemBase<Args...>(name, isOpen, drawFunction) {}

public:
    void drawMenuItem() override {
        ImGui::MenuItem(this->m_name, nullptr, &this->m_isOpen);
    }

    void drawWindow(Args*... args) override {
        if (this->m_isOpen)
        {
            //            ImGuiWindowClass windowClass;
            //            windowClass.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoDecoration;
            //            windowClass.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoTaskBarIcon;
            //            windowClass.ViewportFlagsOverrideSet = ImGuiViewportFlags_TopMost;
            //            ImGui::SetNextWindowClass(&windowClass);
            ImGui::Begin(this->m_name, &this->m_isOpen);
            this->m_drawFunction(args...);
            ImGui::End();
        }
    }
};

template <typename... Args>
class ImGuiMenuItemPopup : public ImGuiMenuItemBase<Args...> {
public:
    ImGuiMenuItemPopup(const char* name, const bool isOpen, std::function<void(Args*...)> drawFunction)
        : ImGuiMenuItemBase<Args...>(name, isOpen, drawFunction) {}

public:
    void drawMenuItem() override {
        if (ImGui::MenuItem(this->m_name))
        {
            this->m_isOpen = true;
        }
    }

    void drawWindow(Args*... args) override {
        if (this->m_isOpen)
        {
            ImGui::OpenPopup(this->m_name);
            if (ImGui::BeginPopupModal(this->m_name, &this->m_isOpen,
                    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
            {
                this->m_drawFunction(args...);
                ImGui::EndPopup();
            }
        }
    }
};
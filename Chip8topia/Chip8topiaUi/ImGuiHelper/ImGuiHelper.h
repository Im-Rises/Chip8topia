#pragma once

#include <functional>
#include <queue>
#include <string>
#include <imgui.h>

template <typename... Args>
class ImGuiMenuItemBase
{
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
class ImGuiMenuItemWindow : public ImGuiMenuItemBase<Args...>
{
public:
    ImGuiMenuItemWindow(const char* name, const bool isOpen, std::function<void(Args*...)> drawFunction)
        : ImGuiMenuItemBase<Args...>(name, isOpen, drawFunction) {}

public:
    void drawMenuItem() override
    {
        ImGui::MenuItem(this->m_name, nullptr, &this->m_isOpen);
    }

    void drawWindow(Args*... args) override
    {
        if (this->m_isOpen)
        {
            ImGui::Begin(this->m_name, &this->m_isOpen);
            this->m_drawFunction(args...);
            ImGui::End();
        }
    }
};

template <typename... Args>
class ImGuiMenuItemPopup : public ImGuiMenuItemBase<Args...>
{
public:
    ImGuiMenuItemPopup(const char* name, const bool isOpen, std::function<void(Args*...)> drawFunction)
        : ImGuiMenuItemBase<Args...>(name, isOpen, drawFunction) {}

public:
    void drawMenuItem() override
    {
        ImGui::MenuItem(this->m_name);

        if (ImGui::IsItemClicked())
        {
            this->m_isOpen = true;
        }
    }

    void drawWindow(Args*... args) override
    {
        if (this->m_isOpen)
        {
            ImGui::OpenPopup(this->m_name);
            this->m_isOpen = false;
        }

        if (ImGui::BeginPopup(this->m_name, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
        {
            this->m_drawFunction(args...);
            ImGui::EndPopup();
        }
    }
};

class ImGuiMessageQueue
{
private:
    static constexpr const char* const TOO_MANY_MESSAGES_TITLE = "Too many messages";
    static constexpr const char* const TOO_MANY_MESSAGES_MESSAGE = "Skipping next messages.";

    struct MessageData
    {
        const std::string m_title;
        const std::string m_message;
        const std::function<void()> m_callback;

        MessageData(const std::string& title, const std::string& message, std::function<void()> callback)
            : m_title(title), m_message(message), m_callback(std::move(callback)) {}
    };

public:
    explicit ImGuiMessageQueue(const unsigned int maxMessages = 10)
        : MAX_MESSAGES(maxMessages) {}
    ImGuiMessageQueue(const ImGuiMessageQueue&) = delete;
    ImGuiMessageQueue(ImGuiMessageQueue&&) = delete;
    auto operator=(const ImGuiMessageQueue&) -> ImGuiMessageQueue& = delete;
    auto operator=(ImGuiMessageQueue&&) -> ImGuiMessageQueue& = delete;
    ~ImGuiMessageQueue() = default;

public:
    void pushMessage(const std::string& title, const std::string& message, std::function<void()> callback = nullptr)
    {
        if (m_messageQueue.size() >= MAX_MESSAGES && m_canPushMessage)
        {
            m_canPushMessage = false;
            m_messageQueue.emplace(TOO_MANY_MESSAGES_TITLE, TOO_MANY_MESSAGES_MESSAGE, [&]()
                { m_canPushMessage = true; });
        }

        if (m_canPushMessage)
        {
            m_messageQueue.emplace(title, message, std::move(callback));
        }
    }

    void showMessage()
    {
        if (m_messageQueue.empty())
        {
            return;
        }

        static constexpr int BUTTON_WIDTH = 120;
        ImGui::OpenPopup(m_messageQueue.front().m_title.data());
        if (ImGui::BeginPopupModal(m_messageQueue.front().m_title.data(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
        {
            ImGui::Text("%s", m_messageQueue.front().m_message.c_str());
            ImGui::NewLine();
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - BUTTON_WIDTH) * 0.5F);
            if (ImGui::Button("OK", ImVec2(BUTTON_WIDTH, 0)))
            {
                if (m_messageQueue.front().m_callback)
                {
                    m_messageQueue.front().m_callback();
                }

                ImGui::CloseCurrentPopup();
                m_messageQueue.pop();
            }
            ImGui::EndPopup();
        }
    }

    void clearMessageQueue()
    {
        while (!m_messageQueue.empty())
        {
            m_messageQueue.pop();
        }
    }

    [[nodiscard]] auto empty() const -> bool
    {
        return m_messageQueue.empty();
    }

private:
    std::queue<MessageData> m_messageQueue;
    const unsigned int MAX_MESSAGES;
    bool m_canPushMessage = true;
};

// // Rename class...
//
// class ImGuiText
//{
// public:
//     ImGuiText(float x, float y, float fontScale = 1.0F, ImVec4 color = ImVec4(1.0F, 1.0F, 1.0F, 1.0F)) : m_x(x), m_y(y), m_fontScale(fontScale), m_color(color) {}
//     ImGuiText(const ImGuiText&) = delete;
//     ImGuiText(ImGuiText&&) = delete;
//     auto operator=(const ImGuiText&) -> ImGuiText& = delete;
//     auto operator=(ImGuiText&&) -> ImGuiText& = delete;
//     ~ImGuiText() = default;
//
// public:
//     void drawText(const char* text) const
//     {
//         const ImVec2 mainWindowPos = ImGui::GetMainViewport()->Pos;
//         ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + m_x, mainWindowPos.y + m_y), ImGuiCond_Always);
//
//         // ImGuiText:
//         //  - Needs to change the name in ImGui::Begin or set ID manually when using ?
//         //  - SetWindowFontScale seems obsolete
//         ImGui::Begin("Text", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
//         ImGui::SetWindowFontScale(m_fontScale);
//         ImGui::PushStyleColor(ImGuiCol_Text, m_color);
//         ImGui::Text("%s", text);
//         ImGui::PopStyleColor();
//         ImGui::SetWindowFontScale(1.0F);
//         ImGui::End();
//     }
//
//     void setPos(float x, float y)
//     {
//         m_x = x;
//         m_y = y;
//     }
//
//     void setColor(const ImVec4& color)
//     {
//         m_color = color;
//     }
//
// private:
//     float m_x;
//     float m_y;
//     float m_fontScale;
//     ImVec4 m_color;
// };

// class ImGuiFixedWindow (use the one in the Chip8topiaPerformanceMonitor.cpp)

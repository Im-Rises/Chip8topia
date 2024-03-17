#pragma once

#include <functional>
#include <string>
#include <queue>

#include "../ImGuiHelper/ImGuiHelper.h"

class Chip8topiaMessageHandler
{
public:
    Chip8topiaMessageHandler();
    Chip8topiaMessageHandler(const Chip8topiaMessageHandler&) = delete;
    Chip8topiaMessageHandler(Chip8topiaMessageHandler&&) = delete;
    auto operator=(const Chip8topiaMessageHandler&) -> Chip8topiaMessageHandler& = delete;
    auto operator=(Chip8topiaMessageHandler&&) -> Chip8topiaMessageHandler& = delete;
    ~Chip8topiaMessageHandler();

public:
    void triggerError(const std::string& message, std::function<void()> callback = nullptr);
    void triggerWarning(const std::string& message, std::function<void()> callback = nullptr);
    void triggerInfo(const std::string& message, std::function<void()> callback = nullptr);
    void triggerSuccess(const std::string& message, std::function<void()> callback = nullptr);
    void showMessages();

private:
    void clearMessageQueue();

private:
    ImGuiMessageQueue m_messageQueue;
};

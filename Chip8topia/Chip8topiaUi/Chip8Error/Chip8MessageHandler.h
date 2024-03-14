#pragma once

#include <functional>
#include <string>
#include <queue>

#include "../ImGuiHelper/ImGuiHelper.h"

class Chip8MessageHandler {
public:
    Chip8MessageHandler();
    Chip8MessageHandler(const Chip8MessageHandler&) = delete;
    Chip8MessageHandler(Chip8MessageHandler&&) = delete;
    auto operator=(const Chip8MessageHandler&) -> Chip8MessageHandler& = delete;
    auto operator=(Chip8MessageHandler&&) -> Chip8MessageHandler& = delete;
    ~Chip8MessageHandler();

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

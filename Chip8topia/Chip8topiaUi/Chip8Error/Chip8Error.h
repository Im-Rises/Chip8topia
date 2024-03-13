#pragma once

#include <functional>
#include <string>
#include <queue>

class Chip8Error {
private:
    static constexpr auto ERROR_TITLE = "Error";
    static constexpr auto MAX_ERRORS = 10;
    static constexpr auto TOO_MANY_ERRORS_MESSAGE = "Too many errors, please restart the application.";

private:
    struct ErrorData {
        //        std::string title = ERROR_TITLE;
        std::string message;
        std::function<void()> callback;
    };

public:
    Chip8Error();
    Chip8Error(const Chip8Error&) = delete;
    Chip8Error(Chip8Error&&) = delete;
    Chip8Error& operator=(const Chip8Error&) = delete;
    Chip8Error& operator=(Chip8Error&&) = delete;
    ~Chip8Error();

public:
    void triggerError(const std::string& message, std::function<void()> callback = nullptr);
    void showError();

private:
    void clearErrorQueue();

private:
    std::queue<ErrorData> m_errorQueue;
};

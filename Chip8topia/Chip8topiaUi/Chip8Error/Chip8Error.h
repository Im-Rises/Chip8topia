#pragma once

#include <string>
#include <queue>

class Chip8Error {
public:
    Chip8Error();
    Chip8Error(const Chip8Error&) = delete;
    Chip8Error(Chip8Error&&) = delete;
    Chip8Error& operator=(const Chip8Error&) = delete;
    Chip8Error& operator=(Chip8Error&&) = delete;
    ~Chip8Error();

public:
    void triggerError(const std::string& message);
    void showError();

private:
    std::queue<std::string> m_errorQueue;
};

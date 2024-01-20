#pragma once

class Chip8topiaInputHandler {
    public:
        Chip8topiaInputHandler() = default;
        Chip8topiaInputHandler(const Chip8topiaInputHandler&) = delete;
        auto operator=(const Chip8topiaInputHandler&) -> Chip8topiaInputHandler& = delete;
        Chip8topiaInputHandler(Chip8topiaInputHandler&&) = delete;
        auto operator=(Chip8topiaInputHandler&&) -> Chip8topiaInputHandler& = delete;
        ~Chip8topiaInputHandler() = default;

    public:
        void handleInputs();
};

#pragma once

class Chip8Core {
public:
    Chip8Core() = default;
    Chip8Core(const Chip8Core&) = delete;
    Chip8Core(Chip8Core&&) = delete;
    auto operator=(const Chip8Core&) -> Chip8Core& = delete;
    auto operator=(Chip8Core&&) -> Chip8Core& = delete;
    ~Chip8Core() = default;

public:

};

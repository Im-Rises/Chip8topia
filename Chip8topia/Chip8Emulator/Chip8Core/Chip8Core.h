#pragma once

#include "../Chip8CoreBase/Chip8CoreBase.h"

class Chip8Core final : public Chip8CoreBase {
public:
    Chip8Core();
    Chip8Core(const Chip8Core&) = delete;
    Chip8Core(Chip8Core&&) = delete;
    auto operator=(const Chip8Core&) -> Chip8Core& = delete;
    auto operator=(Chip8Core&&) -> Chip8Core& = delete;
    ~Chip8Core() final = default;

public:
    void clock() final;
};

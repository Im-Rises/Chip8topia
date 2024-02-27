#pragma once

#include "../../Chip8CoreBase/Chip8CoreBase.h"

class Chip8Cpu;
class Chip8Core final : public Chip8CoreBase {
public:
    Chip8Core();
    Chip8Core(const Chip8Core&) = delete;
    Chip8Core(Chip8Core&&) = delete;
    auto operator=(const Chip8Core&) -> Chip8Core& = delete;
    auto operator=(Chip8Core&&) -> Chip8Core& = delete;
    ~Chip8Core() final = default;

public:
    [[nodiscard]] auto getType() const -> Chip8CoreType final { return Chip8CoreType::Chip8; }
    void clock() final;

private:
    Chip8Cpu* m_cpuCasted; // TODO: Find a better solution...
};
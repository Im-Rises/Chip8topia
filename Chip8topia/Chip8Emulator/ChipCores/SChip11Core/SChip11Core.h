#pragma once

#include "../../Chip8CoreBase/Chip8CoreBase.h"

class SChip11Cpu;
class SChip11Core final : public Chip8CoreBase {
public:
    explicit SChip11Core(Chip8Frequency cpuClockFrequency);
    SChip11Core(const SChip11Core&) = delete;
    SChip11Core(SChip11Core&&) = delete;
    auto operator=(const SChip11Core&) -> SChip11Core& = delete;
    auto operator=(SChip11Core&&) -> SChip11Core& = delete;
    ~SChip11Core() final = default;

public:
    [[nodiscard]] auto getType() const -> Chip8CoreType final { return Chip8CoreType::SChip11; }
    [[nodiscard]] auto getConsoleName() const -> const char* final { return "SChip11"; }
    auto clock() -> bool final;

private:
    SChip11Cpu* m_cpuCasted; // TODO: Find a better solution...
};

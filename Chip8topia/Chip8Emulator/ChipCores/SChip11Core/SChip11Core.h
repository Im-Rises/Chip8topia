#pragma once

#include "../../Chip8CoreBase/Chip8CoreBase.h"
#include "Core/SChip11Cpu.h"

// class SChip11Cpu;
class SChip11Core final : public Chip8CoreBase {
public:
    explicit SChip11Core(Chip8Frequency cpuClockFrequency, bool isModernMode);
    SChip11Core(const SChip11Core&) = delete;
    SChip11Core(SChip11Core&&) = delete;
    auto operator=(const SChip11Core&) -> SChip11Core& = delete;
    auto operator=(SChip11Core&&) -> SChip11Core& = delete;
    ~SChip11Core() final = default;

public:
    [[nodiscard]] auto getType() const -> Chip8CoreType final { return m_cpuCasted->getIsModernMode() ? Chip8CoreType::SChip11Modern : Chip8CoreType::SChip11Legacy; }
    [[nodiscard]] auto getConsoleName() const -> const char* final { return m_cpuCasted->getIsModernMode() ? "SuperChip 1.1 (modern)" : "SuperChip 1.1 (legacy)"; }
    auto clock() -> bool final;

private:
    SChip11Cpu* m_cpuCasted;
};

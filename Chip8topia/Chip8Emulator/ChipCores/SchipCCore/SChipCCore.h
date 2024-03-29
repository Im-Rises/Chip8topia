#pragma once

#include "../../Chip8CoreBase/Chip8CoreBase.h"

class SChipCCore final : public Chip8CoreBase {
public:
    explicit SChipCCore(Chip8Frequency cpuClockFrequency);
    SChipCCore(const SChipCCore&) = delete;
    SChipCCore(SChipCCore&&) = delete;
    auto operator=(const SChipCCore&) -> SChipCCore& = delete;
    auto operator=(SChipCCore&&) -> SChipCCore& = delete;
    ~SChipCCore() final = default;

public:
    [[nodiscard]] auto getType() const -> Chip8CoreType final { return Chip8CoreType::SChipC; }
    [[nodiscard]] auto getConsoleName() const -> const char* final { return "SChipC"; }
};

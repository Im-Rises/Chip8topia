#pragma once

#include "../../Chip8CoreBase/Chip8CoreBase.h"

#include <memory>

class SChip11Core final : public Chip8CoreBase {
public:
    SChip11Core();
    SChip11Core(const SChip11Core&) = delete;
    SChip11Core(SChip11Core&&) = delete;
    auto operator=(const SChip11Core&) -> SChip11Core& = delete;
    auto operator=(SChip11Core&&) -> SChip11Core& = delete;
    ~SChip11Core() final = default;

public:
    [[nodiscard]] auto getType() const -> Chip8CoreType final { return Chip8CoreType::SChip11; }
    void clock() final;
};

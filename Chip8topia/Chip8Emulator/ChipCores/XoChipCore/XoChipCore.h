#pragma once

#include "../../Chip8CoreBase/Chip8CoreBase.h"

class XoChipCore final : public Chip8CoreBase {
public:
    explicit XoChipCore(Chip8Frequency cpuClockFrequency);
    XoChipCore(const XoChipCore&) = delete;
    XoChipCore(XoChipCore&&) = delete;
    auto operator=(const XoChipCore&) -> XoChipCore& = delete;
    auto operator=(XoChipCore&&) -> XoChipCore& = delete;
    ~XoChipCore() final = default;

public:
    [[nodiscard]] auto getType() const -> Chip8CoreType final { return Chip8CoreType::XoChip; }
    [[nodiscard]] auto getConsoleName() const -> const char* final { return "XoChip"; }
};

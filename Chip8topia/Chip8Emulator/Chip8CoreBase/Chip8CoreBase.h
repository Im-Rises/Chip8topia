#pragma once

#include <string>
#include <memory>
#include <vector>
#include <binaryLib/binaryLib.h>
#include <functional>

#include "Core/CpuBase.h"

enum class Chip8CoreType : uint8
{
    Chip8,         // Chip-8 (COSMAC VIP)
    SChip11Legacy, // SuperChip 1.1 (legacy)
    SChip11Modern, // SuperChip 1.1 (modern)
    SChipC,        // SuperChipC
    XoChip         // XO-Chip
};

enum class Chip8Frequency : uint32
{
    Freq600Hz = 600,
    Freq1200Hz = 1200,
    Freq1800Hz = 1800,
    Freq1200000Hz = 1200000,  // 1.2MHz for Octo
    Freq24000000Hz = 24000000 // 24MHz for Alien-Inv8sion
};

// class CpuBase;
class Input;
class Chip8CoreBase
{
public:
    static constexpr unsigned int SCREEN_AND_TIMERS_FREQUENCY = 60;

public:
    Chip8CoreBase(Chip8Frequency cpuClockFrequency, std::unique_ptr<CpuBase> cpu, std::shared_ptr<PpuBase> ppu);
    Chip8CoreBase(const Chip8CoreBase&) = delete;
    Chip8CoreBase(Chip8CoreBase&&) = delete;
    auto operator=(const Chip8CoreBase&) -> Chip8CoreBase& = delete;
    auto operator=(Chip8CoreBase&&) -> Chip8CoreBase& = delete;
    virtual ~Chip8CoreBase() = default;

public:
    void reset();
    void readRom(const std::vector<uint8>& rom);
    virtual auto clock() -> bool;
    void updateKey(const uint8 key, const bool pressed);

    [[nodiscard]] virtual auto getType() const -> Chip8CoreType = 0;
    [[nodiscard]] virtual auto getConsoleName() const -> const char* = 0;

    [[nodiscard]] auto getFrequency() const -> Chip8Frequency;
    [[nodiscard]] auto getCpu() -> std::unique_ptr<CpuBase>&;
    [[nodiscard]] auto getPpu() -> std::shared_ptr<PpuBase>&;
    [[nodiscard]] auto getInput() -> std::shared_ptr<Input>&;

protected:
    std::unique_ptr<CpuBase> m_cpu;
    std::shared_ptr<PpuBase> m_ppu;
    std::shared_ptr<Input> m_input;

    unsigned int m_clockCounter;
    const Chip8Frequency CPU_CLOCK_FREQUENCY_ENUM;
    const uint32 CPU_CLOCK_FREQUENCY;
};

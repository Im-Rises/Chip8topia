#pragma once

#include <string>
#include <memory>
#include <vector>
#include <binaryLib/binaryLib.h>

enum class Chip8CoreType : uint8 {
    Chip8,
    SuperChip,
    XoChip
};

class CpuBase;
class PpuBase;
class Input;
class Chip8CoreBase {
public:
    static constexpr unsigned int SCREEN_AND_TIMERS_FREQUENCY = 60;

public:
    Chip8CoreBase(std::unique_ptr<CpuBase> cpu, std::shared_ptr<PpuBase> ppu);
    Chip8CoreBase(const Chip8CoreBase&) = delete;
    Chip8CoreBase(Chip8CoreBase&&) = delete;
    auto operator=(const Chip8CoreBase&) -> Chip8CoreBase& = delete;
    auto operator=(Chip8CoreBase&&) -> Chip8CoreBase& = delete;
    virtual ~Chip8CoreBase() = default;

public:
    [[nodiscard]] virtual auto getType() const -> Chip8CoreType = 0;
    void readRom(const std::vector<uint8>& rom);
    virtual void clock() = 0;
    void updateKey(const uint8 key, const bool pressed);
    void reset();

public:
    [[nodiscard]] auto getCpu() -> std::unique_ptr<CpuBase>& { return m_cpu; }
    [[nodiscard]] auto getPpu() -> std::shared_ptr<PpuBase> { return m_ppu; }
    [[nodiscard]] auto getInput() -> std::shared_ptr<Input> { return m_input; }

protected:
    std::unique_ptr<CpuBase> m_cpu;
    std::shared_ptr<PpuBase> m_ppu;
    std::shared_ptr<Input> m_input;

    unsigned int m_clockCounter;
};

// #pragma once
//
// #include <memory>
//
// class Chip8CoreBase {
// public:
//     Chip8CoreBase(std::unique_ptr<Cpu> cpu, std::shared_ptr<Ppu> ppu);
//     Chip8CoreBase(const Chip8CoreBase&) = delete;
//     Chip8CoreBase(Chip8CoreBase&&) = delete;
//     auto operator=(const Chip8CoreBase&) -> Chip8CoreBase& = delete;
//     auto operator=(Chip8CoreBase&&) -> Chip8CoreBase& = delete;
//     ~Chip8CoreBase() = default;
//
// public:
//     void readRom(const std::vector<uint8>& rom);
//     void clock();
//     void updateKey(const uint8 key, const bool pressed);
//     void reset();
//
// public:
//     auto getCpu() -> Cpu&;
//     auto getPpu() -> std::shared_ptr<Ppu>;
//     auto getInput() -> std::shared_ptr<Input>;
//
// private:
//     std::unique_ptr<CpuBase> m_cpu;     // Different depending on the console version
//     std::shared_ptr<PpuBase> m_ppu;     // Different depending on the console version
//     std::shared_ptr<Input> m_input; // Same for all console versions
//
//     unsigned int m_clockCounter;
// };

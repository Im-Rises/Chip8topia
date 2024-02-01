#pragma once

#include <binaryLib/binaryLib.h>

#include <array>
#include <memory>
#include <vector>

class Ppu;
class Input;

class Cpu {
public: // TODO: Check to be sure between Machine cycle and Clock cycle
    static constexpr uint16 START_ADDRESS = 0x200;
    static constexpr uint16 FONTSET_START_ADDRESS = 0x50;
    static constexpr uint16 FONTSET_SIZE = 80;
    static constexpr uint16 CLOCK_FREQUENCY = 500;
    static constexpr size_t MEMORY_SIZE = 0x1000;
    static constexpr size_t ROM_SIZE = MEMORY_SIZE - START_ADDRESS;
    static constexpr size_t REGISTER_SIZE = 16;
    static constexpr size_t STACK_SIZE = 16;

public:
    Cpu();
    Cpu(const Cpu& other) = delete;
    Cpu(Cpu&& other) = delete;
    auto operator=(const Cpu& other) -> Cpu& = delete;
    auto operator=(Cpu&& other) -> Cpu& = delete;
    ~Cpu() = default;

    void setPpu(std::shared_ptr<Ppu> ppu);
    void setInput(std::shared_ptr<Input> input);

public:
    void reset();
    void readRom(const std::vector<uint8>& rom); // TODO: Maybe change to a std::array<uint8, 4096> rom
    void clock();
    void clockTimers();

private:
    [[nodiscard]] auto fetchOpcode() -> uint16;
    void computeOpcode(const uint16 opcode);

    inline void CLS();
    inline void RET();
    inline void SYS(const uint16 address);
    inline void JP(const uint16 address);
    inline void CALL(const uint16 address);
    inline void SE_Vx_byte(const uint8 x, const uint8 byte);
    inline void SNE_Vx_byte(const uint8 x, const uint8 byte);
    inline void SE_Vx_Vy(const uint8 x, const uint8 y);
    inline void LD_Vx_byte(const uint8 x, const uint8 byte);
    inline void ADD_Vx_byte(const uint8 x, const uint8 byte);
    inline void LD_Vx_Vy(const uint8 x, const uint8 y);
    inline void OR_Vx_Vy(const uint8 x, const uint8 y);
    inline void AND_Vx_Vy(const uint8 x, const uint8 y);
    inline void XOR_Vx_Vy(const uint8 x, const uint8 y);
    inline void ADD_Vx_Vy(const uint8 x, const uint8 y);
    inline void SUB_Vx_Vy(const uint8 x, const uint8 y);
    inline void SHR_Vx(const uint8 x);
    inline void SUBN_Vx_Vy(const uint8 x, const uint8 y);
    inline void SHL_Vx(const uint8 x);
    inline void SNE_Vx_Vy(const uint8 x, const uint8 y);
    inline void LD_I_addr(const uint16 address);
    inline void JP_V0_addr(const uint16 address);
    inline void RND_Vx_byte(const uint8 x, const uint8 byte);
    inline void DRW_Vx_Vy_nibble(const uint8 x, const uint8 y, const uint8 nibble);
    inline void SKP_Vx(const uint8 x);
    inline void SKNP_Vx(const uint8 x);
    inline void LD_Vx_DT(const uint8 x);
    inline void LD_Vx_K(const uint8 x);
    inline void LD_DT_Vx(const uint8 x);
    inline void LD_ST_Vx(const uint8 x);
    inline void ADD_I_Vx(const uint8 x);
    inline void LD_F_Vx(const uint8 x);
    inline void LD_B_Vx(const uint8 x);
    inline void LD_I_Vx(const uint8 x);
    inline void LD_Vx_I(const uint8 x);

public:
    auto getMemory() -> std::array<uint8, MEMORY_SIZE>& { return m_memory; }
    auto getPc() -> uint16& { return m_pc; }

    auto getStack() -> std::array<uint16, STACK_SIZE>& { return m_stack; }
    auto getSp() -> uint8& { return m_sp; }

    auto getV() -> std::array<uint8, REGISTER_SIZE>& { return m_V; }
    auto getI() -> uint16& { return m_I; }

    auto getGameTimer() -> uint8& { return m_gameTimer; }
    auto getAudioTimer() -> uint8& { return m_audioTimer; }

private:
    std::array<uint8, MEMORY_SIZE> m_memory;
    uint16 m_pc;

    std::array<uint16, STACK_SIZE> m_stack;
    uint8 m_sp;

    std::array<uint8, REGISTER_SIZE> m_V;
    uint16 m_I;

    uint8 m_gameTimer;
    uint8 m_audioTimer;

    std::shared_ptr<Ppu> m_ppu;
    std::shared_ptr<Input> m_input;
};

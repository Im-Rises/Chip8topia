#pragma once

#include <memory>
#include <binaryLib/binaryLib.h>
#include <array>
#include <RandomGenerator/RandomGenerator.h>

class PpuBase;
class Input;
class CpuBase {
public:
    static constexpr uint16 START_ADDRESS = 0x200;
    static constexpr uint16 CLOCK_FREQUENCY = 1800; // 600 // 500Hz normally // 100Hz in SCHIP 1.1 validate the display wait
    static constexpr size_t MEMORY_SIZE = 0x1000;
    static constexpr size_t ROM_SIZE = MEMORY_SIZE - START_ADDRESS;
    static constexpr size_t REGISTER_V_SIZE = 16;
    static constexpr size_t STACK_SIZE = 16;

    static constexpr const std::array<uint8, 80> FONTSET = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

public:
    CpuBase();
    CpuBase(const CpuBase&) = delete;
    CpuBase(CpuBase&&) = delete;
    auto operator=(const CpuBase&) -> CpuBase& = delete;
    auto operator=(CpuBase&&) -> CpuBase& = delete;
    virtual ~CpuBase() = default;

    void setPpu(std::shared_ptr<PpuBase> ppu) { m_ppu = std::move(ppu); }
    void setInput(std::shared_ptr<Input> input) { m_input = std::move(input); }

public:
    virtual void reset();
    void readRom(const std::vector<uint8>& rom);
    void clock();
    void clockTimers();

private:
    [[nodiscard]] auto fetchOpcode() -> uint16;

protected:
    virtual void computeOpcode(const uint16 opcode) = 0;

    // TODO: Can it be put inlined?
    // TODO: Check if for all versions of the chip8 they have all these opcodes
    void CLS();                                                                // 00E0
    void RET();                                                                // 00EE
    void JP_addr(const uint16 addr);                                           // 1nnn
    void CALL_addr(const uint16 addr);                                         // 2nnn
    void SE_Vx_nn(const uint8 x, const uint8 nn);                              // 3xnn
    void SNE_Vx_nn(const uint8 x, const uint8 nn);                             // 4xnn
    void SE_Vx_Vy(const uint8 x, const uint8 y);                               // 5xy0
    void LD_Vx_nn(const uint8 x, const uint8 nn);                              // 6xnn
    void ADD_Vx_nn(const uint8 x, const uint8 nn);                             // 7xnn
    void LD_Vx_Vy(const uint8 x, const uint8 y);                               // 8xy0
    virtual void OR_Vx_Vy(const uint8 x, const uint8 y) = 0;                   // 8xy1
    virtual void AND_Vx_Vy(const uint8 x, const uint8 y) = 0;                  // 8xy2
    virtual void XOR_Vx_Vy(const uint8 x, const uint8 y) = 0;                  // 8xy3
    void ADD_Vx_Vy(const uint8 x, const uint8 y);                              // 8xy4
    void SUB_Vx_Vy(const uint8 x, const uint8 y);                              // 8xy5
    void SNE_Vx_Vy(const uint8 x, const uint8 y);                              // 9xy0
    void LD_I_addr(const uint16 addr);                                         // Annn
    void RND_Vx_nn(const uint8 x, const uint8 nn);                             // Cxnn
    virtual void DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) = 0; // Dxyn
    void SKP_Vx(const uint8 x);                                                // Ex9E
    void SKNP_Vx(const uint8 x);                                               // ExA1
    void LD_Vx_DT(const uint8 x);                                              // Fx07
    void LD_Vx_K(const uint8 x);                                               // Fx0A
    void LD_DT_Vx(const uint8 x);                                              // Fx15
    void LD_ST_Vx(const uint8 x);                                              // Fx18
    void ADD_I_Vx(const uint8 x);                                              // Fx1E
    void LD_F_Vx(const uint8 x);                                               // Fx29
    void LD_B_Vx(const uint8 x);                                               // Fx33
    virtual void LD_aI_Vx(const uint8 x) = 0;                                  // Fx55
    virtual void LD_Vx_aI(const uint8 x) = 0;                                  // Fx65


public:
    auto getMemory() -> std::array<uint8, MEMORY_SIZE>& { return m_memory; }
    auto getPc() -> uint16& { return m_pc; }

    auto getStack() -> std::array<uint16, STACK_SIZE>& { return m_stack; }
    auto getSp() -> uint8& { return m_sp; }

    auto getV() -> std::array<uint8, REGISTER_V_SIZE>& { return m_V; }
    auto getI() -> uint16& { return m_I; }

    auto getDT() -> uint8& { return m_DT; }
    auto getST() -> uint8& { return m_ST; }

protected:
    std::array<uint8, MEMORY_SIZE> m_memory;
    uint16 m_pc;

    std::array<uint16, STACK_SIZE> m_stack;
    uint8 m_sp;

    std::array<uint8, REGISTER_V_SIZE> m_V;
    uint16 m_I;

    uint8 m_DT;
    uint8 m_ST;

    std::shared_ptr<PpuBase> m_ppu;
    std::shared_ptr<Input> m_input;

    RandomGenerator m_u8NumberRandomGenerator;
};

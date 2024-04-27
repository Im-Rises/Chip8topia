#pragma once

#include <memory>
#include <binaryLib/binaryLib.h>
#include <array>
#include <RandomGenerator/RandomGenerator.h>
#include <functional>
#include <fmt/format.h>

#include "../chip8Fonts.h"

#if defined(BUILD_PARAM_SAFE)
#define TRIGGER_EMULATION_ERROR(...) throw std::runtime_error(fmt::format(__VA_ARGS__))
#else
#define TRIGGER_EMULATION_ERROR(...) (void)(__VA_ARGS__)
#endif

class PpuBase;
class Input;
class CpuBase
{
public:
    static constexpr uint16 START_ADDRESS = 0x200;
    //    static constexpr size_t MEMORY_SIZE = 0x1000;// Chip8, SCHip11, SCHipC
    static constexpr size_t MEMORY_SIZE = 0x10000; // XoChip
    static constexpr size_t ROM_SIZE = MEMORY_SIZE - START_ADDRESS;
    static constexpr size_t REGISTER_V_SIZE = 16;
    static constexpr size_t STACK_SIZE = 16;

public:
    CpuBase();
    CpuBase(const CpuBase&) = delete;
    CpuBase(CpuBase&&) = delete;
    auto operator=(const CpuBase&) -> CpuBase& = delete;
    auto operator=(CpuBase&&) -> CpuBase& = delete;
    virtual ~CpuBase() = default;

    void setPpu(std::shared_ptr<PpuBase> ppu);
    void setInput(std::shared_ptr<Input> input);

public:
    virtual void reset();
    void readRom(const std::vector<uint8>& rom);
    void clock();
    void clockTimers();

private:
    [[nodiscard]] auto fetchOpcode() -> uint16;

protected:
    [[nodiscard]] auto fetchWord() -> uint16;
    [[nodiscard]] auto readNextWord() -> uint16;
    virtual void skipNextInstruction();
    virtual void haltCurrentInstruction();

    virtual void computeOpcode(const uint16 opcode) = 0;

    virtual void SCD(const uint8 n);                                           // 00CN
    void SCU(const uint8 n);                                                   // 00DN
    void CLS();                                                                // 00E0
    void RET();                                                                // 00EE
    void EXIT();                                                               // 00FD
    virtual void SCR(const uint8 n);                                           // 00FB
    virtual void SCL(const uint8 n);                                           // 00FC
    virtual void LORES();                                                      // 00FE
    virtual void HIRES();                                                      // 00FF
    void SYS(const uint16 address);                                            // 0nnn
    void JP_addr(const uint16 addr);                                           // 1nnn
    void CALL_addr(const uint16 addr);                                         // 2nnn
    virtual void SE_Vx_nn(const uint8 x, const uint8 nn);                      // 3xnn
    virtual void SNE_Vx_nn(const uint8 x, const uint8 nn);                     // 4xnn
    virtual void SE_Vx_Vy(const uint8 x, const uint8 y);                       // 5xy0
    void LD_Vx_nn(const uint8 x, const uint8 nn);                              // 6xnn
    void ADD_Vx_nn(const uint8 x, const uint8 nn);                             // 7xnn
    void LD_Vx_Vy(const uint8 x, const uint8 y);                               // 8xy0
    virtual void OR_Vx_Vy(const uint8 x, const uint8 y);                       // 8xy1
    virtual void AND_Vx_Vy(const uint8 x, const uint8 y);                      // 8xy2
    virtual void XOR_Vx_Vy(const uint8 x, const uint8 y);                      // 8xy3
    void ADD_Vx_Vy(const uint8 x, const uint8 y);                              // 8xy4
    void SUB_Vx_Vy(const uint8 x, const uint8 y);                              // 8xy5
    virtual void SHR_Vx_Vy(const uint8 x, const uint8 y);                      // 8xy6
    void SUBN_Vx_Vy(const uint8 x, const uint8 y);                             // 8xy7
    virtual void SHL_Vx_Vy(const uint8 x, const uint8 y);                      // 8xyE
    virtual void SNE_Vx_Vy(const uint8 x, const uint8 y);                      // 9xy0
    void LD_I_addr(const uint16 addr);                                         // Annn
    void JP_nnn_V0(const uint16 address);                                      // Bnnn
    void JP_xnn_Vx(const uint16 address, const uint8 x);                       // Bxnn
    void RND_Vx_nn(const uint8 x, const uint8 nn);                             // Cxnn
    virtual void DRW_Vx_Vy_n(const uint8 x, const uint8 y, const uint8 n) = 0; // DXYN
    void SKP_Vx(const uint8 x);                                                // Ex9E
    void SKNP_Vx(const uint8 x);                                               // ExA1
    void LD_Vx_DT(const uint8 x);                                              // Fx07
    void LD_Vx_K(const uint8 x);                                               // Fx0A
    void LD_DT_Vx(const uint8 x);                                              // Fx15
    void LD_ST_Vx(const uint8 x);                                              // Fx18
    void ADD_I_Vx(const uint8 x);                                              // Fx1E
    void LD_F_Vx(const uint8 x);                                               // Fx29
    void LD_HF_Vx(const uint8 x);                                              // Fx30
    void LD_B_Vx(const uint8 x);                                               // Fx33
    virtual void LD_aI_Vx(const uint8 x);                                      // Fx55
    virtual void LD_Vx_aI(const uint8 x);                                      // Fx65
    virtual void LD_R_Vx(const uint8 x);                                       // Fx75
    virtual void LD_Vx_R(const uint8 x);                                       // Fx85

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
    std::array<uint8, REGISTER_V_SIZE> m_RPL;
    uint16 m_I;

    uint8 m_DT;
    uint8 m_ST;

    std::shared_ptr<PpuBase> m_ppu;
    std::shared_ptr<Input> m_input;

    RandomGenerator m_u8NumberRandomGenerator;
};

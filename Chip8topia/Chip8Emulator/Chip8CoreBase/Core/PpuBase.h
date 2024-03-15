#pragma once

#include <array>
#include <functional>

#include "CpuBase.h"

class PpuBase
{
public:
    enum class PpuMode
    {
        LORES,
        HIRES
    };

    static constexpr uint8 PIXEL_ON = 1;
    static constexpr uint8 PIXEL_OFF = 0;

    static constexpr unsigned int SCREEN_LORES_MODE_WIDTH = 64;
    static constexpr unsigned int SCREEN_LORES_MODE_HEIGHT = 32;
    static constexpr unsigned int SCREEN_HIRES_MODE_WIDTH = 128;
    static constexpr unsigned int SCREEN_HIRES_MODE_HEIGHT = 64;

public:
    PpuBase();
    PpuBase(const PpuBase&) = delete;
    PpuBase(PpuBase&&) = delete;
    auto operator=(const PpuBase&) -> PpuBase& = delete;
    auto operator=(PpuBase&&) -> PpuBase& = delete;
    virtual ~PpuBase() = default;

#if defined(BUILD_PARAM_SAFE)
    void setErrorCallback(const std::function<void(const std::string&)>& errorCallback);
#endif

public:
    virtual void clearScreen() = 0;
    virtual auto drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8 = 0;

    virtual void setMode(PpuMode mode) { m_mode = mode; }
    [[nodiscard]] auto getMode() const -> PpuMode { return m_mode; }
    auto getLoresVideoMemory() -> std::array<uint8, SCREEN_LORES_MODE_WIDTH * SCREEN_LORES_MODE_HEIGHT>& { return m_loresVideoMemory; }
    auto getHiresVideoMemory() -> std::array<uint8, SCREEN_HIRES_MODE_WIDTH * SCREEN_HIRES_MODE_HEIGHT>& { return m_hiresVideoMemory; }

protected:
    PpuMode m_mode = PpuMode::LORES;
    std::array<uint8, SCREEN_LORES_MODE_WIDTH * SCREEN_LORES_MODE_HEIGHT> m_loresVideoMemory;
    std::array<uint8, SCREEN_HIRES_MODE_WIDTH * SCREEN_HIRES_MODE_HEIGHT> m_hiresVideoMemory;

#if defined(BUILD_PARAM_SAFE)
    std::function<void(const std::string&)> m_errorCallback;
#endif
};

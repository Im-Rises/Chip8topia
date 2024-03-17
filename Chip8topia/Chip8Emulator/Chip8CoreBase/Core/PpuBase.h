#pragma once

#include <array>
#include <functional>

#include "CpuBase.h"

class PpuBase
{
public:
    enum class PpuMode : uint8
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

    static constexpr unsigned int SCREEN_LORES_MODE_SIZE = SCREEN_LORES_MODE_WIDTH * SCREEN_LORES_MODE_HEIGHT;
    static constexpr unsigned int SCREEN_HIRES_MODE_SIZE = SCREEN_HIRES_MODE_WIDTH * SCREEN_HIRES_MODE_HEIGHT;

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
    virtual void reset();
    virtual void clearScreen() = 0;
    virtual auto drawSprite(uint8 Vx, uint8 Vy, uint8 n, const std::array<uint8, CpuBase::MEMORY_SIZE>& memory, uint16 I_reg) -> uint8 = 0;

    virtual void setMode(PpuMode mode) { m_mode = mode; }
    [[nodiscard]] auto getMode() const -> PpuMode { return m_mode; }
    auto getLoresVideoMemory() -> std::array<uint8, SCREEN_LORES_MODE_SIZE>& { return m_loresVideoMemory; }
    auto getHiresVideoMemory() -> std::array<uint8, SCREEN_HIRES_MODE_SIZE>& { return m_hiresVideoMemory; }
    auto getLoresVideoMemoryPlane() -> std::array<uint8, SCREEN_LORES_MODE_SIZE>& { return m_loresVideoMemoryPlane; }
    auto getHiresVideoMemoryPlane() -> std::array<uint8, SCREEN_HIRES_MODE_SIZE>& { return m_hiresVideoMemoryPlane; }

    void setPlane(uint8 x);
    [[nodiscard]] auto getPlane() const -> uint8;

protected: // TODO: For improvements, better make an array of plane instead of two separate arrays
    PpuMode m_mode = PpuMode::LORES;
    std::array<uint8, SCREEN_LORES_MODE_SIZE> m_loresVideoMemory;
    std::array<uint8, SCREEN_HIRES_MODE_SIZE> m_hiresVideoMemory;

    std::array<uint8, SCREEN_LORES_MODE_SIZE> m_loresVideoMemoryPlane;
    std::array<uint8, SCREEN_HIRES_MODE_SIZE> m_hiresVideoMemoryPlane;

    uint8 m_plane;

#if defined(BUILD_PARAM_SAFE)
    std::function<void(const std::string&)> m_errorCallback;
#endif
};

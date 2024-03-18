#include "PpuBase.h"

PpuBase::PpuBase() : m_loresVideoMemory{}, m_hiresVideoMemory{}, m_loresVideoMemoryPlane{}, m_hiresVideoMemoryPlane{}, m_plane(1), m_mode(PpuMode::LORES)
{
}

void PpuBase::reset()
{
    m_loresVideoMemory.fill(0);
    m_hiresVideoMemory.fill(0);
    m_loresVideoMemoryPlane.fill(0);
    m_hiresVideoMemoryPlane.fill(0);
    m_plane = 1;
    m_mode = PpuMode::LORES;
}

void PpuBase::scrollDown(uint8 n)
{
}

void PpuBase::scrollUp(uint8 n)
{
}

void PpuBase::scrollRight(uint8 n)
{
}

void PpuBase::scrollLeft(uint8 n)
{
}

void PpuBase::setMode(PpuMode mode)
{
    m_mode = mode;
}

[[nodiscard]] auto PpuBase::getMode() const -> PpuMode
{
    return m_mode;
}

auto PpuBase::getLoresVideoMemory() -> std::array<uint8, SCREEN_LORES_MODE_SIZE>&
{
    return m_loresVideoMemory;
}

auto PpuBase::getHiresVideoMemory() -> std::array<uint8, SCREEN_HIRES_MODE_SIZE>&
{
    return m_hiresVideoMemory;
}

auto PpuBase::getLoresVideoMemoryPlane() -> std::array<uint8, SCREEN_LORES_MODE_SIZE>&
{
    return m_loresVideoMemoryPlane;
}

auto PpuBase::getHiresVideoMemoryPlane() -> std::array<uint8, SCREEN_HIRES_MODE_SIZE>&
{
    return m_hiresVideoMemoryPlane;
}

void PpuBase::setPlane(uint8 x)
{
    m_plane = x;
}

#if defined(BUILD_PARAM_SAFE)
void PpuBase::setErrorCallback(const std::function<void(const std::string&)>& errorCallback)
{
    m_errorCallback = errorCallback;
}
#endif

#include "PpuBase.h"

PpuBase::PpuBase() : m_plane(1), m_mode(PpuMode::LORES), m_loresVideoMemoryPlanes{}, m_hiresVideoMemoryPlanes{}
{
}

void PpuBase::reset()
{
    m_plane = 1;
    m_mode = PpuMode::LORES;

    for (auto& plane : m_loresVideoMemoryPlanes)
    {
        plane.fill(PIXEL_OFF);
    }

    for (auto& plane : m_hiresVideoMemoryPlanes)
    {
        plane.fill(PIXEL_OFF);
    }
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

auto PpuBase::getLoresVideoMemory(uint8 plane) -> std::array<uint8, SCREEN_LORES_MODE_SIZE>&
{
    return m_loresVideoMemoryPlanes.at(plane);
}

auto PpuBase::getHiresVideoMemory(uint8 plane) -> std::array<uint8, SCREEN_HIRES_MODE_SIZE>&
{
    return m_hiresVideoMemoryPlanes.at(plane);
}

void PpuBase::setPlane(uint8 x)
{
    m_plane = x;
}

auto PpuBase::getPlane() const -> uint8
{
    return m_plane;
}

// auto PpuBase::getLoresVideoMemoryPlanes() const -> const std::array<std::array<uint8, SCREEN_LORES_MODE_SIZE>, PLANE_COUNT>&
//{
//     return m_loresVideoMemoryPlanes;
// }
//
// auto PpuBase::getHiresVideoMemoryPlanes() const -> const std::array<std::array<uint8, SCREEN_HIRES_MODE_SIZE>, PLANE_COUNT>&
//{
//     return m_hiresVideoMemoryPlanes;
// }

#if defined(BUILD_PARAM_SAFE)
void PpuBase::setErrorCallback(const std::function<void(const std::string&)>& errorCallback)
{
    m_errorCallback = errorCallback;
}
#endif

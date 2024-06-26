#include "PpuBase.h"

PpuBase::PpuBase() : m_planeMask(1), m_mode(PpuMode::LORES), m_loresVideoMemoryPlanes{}, m_hiresVideoMemoryPlanes{}
{
}

void PpuBase::reset()
{
    m_planeMask = 1;
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

void PpuBase::clearAllPlanes()
{
    if (getMode() == PpuMode::LORES)
    {
        for (unsigned int i = 0; i < PLANE_COUNT; i++)
        {
            m_loresVideoMemoryPlanes[i].fill(PIXEL_OFF);
        }
    }
    else
    {
        for (unsigned int i = 0; i < PLANE_COUNT; i++)
        {
            m_hiresVideoMemoryPlanes[i].fill(PIXEL_OFF);
        }
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
    m_planeMask = x;
}

auto PpuBase::getPlaneMask() -> uint8&
{
    return m_planeMask;
}

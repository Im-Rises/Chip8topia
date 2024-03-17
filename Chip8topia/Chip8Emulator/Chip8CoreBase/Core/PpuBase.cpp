#include "PpuBase.h"

PpuBase::PpuBase() : m_loresVideoMemory{}, m_hiresVideoMemory{}, m_loresVideoMemoryPlane{}, m_hiresVideoMemoryPlane{}, m_plane(1)
{
}

void PpuBase::reset()
{
    m_loresVideoMemory.fill(0);
    m_hiresVideoMemory.fill(0);
    m_loresVideoMemoryPlane.fill(0);
    m_hiresVideoMemoryPlane.fill(0);
    m_plane = 1;
}

void PpuBase::setPlane(uint8 x)
{
    m_plane = x;
}

auto PpuBase::getPlane() const -> uint8
{
    return m_plane;
}

#if defined(BUILD_PARAM_SAFE)
void PpuBase::setErrorCallback(const std::function<void(const std::string&)>& errorCallback)
{
    m_errorCallback = errorCallback;
}
#endif

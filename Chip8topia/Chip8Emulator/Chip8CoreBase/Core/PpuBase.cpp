#include "PpuBase.h"

PpuBase::PpuBase() : m_loresVideoMemory{}, m_hiresVideoMemory{}, m_loresVideoMemoryPlane{}, m_hiresVideoMemoryPlane{}, m_plane(1)
{
}

#if defined(BUILD_PARAM_SAFE)
void PpuBase::setErrorCallback(const std::function<void(const std::string&)>& errorCallback)
{
    m_errorCallback = errorCallback;
}
#endif

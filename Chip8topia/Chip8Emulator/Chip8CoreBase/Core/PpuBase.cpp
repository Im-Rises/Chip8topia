#include "PpuBase.h"

PpuBase::PpuBase() : m_loresVideoMemory{}, m_hiresVideoMemory{}
{
}

#if defined(BUILD_PARAM_SAFE)
void PpuBase::setErrorCallback(const std::function<void(const std::string&)>& errorCallback)
{
    m_errorCallback = errorCallback;
}
#endif

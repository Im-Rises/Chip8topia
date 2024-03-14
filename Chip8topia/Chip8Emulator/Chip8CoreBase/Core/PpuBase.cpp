#include "PpuBase.h"

PpuBase::PpuBase() : m_loresVideoMemory{}, m_hiresVideoMemory{} {
}

void PpuBase::setErrorCallback(const std::function<void(const std::string&)>& errorCallback) {
    m_errorCallback = errorCallback;
}

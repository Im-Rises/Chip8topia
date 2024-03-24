#include "PerformanceMonitor.h"

PerformanceMonitor::PerformanceMonitor() : m_memInfo(), m_pmc()
{
    m_memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    m_pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
}

PerformanceMonitor::~PerformanceMonitor()
{
    //    os_cpu_usage_info_destroy(m_cpuInfo);
}

void PerformanceMonitor::fetchRamUsage()
{
    GlobalMemoryStatusEx(&m_memInfo);
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&m_pmc, sizeof(m_pmc));
}

auto PerformanceMonitor::getTotalVirtualMemory() const -> DWORDLONG
{
    return m_memInfo.ullTotalPageFile;
}

auto PerformanceMonitor::getVirtualMemoryUsed() const -> DWORDLONG
{
    return m_memInfo.ullTotalPageFile - m_memInfo.ullAvailPageFile;
}

auto PerformanceMonitor::getVirtualMemoryUsedByMe() const -> SIZE_T
{
    return m_pmc.PrivateUsage;
}

auto PerformanceMonitor::getTotalPhysicalMemory() const -> DWORDLONG
{
    return m_memInfo.ullTotalPhys;
}

auto PerformanceMonitor::getPhysicalMemoryUsed() const -> DWORDLONG
{
    return m_memInfo.ullTotalPhys - m_memInfo.ullAvailPhys;
}

auto PerformanceMonitor::getPhysicalMemoryUsedByMe() const -> SIZE_T
{
    return m_pmc.WorkingSetSize;
}

void PerformanceMonitor::fetchCpuUsage()
{
}

auto PerformanceMonitor::getCpuUsage() -> float
{
    return 0.0f;
}

auto PerformanceMonitor::getCpuUsageByMe() -> float
{
    return 0.0f;
}

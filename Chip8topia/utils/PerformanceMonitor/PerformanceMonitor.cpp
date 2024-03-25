#include "PerformanceMonitor.h"

PerformanceMonitor::PerformanceMonitor() : m_memInfo(), m_pmc()
{
    // RAM (virtual memory) usage
    m_memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    // RAM (physical memory) usage
    m_pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);

    // CPU usage
    PdhOpenQuery(NULL, NULL, &m_cpuQuery);
    PdhAddEnglishCounter(m_cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time"), NULL, &m_cpuTotal);
    PdhCollectQueryData(m_cpuQuery);
    // CPU usage current process
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;
    GetSystemInfo(&sysInfo);
    m_numProcessors = sysInfo.dwNumberOfProcessors;
    GetSystemTimeAsFileTime(&ftime);
    memcpy(&m_lastCPU, &ftime, sizeof(FILETIME));
    m_self = GetCurrentProcess();
    GetProcessTimes(m_self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&m_lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&m_lastUserCPU, &fuser, sizeof(FILETIME));
}

PerformanceMonitor::~PerformanceMonitor()
{
}

void PerformanceMonitor::update()
{
    // RAM (virtual memory) usage
    GlobalMemoryStatusEx(&m_memInfo);
    // RAM (physical memory) usage
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&m_pmc, sizeof(m_pmc));
}

auto PerformanceMonitor::getTotalVirtualMemory() const -> DWORDLONG
{
    return m_memInfo.ullTotalPageFile / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getVirtualMemoryUsed() const -> DWORDLONG
{
    return (m_memInfo.ullTotalPageFile - m_memInfo.ullAvailPageFile) / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getVirtualMemoryUsedByCurrentProcess() const -> SIZE_T
{
    return m_pmc.PrivateUsage / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getTotalPhysicalMemory() const -> DWORDLONG
{
    return m_memInfo.ullTotalPhys / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getPhysicalMemoryUsed() const -> DWORDLONG
{
    return (m_memInfo.ullTotalPhys - m_memInfo.ullAvailPhys) / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getPhysicalMemoryUsedByCurrentProcess() const -> SIZE_T
{
    return m_pmc.WorkingSetSize / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getCpuUsed() -> float
{
    PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(m_cpuQuery);
    PdhGetFormattedCounterValue(m_cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

    return counterVal.doubleValue;
}

auto PerformanceMonitor::getCpuUsedByCurrentProcess() -> float
{
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(m_self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - m_lastSysCPU.QuadPart) +
              (user.QuadPart - m_lastUserCPU.QuadPart);
    percent /= (now.QuadPart - m_lastCPU.QuadPart);
    percent /= m_numProcessors;
    m_lastCPU = now;
    m_lastUserCPU = user;
    m_lastSysCPU = sys;

    return percent * 100;
}

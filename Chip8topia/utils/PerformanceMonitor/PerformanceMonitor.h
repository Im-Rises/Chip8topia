#pragma once

// https://learn.microsoft.com/en-us/windows/win32/psapi/collecting-memory-usage-information-for-a-process
// https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
// https://stackoverflow.com/questions/8501706/how-to-get-the-cpu-usage-in-c

#if defined(PLATFORM_WINDOWS)
#include <cstdio>
#include <windows.h>
#include <psapi.h>
#include <Pdh.h>
#include <TCHAR.h>
#elif defined(PLATFORM_LINUX)
#include <sys/sysinfo.h>
#endif

// TODO: Add error handling everywhere

class PerformanceMonitor
{
    static constexpr float RAM_MB_FACTOR = 1024.0F * 1024.0F;

public:
    PerformanceMonitor();
    PerformanceMonitor(const PerformanceMonitor&) = delete;
    auto operator=(const PerformanceMonitor&) -> PerformanceMonitor& = delete;
    PerformanceMonitor(PerformanceMonitor&&) = delete;
    auto operator=(PerformanceMonitor&&) -> PerformanceMonitor& = delete;
    ~PerformanceMonitor();

public:
    void update();

    [[nodiscard]] auto getTotalVirtualMemory() const -> float;
    [[nodiscard]] auto getVirtualMemoryUsed() const -> float;
    [[nodiscard]] auto getVirtualMemoryUsedByCurrentProcess() const -> float;

    [[nodiscard]] auto getTotalPhysicalMemory() const -> float;
    [[nodiscard]] auto getPhysicalMemoryUsed() const -> float;
    [[nodiscard]] auto getPhysicalMemoryUsedByCurrentProcess() const -> float;

    [[nodiscard]] auto getCpuUsed() -> float;
    [[nodiscard]] auto getCpuUsedByCurrentProcess() -> float;

private:
#if defined(PLATFORM_WINDOWS)
    // RAM (virtual memory) usage
    MEMORYSTATUSEX m_memInfo;
    // RAM (physical memory) usage
    PROCESS_MEMORY_COUNTERS_EX m_pmc;

    // CPU usage
    PDH_HQUERY m_cpuQuery;
    PDH_HCOUNTER m_cpuTotal;
    // CPU usage current process
    ULARGE_INTEGER m_lastCPU, m_lastSysCPU, m_lastUserCPU;
    int m_numProcessors;
    HANDLE m_self;
#elif defined(PLATFORM_LINUX)
    struct sysinfo m_info;
#endif
};

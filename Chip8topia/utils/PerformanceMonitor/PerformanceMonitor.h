#pragma once

// https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
// https://learn.microsoft.com/en-us/windows/win32/psapi/collecting-memory-usage-information-for-a-process

#include <cstdio>
#include <windows.h>
#include <psapi.h>
#include <Pdh.h>
#include <TCHAR.h>

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

    [[nodiscard]] auto getTotalVirtualMemory() const -> DWORDLONG;
    [[nodiscard]] auto getVirtualMemoryUsed() const -> DWORDLONG;
    [[nodiscard]] auto getVirtualMemoryUsedByCurrentProcess() const -> SIZE_T;

    [[nodiscard]] auto getTotalPhysicalMemory() const -> DWORDLONG;
    [[nodiscard]] auto getPhysicalMemoryUsed() const -> DWORDLONG;
    [[nodiscard]] auto getPhysicalMemoryUsedByCurrentProcess() const -> SIZE_T;

    [[nodiscard]] auto getCpuUsed() -> float;
    [[nodiscard]] auto getCpuUsedByCurrentProcess() -> float;

private:
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

    // TODO: Add error handling everywhere
};

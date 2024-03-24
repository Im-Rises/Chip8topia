#pragma once

// https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
// https://learn.microsoft.com/en-us/windows/win32/psapi/collecting-memory-usage-information-for-a-process
// https://github.com/obsproject/obs-studio/blob/master/libobs/util/platform-windows.c

#include <cstdio>
#include <windows.h>
#include <psapi.h> //RAM
#include <Pdh.h>   //CPU
#include <TCHAR.h>

union time_data
{
    FILETIME ft;
    unsigned long long val;
};

struct os_cpu_usage_info
{
    union time_data last_time, last_sys_time, last_user_time;
    DWORD core_count;
};

typedef struct os_cpu_usage_info os_cpu_usage_info_t;

class PerformanceMonitor
{
public:
    PerformanceMonitor();
    PerformanceMonitor(const PerformanceMonitor&) = delete;
    auto operator=(const PerformanceMonitor&) -> PerformanceMonitor& = delete;
    PerformanceMonitor(PerformanceMonitor&&) = delete;
    auto operator=(PerformanceMonitor&&) -> PerformanceMonitor& = delete;
    ~PerformanceMonitor();

public:
    void fetchRamUsage();

    [[nodiscard]] auto getTotalVirtualMemory() const -> DWORDLONG;
    [[nodiscard]] auto getVirtualMemoryUsed() const -> DWORDLONG;
    [[nodiscard]] auto getVirtualMemoryUsedByMe() const -> SIZE_T;
    [[nodiscard]] auto getTotalPhysicalMemory() const -> DWORDLONG;
    [[nodiscard]] auto getPhysicalMemoryUsed() const -> DWORDLONG;
    [[nodiscard]] auto getPhysicalMemoryUsedByMe() const -> SIZE_T;

    void fetchCpuUsage();
    [[nodiscard]] auto getCpuUsage() -> float;
    [[nodiscard]] auto getCpuUsageByMe() -> float;

    /*
        os_cpu_usage_info_t* os_cpu_usage_info_start(void);
        void os_cpu_usage_info_destroy(os_cpu_usage_info_t* info);
    */
private:
    MEMORYSTATUSEX m_memInfo;
    PROCESS_MEMORY_COUNTERS_EX m_pmc;

    /*
        os_cpu_usage_info* m_cpuInfo;
        double m_prevCpuUsage = 0.0;
    */

    PDH_HQUERY m_cpuQuery;
    PDH_HCOUNTER m_cpuTotal;
    void initCpuUsage();
    double calculateCpuUsage();

    ULARGE_INTEGER m_lastCPU, m_lastSysCPU, m_lastUserCPU;
    int m_numProcessors;
    HANDLE m_self;
    void initCpuUsageCurrentProcess();
    double calculateCpuUsageCurrentProcess();
};

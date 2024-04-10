#pragma once

// https://learn.microsoft.com/en-us/windows/win32/psapi/collecting-memory-usage-information-for-a-process
// https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
// https://stackoverflow.com/questions/8501706/how-to-get-the-cpu-usage-in-c

/*
 *                                   | Windows | Linux
 * - Total Virtual Memory            | Yes     | No
 * - Virtual Memory Used             | Yes     | No
 * - Virtual Memory Used by process  | Yes     | No
 * - Total Physical Memory           | Yes     | Yes
 * - Physical Memory Used            | Yes     | Yes
 * - Physical Memory Used by process | Yes     | Yes
 * - CPU Usage                       | Yes     | Yes
 * - CPU Usage by process            | No      | No
 * */

// TODO: PerformanceMonitor
// - Add error handling everywhere

#if defined(PLATFORM_WINDOWS)
#include <cstdio>
#include <windows.h>
#include <psapi.h>
#include <Pdh.h>
#include <TCHAR.h>
#elif defined(PLATFORM_LINUX)
#include <unistd.h>
#include <ctime>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/resource.h>
#elif defined(PLATFORM_MACOS)
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/mach_error.h>
#include <mach/vm_map.h>
#endif

#include <chrono>


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

#if defined(PLATFORM_MACOS)
    auto CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks) -> float;
    auto GetCPULoad() -> float;
#endif
    [[nodiscard]] auto getCpuUsed() -> float;
    [[nodiscard]] auto getCpuUsedByCurrentProcess() -> float;

private:
#if defined(PLATFORM_WINDOWS)
    // RAM (virtual memory)
    MEMORYSTATUSEX m_memInfo;
    // RAM (physical memory)
    PROCESS_MEMORY_COUNTERS_EX m_pmc;

    // CPU usage
    PDH_HQUERY m_cpuQuery;
    PDH_HCOUNTER m_cpuTotal;
    // CPU usage current process
    ULARGE_INTEGER m_lastCPU, m_lastSysCPU, m_lastUserCPU;
    int m_numProcessors;
    HANDLE m_self;
#elif defined(PLATFORM_LINUX)
    // RAM (physical memory)
    struct sysinfo m_info;
    // CPU usage
    struct rusage m_usage;
    suseconds_t m_lastTimeUsec;
    // TIMER
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTime;
    long m_deltaTime;
#elif defined(PLATFORM_MACOS)
    //    // RAM (total physical memory)
    //    int m_mib[2];
    // RAM (used physical memory)
    vm_size_t m_page_size;
    //    mach_port_t m_mach_port;
    mach_msg_type_number_t m_mac_msg_type_number;
    vm_statistics64_data_t m_vm_stats;
    // CPU usage
    unsigned long long m_previousTotalTicks = 0;
    unsigned long long m_previousIdleTicks = 0;
#endif
};

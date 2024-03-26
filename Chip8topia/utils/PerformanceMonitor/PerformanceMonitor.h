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
 * - CPU Usage                       | No      | No
 * - CPU Usage by process            | No      | No
 * */

#ifdef _WIN32 // Windows platform
#include <windows.h>
#include <iostream>
#else // Linux platform
#include <unistd.h>
#include <ctime>
#include <iostream>
#endif

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

#ifdef _WIN32
    double getFileTimeAsDouble(const FILETIME& ft)
    {
        LARGE_INTEGER li;
        li.LowPart = ft.dwLowDateTime;
        li.HighPart = ft.dwHighDateTime;
        return li.QuadPart / static_cast<double>(10000000); // Convert 100-nanosecond intervals to seconds
    }

    double getCPUTime()
    {
        FILETIME createTime, exitTime, kernelTime, userTime;
        GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTime, &userTime);
        double kernelTimeInSeconds = getFileTimeAsDouble(kernelTime);
        double userTimeInSeconds = getFileTimeAsDouble(userTime);
        return kernelTimeInSeconds + userTimeInSeconds;
    }

    int getNumLogicalCores()
    {
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        return sysinfo.dwNumberOfProcessors;
    }
#else
    double getCPUTime()
    {
        clock_t t = clock();
        return ((double)t) / CLOCKS_PER_SEC;
    }

    int getNumLogicalCores()
    {
        return sysconf(_SC_NPROCESSORS_ONLN);
    }
#endif

    double startCPUTime = getCPUTime();
    time_t startTime = time(nullptr);

public:
    double getCpuTest()
    {
        double endCPUTime = getCPUTime();
        time_t endTime = time(nullptr);

        if (endTime - startTime < 1)
        {
            return -1.0F;
        }

        int numLogicalCores = getNumLogicalCores();
        double cpuUsage = (endCPUTime - startCPUTime) / numLogicalCores / difftime(endTime, startTime) * 100;
        startCPUTime = endCPUTime;
        startTime = endTime;

        std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;
        return cpuUsage;
    }
};

#include "PerformanceMonitor.h"

#include <algorithm>

#if defined(PLATFORM_WINDOWS)

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
    // CPU usage
    PdhCloseQuery(m_cpuQuery);
}

void PerformanceMonitor::update()
{
    // RAM (virtual memory) usage
    GlobalMemoryStatusEx(&m_memInfo);
    // RAM (physical memory) usage
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&m_pmc, sizeof(m_pmc));
}

auto PerformanceMonitor::getTotalVirtualMemory() const -> float
{
    return m_memInfo.ullTotalPageFile / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getVirtualMemoryUsed() const -> float
{
    return (m_memInfo.ullTotalPageFile - m_memInfo.ullAvailPageFile) / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getVirtualMemoryUsedByCurrentProcess() const -> float
{
    return m_pmc.PrivateUsage / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getTotalPhysicalMemory() const -> float
{
    return m_memInfo.ullTotalPhys / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getPhysicalMemoryUsed() const -> float
{
    return (m_memInfo.ullTotalPhys - m_memInfo.ullAvailPhys) / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getPhysicalMemoryUsedByCurrentProcess() const -> float
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

    return percent * 100.0F;
}

#elif defined(PLATFORM_LINUX)

PerformanceMonitor::PerformanceMonitor() : m_deltaTime(0.0F), m_lastTime(std::chrono::high_resolution_clock::now())
{
    // RAM
    sysinfo(&m_info);
    // CPU
    getrusage(RUSAGE_SELF, &m_usage);
    suseconds_t user = m_usage.ru_utime.tv_sec * 1000000 + m_usage.ru_utime.tv_usec;
    suseconds_t sys = m_usage.ru_stime.tv_sec * 1000000 + m_usage.ru_stime.tv_usec;
    m_lastTimeUsec = user + sys;
}

PerformanceMonitor::~PerformanceMonitor()
{
}

void PerformanceMonitor::update()
{
    // RAM
    sysinfo(&m_info);
    // CPU
    getrusage(RUSAGE_SELF, &m_usage);
    // Delta time
    m_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_lastTime).count();
    m_lastTime = std::chrono::high_resolution_clock::now();
}

auto PerformanceMonitor::getTotalVirtualMemory() const -> float
{
    return -1.0F;
}

auto PerformanceMonitor::getVirtualMemoryUsed() const -> float
{
    return -1.0F;
}

auto PerformanceMonitor::getVirtualMemoryUsedByCurrentProcess() const -> float
{
    return -1.0F;
}

auto PerformanceMonitor::getTotalPhysicalMemory() const -> float
{
    return m_info.totalram / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getPhysicalMemoryUsed() const -> float
{
    return (m_info.totalram - m_info.freeram) / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getPhysicalMemoryUsedByCurrentProcess() const -> float
{
    return -1.0F;
}

auto PerformanceMonitor::getCpuUsed() -> float
{
    suseconds_t user = m_usage.ru_utime.tv_sec * 1000000 + m_usage.ru_utime.tv_usec;
    suseconds_t sys = m_usage.ru_stime.tv_sec * 1000000 + m_usage.ru_stime.tv_usec;
    suseconds_t totalCpuTime = user + sys - m_lastTimeUsec;
    m_lastTimeUsec = user + sys;

    return std::clamp((static_cast<float>(totalCpuTime) / m_deltaTime) * 100.0F, 0.0F, 100.0F);
}

auto PerformanceMonitor::getCpuUsedByCurrentProcess() -> float
{
    return -1.0F;
}

#elif defined(PLATFORM_MACOS)

PerformanceMonitor::PerformanceMonitor()
{
    m_mib[0] = CTL_HW;
    m_mib[1] = HW_MEMSIZE;
}

PerformanceMonitor::~PerformanceMonitor()
{
}

void PerformanceMonitor::update()
{
}

auto PerformanceMonitor::getTotalVirtualMemory() const -> float
{
    return -1.0F;
}

auto PerformanceMonitor::getVirtualMemoryUsed() const -> float
{
    return -1.0F;
}

auto PerformanceMonitor::getVirtualMemoryUsedByCurrentProcess() const -> float
{
    return -1.0F;
}

auto PerformanceMonitor::getTotalPhysicalMemory() const -> float
{
    int64_t physical_memory;
    size_t length = sizeof(int64_t);
    int mib[2];
    sysctl(mib, 2, &physical_memory, &length, NULL, 0);

    return physical_memory / RAM_MB_FACTOR;
}

auto PerformanceMonitor::getPhysicalMemoryUsed() const -> float
{
    mach_port_t mach_port;
    mach_port = mach_host_self();
    size_t count = sizeof(m_vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &m_page_size) &&
        KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                            (host_info64_t)&vm_stats, &m_mac_msg_type_number))
    {
        //    long long free_memory = (int64_t)vm_stats.free_count * (int64_t)m_page_size;

        long long used_memory = ((int64_t)m_vm_stats.active_count +
                                    (int64_t)m_vm_stats.inactive_count +
                                    (int64_t)m_vm_stats.wire_count) *
                                (int64_t)m_page_size;

        return (used_memory / RAM_MB_FACTOR);
    }

    return -1.0F;
}

auto PerformanceMonitor::getPhysicalMemoryUsedByCurrentProcess() const -> float
{
    return -1.0F;
}

auto PerformanceMonitor::CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks) -> float
{
    unsigned long long totalTicksSinceLastTime = totalTicks - m_previousTotalTicks;
    unsigned long long idleTicksSinceLastTime = idleTicks - m_previousIdleTicks;
    float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);
    m_previousTotalTicks = totalTicks;
    m_previousIdleTicks = idleTicks;
    return ret;
}

auto PerformanceMonitor::GetCPULoad() -> float
{
    host_cpu_load_info_data_t cpuinfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count) == KERN_SUCCESS)
    {
        unsigned long long totalTicks = 0;
        for (int i = 0; i < CPU_STATE_MAX; i++)
            totalTicks += cpuinfo.cpu_ticks[i];
        return CalculateCPULoad(cpuinfo.cpu_ticks[CPU_STATE_IDLE], totalTicks);
    }
    else
        return -1.0f;
}

auto PerformanceMonitor::getCpuUsed() -> float
{
    return GetCPULoad() * 100.0F;
}

auto PerformanceMonitor::getCpuUsedByCurrentProcess() -> float
{
    return -1.0F;
}

#endif
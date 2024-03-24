#include "PerformanceMonitor.h"

PerformanceMonitor::PerformanceMonitor() : m_memInfo(), m_pmc()
{
    m_memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    // m_pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);

    //    m_cpuInfo = os_cpu_usage_info_start();

    initCpuUsage();
    initCpuUsageCurrentProcess();
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

/*
os_cpu_usage_info_t* PerformanceMonitor::os_cpu_usage_info_start(void)
{
    struct os_cpu_usage_info* info = new os_cpu_usage_info;
    memset(info, 0, sizeof(*info));

    SYSTEM_INFO si;
    FILETIME dummy;

    GetSystemInfo(&si);
    GetSystemTimeAsFileTime(&info->last_time.ft);
    GetProcessTimes(GetCurrentProcess(), &dummy, &dummy,
        &info->last_sys_time.ft, &info->last_user_time.ft);
    info->core_count = si.dwNumberOfProcessors;

    return info;
}
double os_cpu_usage_info_query(os_cpu_usage_info_t* info)
{
    union time_data cur_time, cur_sys_time, cur_user_time;
    FILETIME dummy;
    double percent;

    if (!info)
        return 0.0;

    GetSystemTimeAsFileTime(&cur_time.ft);
    GetProcessTimes(GetCurrentProcess(), &dummy, &dummy, &cur_sys_time.ft,
        &cur_user_time.ft);

    percent = (double)(cur_sys_time.val - info->last_sys_time.val +
                       (cur_user_time.val - info->last_user_time.val));
    percent /= (double)(cur_time.val - info->last_time.val);
    percent /= (double)info->core_count;

    info->last_time.val = cur_time.val;
    info->last_sys_time.val = cur_sys_time.val;
    info->last_user_time.val = cur_user_time.val;

    return percent * 100.0;
}

void PerformanceMonitor::os_cpu_usage_info_destroy(os_cpu_usage_info_t* info)
{
    if (info)
        delete info;
}

auto PerformanceMonitor::getCpuUsage() -> float
{
    const double cpuUsage = os_cpu_usage_info_query(m_cpuInfo);
    if (cpuUsage > 0.0)
    {
        m_prevCpuUsage = cpuUsage;
        return cpuUsage;
    }
    else
    {
        return m_prevCpuUsage;
    }
    //    return os_cpu_usage_info_query(m_cpuInfo);
}

auto PerformanceMonitor::getCpuUsageByMe() -> float
{
    return 0;
}
*/


auto PerformanceMonitor::getCpuUsage() -> float
{
    return calculateCpuUsage();
}

auto PerformanceMonitor::getCpuUsageByMe() -> float
{
    return calculateCpuUsageCurrentProcess();
}

void PerformanceMonitor::initCpuUsage()
{
    PdhOpenQuery(NULL, NULL, &m_cpuQuery);
    // You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
    PdhAddEnglishCounter(m_cpuQuery, reinterpret_cast<LPCSTR>(L"\\Processor(_Total)\\% Processor Time"), NULL, &m_cpuTotal);
    PdhCollectQueryData(m_cpuQuery);
}

double PerformanceMonitor::calculateCpuUsage()
{
    PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(m_cpuQuery);
    PdhGetFormattedCounterValue(m_cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;
}
void PerformanceMonitor::initCpuUsageCurrentProcess()
{
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

double PerformanceMonitor::calculateCpuUsageCurrentProcess()
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

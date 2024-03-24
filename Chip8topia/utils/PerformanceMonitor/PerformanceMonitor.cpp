#include "PerformanceMonitor.h"

PerformanceMonitor::PerformanceMonitor() : m_memInfo(), m_pmc()
{
    m_memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    // m_pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
}

PerformanceMonitor::~PerformanceMonitor()
{
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

auto PerformanceMonitor::getCpuUsage() const -> float
{
    return 0;
}

auto PerformanceMonitor::getCpuUsageByMe() const -> float
{
    return 0;
}

/*
#include "pdh.h"   //CPU
#include <TCHAR.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <inttypes.h>
#include <locale.h>
// struct os_cpu_usage_info;
// typedef struct os_cpu_usage_info os_cpu_usage_info_t;
// struct os_cpu_usage_info
//{
//    clock_t last_cpu_time, last_sys_time, last_user_time;
//    int core_count;
//};
//
// typedef struct os_cpu_usage_info os_cpu_usage_info_t;
//
// union time_data
//{
//    FILETIME ft;
//    unsigned long long val;
//};

//union time_data
//{
//    FILETIME ft;
//    unsigned long long val;
//};
//
//struct os_cpu_usage_info_t
//{
//    union time_data last_time, last_sys_time, last_user_time;
//    DWORD core_count;
//};
//
////// CPU Currently Used
//double os_cpu_usage_info_query(os_cpu_usage_info_t* info)
//{
//    union time_data cur_time, cur_sys_time, cur_user_time;
//    //    uint64_t cur_time, cur_sys_time, cur_user_time;
//    FILETIME dummy;
//    double percent;
//
//    if (!info)
//        return 0.0;
//
//    GetSystemTimeAsFileTime(&cur_time.ft);
//    GetProcessTimes(GetCurrentProcess(), &dummy, &dummy, &cur_sys_time.ft,
//        &cur_user_time.ft);
//
//    percent = (double)(cur_sys_time.val - info->last_sys_time.val +
//                       (cur_user_time.val - info->last_user_time.val));
//    percent /= (double)(cur_time.val - info->last_time.val);
//    percent /= (double)info->core_count;
//
//    info->last_time.val = cur_time.val;
//    info->last_sys_time.val = cur_sys_time.val;
//    info->last_user_time.val = cur_user_time.val;
//
//    return percent * 100.0;

 int main()
 {
 //    os_cpu_usage_info_t cpu_info;
//    cpu_info.core_count = 1;
//    os_cpu_usage_info_query(&cpu_info);
//    double cpuUsage = os_cpu_usage_info_query(&cpu_info);
//    printf("CPU Usage: %.2f%%\n", cpuUsage);
 }
//}
//
 * */

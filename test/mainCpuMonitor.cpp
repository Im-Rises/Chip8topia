#ifdef _WIN32 // Windows platform
#include <windows.h>
#include <iostream>
#else // Linux platform
#include <unistd.h>
#include <ctime>
#include <iostream>
#endif

using namespace std;

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

int main()
{
    double startCPUTime = getCPUTime();
    time_t startTime = time(nullptr);

    while (true)
    {
        // Do some CPU intensive work here
        for (int i = 0; i < 1000000000; ++i)
        {
            // Some dummy computation
            int x = i * i;
        }

        double endCPUTime = getCPUTime();
        time_t endTime = time(nullptr);

        int numLogicalCores = getNumLogicalCores();
        double cpuUsage = (endCPUTime - startCPUTime) / numLogicalCores / difftime(endTime, startTime) * 100;
        //        double cpuUsage = (endCPUTime - startCPUTime) / 1 / difftime(endTime, startTime) * 100;
        cout << "CPU Usage: " << cpuUsage << "%" << endl;
        startCPUTime = endCPUTime;
        startTime = endTime;
    }
    return 0;
}

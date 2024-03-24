#include "Chip8topiaPerformanceMonitor.h"

#include <imgui.h>

void Chip8topiaPerformanceMonitor::drawWindow()
{
    m_performanceMonitor.fetchRamUsage();

    ImGui::Begin("Performance Monitor");

    ImGui::Text("Total Virtual Memory: %llu", m_performanceMonitor.getTotalVirtualMemory());
    ImGui::Text("Virtual Memory Used: %llu", m_performanceMonitor.getVirtualMemoryUsed());
    ImGui::Text("Virtual Memory Used By Me: %zu", m_performanceMonitor.getVirtualMemoryUsedByMe());
    ImGui::Text("Total Physical Memory: %llu", m_performanceMonitor.getTotalPhysicalMemory());
    ImGui::Text("Physical Memory Used: %llu", m_performanceMonitor.getPhysicalMemoryUsed());
    ImGui::Text("Physical Memory Used By Me: %zu", m_performanceMonitor.getPhysicalMemoryUsedByMe());

    ImGui::Text("CPU Usage: %f", m_performanceMonitor.getCpuUsage());
    ImGui::Text("CPU Usage By Me: %f", m_performanceMonitor.getCpuUsageByMe());

    ImGui::End();
}

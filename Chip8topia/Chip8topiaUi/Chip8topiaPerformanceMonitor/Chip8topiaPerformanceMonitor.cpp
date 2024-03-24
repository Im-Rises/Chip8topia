#include "Chip8topiaPerformanceMonitor.h"

#include <imgui.h>

void Chip8topiaPerformanceMonitor::drawWindow()
{
    m_performanceMonitor.fetchRamUsage();

    //    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::Begin("Performance Monitosr", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus);

    //    ImGui::Text("Total Virtual Memory: %llu", m_performanceMonitor.getTotalVirtualMemory());
    //    ImGui::Text("Virtual Memory Used: %llu", m_performanceMonitor.getVirtualMemoryUsed());
    //    ImGui::Text("Virtual Memory Used By Me: %zu", m_performanceMonitor.getVirtualMemoryUsedByMe());
    ImGui::Text("Total Physical Memory: %llu", m_performanceMonitor.getTotalPhysicalMemory());
    //    ImGui::Text("Physical Memory Used: %llu", m_performanceMonitor.getPhysicalMemoryUsed());
    ImGui::Text("Physical Memory Used By Me: %zu", m_performanceMonitor.getPhysicalMemoryUsedByMe());

    //    ImGui::Text("CPU Usage: %f", m_performanceMonitor.getCpuUsage());
    ImGui::Text("CPU Usage By Me: %f", m_performanceMonitor.getCpuUsageByMe());

    ImGui::End();
}

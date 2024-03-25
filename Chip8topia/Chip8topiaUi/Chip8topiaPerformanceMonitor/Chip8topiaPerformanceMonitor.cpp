#include "Chip8topiaPerformanceMonitor.h"

#include <imgui.h>

void Chip8topiaPerformanceMonitor::drawWindow()
{
    m_performanceMonitor.update();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0F, 0.0F, 1.0F, 1.0F));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    //    ImGui::SetNextWindowSize(ImVec2(200, 200));

    ImGui::Begin("Performance Monitor", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNavInputs);
    //    ImGui::Begin("Performance Monitor", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus);

#if defined(BUILD_DEBUG)
    ImGui::Text("DEBUG");
#else
    ImGui::Text("RELEASE");
#endif
    ImGui::SameLine();
#if defined(__EMSCRIPTEN__)
    ImGui::Text("WASM (EMSCRIPTEN)");
#elif defined(PLATFORM_WINDOWS)
    ImGui::Text("WINDOWS");
#elif defined(PLATFORM_LINUX)
    ImGui::Text("LINUX");
#elif defined(PLATFORM_APPLE)
    ImGui::Text("APPLE");
#endif

    ImGui::Text("Total Virtual Memory: %llu", m_performanceMonitor.getTotalVirtualMemory());
    ImGui::Text("Virtual Memory Used: %llu", m_performanceMonitor.getVirtualMemoryUsed());
    ImGui::Text("Virtual Memory Used by process: %zu", m_performanceMonitor.getVirtualMemoryUsedByCurrentProcess());

    ImGui::Text("Total Physical Memory: %llu", m_performanceMonitor.getTotalPhysicalMemory());
    ImGui::Text("Physical Memory Used: %llu", m_performanceMonitor.getPhysicalMemoryUsed());
    ImGui::Text("Physical Memory Used by process: %zu", m_performanceMonitor.getPhysicalMemoryUsedByCurrentProcess());

    ImGui::Text("CPU Usage: %f", m_performanceMonitor.getCpuUsed());
    ImGui::Text("CPU Usage by process: %f", m_performanceMonitor.getCpuUsedByCurrentProcess());

    ImGui::End();
    ImGui::PopStyleColor();
}

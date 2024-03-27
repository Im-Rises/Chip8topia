#include "Chip8topiaPerformanceMonitor.h"

#include "../Chip8topia.h"

#include <imgui.h>

void Chip8topiaPerformanceMonitor::drawWindow(Chip8topia& chip8topia, bool isMainBarOpen)
{
    m_performanceMonitor.update();

    ImGui::SetNextWindowBgAlpha(0.35F);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0F, 0.0F, 1.0F, 1.0F));
    ImGui::Begin("Performance Monitor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);

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
    ImGui::SameLine();
    ImGui::Text("VERSION %s", CHIP8TOPIA_VERSION);

    //  TODO: Add color to the text (red when bad, yellow when warning, green when good)
    //     ImGui::TextColored(ImGui::GetIO().Framerate > 60.0F ? ImVec4(0.0F, 1.0F, 0.0F, 1.0F) : ImVec4(1.0F, 0.0F, 0.0F, 1.0F), "FPS: %0.f", ImGui::GetIO().Framerate);
    ImGui::Text("Resolution: %dx%d", chip8topia.getCurrentWidth(), chip8topia.getCurrentHeight());

    ImGui::Text("FPS: %0.f", ImGui::GetIO().Framerate);
    ImGui::SameLine();
    ImGui::Text("DeltaTime: %0.3f", ImGui::GetIO().DeltaTime);

    ImGui::Text("RAM: %0.f", m_performanceMonitor.getTotalPhysicalMemory());
    ImGui::SameLine();
    ImGui::Text("RAM Used: %0.f", m_performanceMonitor.getPhysicalMemoryUsed());

    ImGui::Text("CPU Usage: %06.2f", m_performanceMonitor.getCpuUsed());
    ImGui::SameLine();

    ImGui::End();
    ImGui::PopStyleColor();
}

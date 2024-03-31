#include "Chip8topiaPerformanceMonitor.h"

#include <imgui.h>

#include "../Chip8topia.h"

static constexpr ImVec4 GREEN_COLOR = ImVec4(0.0F, 1.0F, 0.0F, 1.0F);
static constexpr ImVec4 YELLOW_COLOR = ImVec4(1.0F, 1.0F, 0.0F, 1.0F);
static constexpr ImVec4 RED_COLOR = ImVec4(1.0F, 0.0F, 0.0F, 1.0F);
static constexpr ImVec4 BLUE_COLOR = ImVec4(0.0F, 1.0F, 1.0F, 1.0F);

inline auto calculateRamColor(float ramPercentUsed) -> ImVec4
{
    return ramPercentUsed > 80.0F ? RED_COLOR : ramPercentUsed > 60.0F ? YELLOW_COLOR
                                                                       : GREEN_COLOR;
}

inline auto calculateFpsColor(float fps) -> ImVec4
{
    return fps < 55.0F ? RED_COLOR : fps < 57.5F ? YELLOW_COLOR
                                                 : GREEN_COLOR;
}

inline auto calculateCpuColor(float cpu) -> ImVec4
{
    return cpu > 80.0F ? RED_COLOR : cpu > 60.0F ? YELLOW_COLOR
                                                 : GREEN_COLOR;
}

void Chip8topiaPerformanceMonitor::drawWindow(Chip8topia& chip8topia, bool isMainBarOpen)
{
#if !defined(__EMSCRIPTEN__)
    m_performanceMonitor.update();
#endif

    const ImVec2 mainWindowPos = ImGui::GetMainViewport()->Pos;
    ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + 10.0F, mainWindowPos.y + (isMainBarOpen ? 30.0F : 10.0F)), ImGuiCond_Always);
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

    ImGui::Text("Resolution:");
    ImGui::SameLine();
    ImGui::TextColored(BLUE_COLOR, "%dx%d", chip8topia.getCurrentWidth(), chip8topia.getCurrentHeight());

#if !defined(__EMSCRIPTEN__)
    ImGui::Text("RAM:");
    ImGui::SameLine();
    ImGui::TextColored(BLUE_COLOR, "%0.f", m_performanceMonitor.getTotalPhysicalMemory());
    ImGui::SameLine();
    ImGui::Text("RAM Used:");
    ImGui::SameLine();
    const float ramUsed = m_performanceMonitor.getPhysicalMemoryUsed();
    const ImVec4 colorRam = calculateRamColor((ramUsed / m_performanceMonitor.getTotalPhysicalMemory() * 100));
    ImGui::TextColored(colorRam, "%0.f", ramUsed);

    // TODO: Maybe replace with real delta time (this one is an average)
    const float cpuUsage = m_performanceMonitor.getCpuUsed();
    const ImVec4 colorCpu = calculateCpuColor(cpuUsage);
    ImGui::Text("CPU Usage:");
    ImGui::SameLine();
    ImGui::TextColored(colorCpu, "%06.2f", cpuUsage);
#endif

    const ImVec4 colorFps = calculateFpsColor(ImGui::GetIO().Framerate);
    ImGui::Text("DeltaTime:");
    ImGui::SameLine();
    ImGui::TextColored(colorFps, "%0.4f", ImGui::GetIO().DeltaTime);
    ImGui::Text("FPS:");
    ImGui::SameLine();
    ImGui::TextColored(colorFps, "%0.f", ImGui::GetIO().Framerate);

    ImGui::End();
    ImGui::PopStyleColor();
}

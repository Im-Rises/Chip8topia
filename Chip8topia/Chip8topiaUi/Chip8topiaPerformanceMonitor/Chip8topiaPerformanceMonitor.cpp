#include "Chip8topiaPerformanceMonitor.h"

#include "../Chip8topia.h"

#include <imgui.h>

void Chip8topiaPerformanceMonitor::drawWindow(Chip8topia& chip8topia, bool isMainBarOpen)
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

    // TODO: Add color to the text (red when bad, yellow when warning, green when good), use a lerp function

    ImGui::Text("CPU Usage: %f", m_performanceMonitor.getCpuUsed());
    ImGui::SameLine();
    ImGui::Text("RAM: %0.f", m_performanceMonitor.getTotalPhysicalMemory());
    ImGui::SameLine();
    ImGui::Text("RAM Used: %0.f", m_performanceMonitor.getPhysicalMemoryUsed());

    ImGui::Text("FPS: %0.f", ImGui::GetIO().Framerate);
    ImGui::SameLine();
    ImGui::Text("DeltaTime: %0.3f", ImGui::GetIO().DeltaTime);

    ImGui::End();
    ImGui::PopStyleColor();

    //    // Get glfw window position
    //    // TODO: Change y position if Main Menu is open or not
    //    //  Set position to the top left corner of the glfw window
    //    std::pair<int, int> windowPosition = chip8topia.getWindowPosition();
    //    if (isMainBarOpen)
    //    {
    //        ImGui::SetNextWindowPos(ImVec2(windowPosition.first + 20, windowPosition.second + 20 + 20));
    //    }
    //    else
    //    {
    //        ImGui::SetNextWindowPos(ImVec2(windowPosition.first + 20, windowPosition.second + 20));
    //    }
    ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
    //    if (isMainBarOpen)
    //    {
    //        ImGui::SetNextWindowPos(ImVec2(20, 20 + 20));
    //    }
    //    else
    //    {
    //        ImGui::SetNextWindowPos(ImVec2(20, 20));
    //    }
    ImGui::SetNextWindowPos(ImVec2(200, 200));
    ImGui::SetNextWindowBgAlpha(0.35F);
    ImGui::Begin("tefdsfdfdsq", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("tefdsfdfdsq");
    ImGui::End();
}

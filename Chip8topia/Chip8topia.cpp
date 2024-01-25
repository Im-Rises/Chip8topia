#include "Chip8topia.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <cstdio>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <chrono>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "imgui/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

Chip8topia::Chip8topia() {
    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() == 0)
        exit(1);

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only
#endif

    // Create window with graphics context
    m_window = glfwCreateWindow(m_currentWidth, m_currentHeight, PROJECT_NAME, nullptr, nullptr);
    if (m_window == nullptr)
        exit(1);
    glfwMakeContextCurrent(m_window);
    //    glfwSwapInterval(1); // Enable vsync
    //    glfwSwapInterval(0); // Disable vsync
    glfwSwapInterval(VSYNC_ENABLED);

#ifdef __EMSCRIPTEN__
    // Initialize OpenGL loader
    if (gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
        exit(1);
#else
    // Initialize OpenGL loader
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
        exit(1);
#endif

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0F;
        style.Colors[ImGuiCol_WindowBg].w = 1.0F;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    m_chip8topiaUi.init(this);

    m_chip8topiaInputHandler.m_EscapeKeyButtonPressedEvent.subscribe(this, &Chip8topia::close);
    m_chip8topiaInputHandler.m_F11KeyButtonPressedEvent.subscribe(this, &Chip8topia::toggleFullScreen);
}

Chip8topia::~Chip8topia() {
    m_chip8topiaInputHandler.m_EscapeKeyButtonPressedEvent.unsubscribe(this, &Chip8topia::close);
    m_chip8topiaInputHandler.m_F11KeyButtonPressedEvent.unsubscribe(this, &Chip8topia::toggleFullScreen);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

auto Chip8topia::getOpenGLVendor() -> std::string_view {
    return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

auto Chip8topia::getOpenGLVersion() -> std::string_view {
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

// auto Chip8topia::getGLSLVersion() -> std::string_view {
//     return reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
// }

auto Chip8topia::getGLFWVersion() -> std::string {
    return std::to_string(GLFW_VERSION_MAJOR) + "." + std::to_string(GLFW_VERSION_MINOR) + "." +
           std::to_string(GLFW_VERSION_REVISION);
}

auto Chip8topia::getGladVersion() -> std::string_view {
    return "0.1.36";
}

auto Chip8topia::getImGuiVersion() -> std::string {
    return IMGUI_VERSION;
}

auto Chip8topia::run() -> int {
#ifdef _WIN32
    timeBeginPeriod(1);
#endif

#ifdef __EMSCRIPTEN__
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else

    auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = 0.0F;

    while (glfwWindowShouldClose(m_window) == 0)
#endif
    {
        currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        lastTime = currentTime;

        handleInputs();
        handleUi(deltaTime);
        handleGameUpdate(deltaTime);
        handleScreenUpdate();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

#ifdef _WIN32
    timeEndPeriod(1);
#endif

    return 0;
}

void Chip8topia::close() {
    glfwSetWindowShouldClose(m_window, 1);
}

void Chip8topia::handleInputs() {
    glfwPollEvents();

    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        m_chip8topiaInputHandler.m_EscapeKeyButtonPressedEvent.trigger();

    if (glfwGetKey(m_window, GLFW_KEY_F11) == GLFW_PRESS)
        m_chip8topiaInputHandler.m_F11KeyButtonPressedEvent.trigger();

    if (glfwGetKey(m_window, GLFW_KEY_F12) == GLFW_PRESS)
        m_chip8topiaInputHandler.m_F12KeyButtonPressedEvent.trigger();
}

void Chip8topia::handleUi(const float deltaTime) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    m_chip8topiaUi.drawUi();

    ImGui::Render();
}

void Chip8topia::handleGameUpdate(const float deltaTime) {
    //    if (m_chip8Emulator.GetIsRomLoaded())
    //    {
    //        m_chip8Emulator.update(deltaTime);
    //    }
}

void Chip8topia::handleScreenUpdate() {
    // todo: move this code ?
    const ImGuiIO& io = ImGui::GetIO();
    static constexpr ImVec4 clear_color = ImVec4(0.45F, 0.55F, 0.60F, 1.00F);

    if (!m_isFullScreen)
    {
        glfwGetWindowPos(m_window, &m_windowedPosX, &m_windowedPosY);
        glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);
    }

    glfwGetFramebufferSize(m_window, &m_currentWidth, &m_currentHeight);
    glViewport(0, 0, m_currentWidth, m_currentHeight);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(m_window);
}

void Chip8topia::toggleFullScreen() {
    if (!m_isFullScreen)
    {
        //        glfwGetWindowPos(m_window, &m_windowedPosX, &m_windowedPosY);
        //        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        //        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        //        glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

        int count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        for (int i = 0; i < count; i++)
        {
            int x, y;
            int width, height;
            glfwGetMonitorWorkarea(monitors[i], &x, &y, &width, &height);

            if (m_windowedPosX >= x && m_windowedPosX <= x + width && m_windowedPosY >= y && m_windowedPosY <= y + height)
            {
                //                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                //                glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                glfwSetWindowMonitor(m_window, monitors[i], x, y, width, height, VSYNC_ENABLED);
                break;
            }
        }
    }
    else
    {
        glfwSetWindowMonitor(m_window, nullptr, m_windowedPosX, m_windowedPosY, m_windowedWidth, m_windowedHeight, 0);
    }

    m_isFullScreen = !m_isFullScreen;
}

void Chip8topia::getWindowedDimensions(int& width, int& height) const {
    width = m_currentWidth;
    height = m_currentHeight;
}

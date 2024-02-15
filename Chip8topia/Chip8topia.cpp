#include "Chip8topia.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <cstdio>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// #include <format>
#include <chrono>

#include "res/chip8topiaIconResource.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "imgui/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void drop_callback(GLFWwindow* window, int count, const char** paths) {
    // TODO: Handle crash when loading file with invalid extension
    (void)count;
    static constexpr int INDEX = 0;
    const char* path = paths[INDEX];
    auto* engine = reinterpret_cast<Chip8topia*>(glfwGetWindowUserPointer(window));
    engine->getChip8Emulator().loadRom(path);
    //    engine->getChip8Emulator().getChip8Core()->getInput()->updateKey(0x0, 1);
}

Chip8topia::Chip8topia() {
    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() == 0)
        exit(1);

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
// #elif defined(__APPLE__)
//     // GL 3.2 + GLSL 150
//     const char* glsl_version = "#version 150";
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only
#endif

    // Create window with graphics context
    m_window = glfwCreateWindow(m_currentWidth, m_currentHeight, PROJECT_NAME, nullptr, nullptr);
    if (m_window == nullptr)
        exit(1);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(m_isTurboMode ? 0 : 1); // 0 = no vsync, 1 = vsync

    // Set window callbacks
    glfwSetWindowUserPointer(m_window, this);
    glfwSetDropCallback(m_window, drop_callback);
    glfwSetKeyCallback(m_window, Chip8topiaInputHandler::key_callback);

    // Center window
    centerWindow(); // TODO: Check if doesn't cause problems on Emscripten

// Initialize OpenGL loader
#ifndef __EMSCRIPTEN__
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
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("WEB_CANVAS_ID");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

#ifndef __EMSCRIPTEN__
    setWindowIcon();
#endif

    m_chip8topiaInputHandler.m_EscapeKeyButtonPressedEvent.subscribe(this, &Chip8topia::close);
    m_chip8topiaInputHandler.m_F3KeyButtonPressedEvent.subscribe(this, &Chip8topia::toggleTurboMode);
    m_chip8topiaInputHandler.m_F10KeyButtonPressedEvent.subscribe(this, &Chip8topia::centerWindow);
    m_chip8topiaInputHandler.m_F11KeyButtonPressedEvent.subscribe(this, &Chip8topia::toggleFullScreen);
#ifdef _DEBUG
    m_chip8topiaInputHandler.m_F12KeyDebugButtonPressedEvent.subscribe(this, &Chip8topia::loadDebugRom);
#endif
}

Chip8topia::~Chip8topia() {
    m_chip8topiaInputHandler.m_EscapeKeyButtonPressedEvent.unsubscribe(this, &Chip8topia::close);
    m_chip8topiaInputHandler.m_F3KeyButtonPressedEvent.unsubscribe(this, &Chip8topia::toggleTurboMode);
    m_chip8topiaInputHandler.m_F10KeyButtonPressedEvent.unsubscribe(this, &Chip8topia::centerWindow);
    m_chip8topiaInputHandler.m_F11KeyButtonPressedEvent.unsubscribe(this, &Chip8topia::toggleFullScreen);
#ifdef _DEBUG
    m_chip8topiaInputHandler.m_F12KeyDebugButtonPressedEvent.unsubscribe(this, &Chip8topia::loadDebugRom);
#endif

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

auto Chip8topia::run() -> int {
    m_chip8Emulator = std::make_unique<Chip8Emulator>();
#ifdef _WIN32
    timeBeginPeriod(1);
#endif

#ifdef __EMSCRIPTEN__
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else

    auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = lastTime;
    float deltaTime = 0.0F;

    float frameCounter = 0.0F;
    float elapsedTimeAccumulator = 0.0F;

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

        frameCounter++;
        elapsedTimeAccumulator += deltaTime;
        if (elapsedTimeAccumulator >= 1.0F)
        {
#ifndef __EMSCRIPTEN__
            setWindowTitle(frameCounter / elapsedTimeAccumulator);
#endif
            frameCounter = 0;
            elapsedTimeAccumulator = 0.0F;
        }
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
    m_chip8topiaInputHandler.update(m_window);
}

void Chip8topia::handleUi(const float deltaTime) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    m_chip8topiaUi.drawUi(*this);
    ImGui::Render();
}

void Chip8topia::handleGameUpdate(const float deltaTime) {
    m_chip8Emulator->update(deltaTime);
}

void Chip8topia::handleScreenUpdate() {
    const ImGuiIO& io = ImGui::GetIO();
    static constexpr ImVec4 CLEAR_COLOR = ImVec4(0.45F, 0.55F, 0.60F, 1.00F); // TODO: Declare as a member ? (even through it doesn't change anything because currently not seen by the user)

    if (!m_isFullScreen)
    {
        glfwGetWindowPos(m_window, &m_windowedPosX, &m_windowedPosY);
        glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);
    }

    glfwGetFramebufferSize(m_window, &m_currentWidth, &m_currentHeight);
    glViewport(0, 0, m_currentWidth, m_currentHeight);
    glClearColor(CLEAR_COLOR.x * CLEAR_COLOR.w, CLEAR_COLOR.y * CLEAR_COLOR.w, CLEAR_COLOR.z * CLEAR_COLOR.w, CLEAR_COLOR.w);
    glClear(GL_COLOR_BUFFER_BIT);

    m_chip8Emulator->render();

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

void Chip8topia::centerWindow() {
    // TODO: Mix code from toggleFullScreen and centerWindow to avoid code duplication for the window area location calculation
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    for (int i = 0; i < count; i++)
    {
        int x, y;
        int width, height;
        glfwGetMonitorWorkarea(monitors[i], &x, &y, &width, &height);

        if (m_windowedPosX >= x && m_windowedPosX <= x + width && m_windowedPosY >= y && m_windowedPosY <= y + height)
        {
            glfwSetWindowPos(m_window, x + (width - m_windowedWidth) / 2, y + (height - m_windowedHeight) / 2);
            break;
        }
    }
}

void Chip8topia::toggleFullScreen() {
    if (!m_isFullScreen)
    {
        int count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        for (int i = 0; i < count; i++)
        {
            int x, y;
            int width, height;
            glfwGetMonitorWorkarea(monitors[i], &x, &y, &width, &height);

            if (m_windowedPosX >= x && m_windowedPosX <= x + width && m_windowedPosY >= y && m_windowedPosY <= y + height)
            {
                const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
                glfwSetWindowMonitor(m_window, monitors[i], 0, 0, mode->width, mode->height, mode->refreshRate);
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

void Chip8topia::toggleTurboMode() {
    glfwSwapInterval(m_isTurboMode ? 1 : 0); // 0 = no vsync, 1 = vsync
    m_isTurboMode = !m_isTurboMode;
    m_chip8Emulator->setIsTurboMode(m_isTurboMode);
}

#ifndef __EMSCRIPTEN__
void Chip8topia::setWindowIcon() {
    int chip8topiaIconWidth = 0, chip8topiaIconHeight = 0, channelsInFile = 0;
    unsigned char* imagePixels = stbi_load_from_memory(CHIP8TOPIA_ICON_DATA.data(), static_cast<int>(CHIP8TOPIA_ICON_DATA.size()), &chip8topiaIconWidth, &chip8topiaIconHeight, &channelsInFile, 0);
    GLFWimage images;
    images.width = chip8topiaIconWidth;
    images.height = chip8topiaIconHeight;
    images.pixels = imagePixels;
    glfwSetWindowIcon(m_window, 1, &images);
    stbi_image_free(imagePixels);
}

void Chip8topia::setWindowTitle(const float fps) {
    //    m_chip8Emulator.getRomName();// TODO: Add rom name to window title
    glfwSetWindowTitle(m_window, std::format("{} - {:.2f} fps", PROJECT_NAME, fps).c_str());
}
#endif

auto Chip8topia::getChip8Emulator() -> Chip8Emulator& {
    return *m_chip8Emulator;
}

auto Chip8topia::getIsTurboMode() const -> bool {
    return m_isTurboMode;
}

auto Chip8topia::getOpenGLVendor() -> std::string_view {
    return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

auto Chip8topia::getOpenGLVersion() -> std::string_view {
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

auto Chip8topia::getGLSLVersion() -> std::string_view {
    return reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
}

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

#ifdef _DEBUG
void Chip8topia::loadDebugRom() {
    m_chip8Emulator->loadRom("trash/5-quirks.ch8");
}
#endif

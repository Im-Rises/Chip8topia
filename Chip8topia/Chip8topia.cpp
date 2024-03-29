#include "Chip8topia.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <imgui_emscripten/imgui_emscripten.h>
#else
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif
#include <GLFW/glfw3.h>

#include <fmt/format.h>
#include <chrono>
#include <iostream>
#include <fstream>

#include <ImGuiNotify.hpp>
#include <IconsFontAwesome6.h>

#include "Chip8Emulator/Chip8Emulator/Chip8RomLoader.h"

Chip8topia::Chip8topia() : m_window(nullptr)
{
}

Chip8topia::~Chip8topia() = default;

auto Chip8topia::run() -> int
{
    auto initErrorCode = init();
    if (initErrorCode != 0)
    {
        return initErrorCode;
    }

    auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = lastTime;
    float deltaTime = 0.0F;

    float frameCounter = 0.0F;
    float elapsedTimeAccumulator = 0.0F;

    m_chip8Emulator = std::make_unique<Chip8Emulator>();
#ifdef _WIN32
    timeBeginPeriod(1);
#endif

#ifdef __EMSCRIPTEN__
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
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

    cleanup();

    return SUCCESS_CODE;
}

#ifndef __EMSCRIPTEN__
void Chip8topia::closeRequest()
{
    glfwSetWindowShouldClose(m_window, 1);
}
#endif

auto Chip8topia::init() -> int
{
    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() == 0)
    {
        return GLFW_INIT_ERROR_CODE;
    }

#if defined(__EMSCRIPTEN__)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only
#endif

    // Get canvas size
#if defined(__EMSCRIPTEN__)
    emscripten_get_canvas_element_size(WEB_CANVAS_ID, &m_currentWidth, &m_currentHeight);
#endif

    // Create window with graphics context
    m_window = glfwCreateWindow(m_currentWidth, m_currentHeight, PROJECT_NAME, nullptr, nullptr);
    if (m_window == nullptr)
    {
        return WINDOW_INIT_ERROR_CODE;
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(m_isTurboMode ? 0 : 1); // 0 = no vsync, 1 = vsync

    // Set window callbacks
    glfwSetWindowUserPointer(m_window, this);
    glfwSetDropCallback(m_window, glfw_drop_callback);
    glfwSetKeyCallback(m_window, Chip8topiaInputHandler::key_callback);

    // Center window
    centerWindow();

    // Initialize OpenGL loader
#ifndef __EMSCRIPTEN__
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
    {
        spdlog::error("Failed to initialize OpenGL loader");
        return GLAD_INIT_ERROR_CODE;
    }
#endif

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoTaskBarIcon = true;            // Disable TaskBar icon for secondary viewports
    // io.ConfigViewportsNoAutoMerge = true;              // Enable Multi-Viewport auto-merge
    // io.ConfigViewportsNoDefaultParent = false;         // Set to true to create a default main viewport, or false to create a custom viewport

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
    {
        style.WindowRounding = 0.0F;
        style.Colors[ImGuiCol_WindowBg].w = 1.0F;
    }

    // Setup ImGuiNotify and IconFontCppHeaders
    io.Fonts->AddFontDefault();

    float baseFontSize = 16.0F;                      // Default font size
    float iconFontSize = baseFontSize * 2.0F / 3.0F; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    // Check if FONT_ICON_FILE_NAME_FAS is a valid path
    std::ifstream fontAwesomeFile(FONT_ICON_FILE_NAME_FAS);

    if (!fontAwesomeFile.good())
    {
#if !defined(__EMSCRIPTEN__)
        spdlog::error("Could not find font awesome file: {}", FONT_ICON_FILE_NAME_FAS);
#else
        std::cerr << "Could not find font awesome file: " << FONT_ICON_FILE_NAME_FAS << '\n';
#endif
        return FONT_AWESOME_INIT_ERROR_CODE;
    }

    static const ImWchar iconsRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize, &iconsConfig, iconsRanges);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Set window icon
#ifndef __EMSCRIPTEN__
    setWindowIcon();
#endif

    // Set input callbacks
#ifndef __EMSCRIPTEN__
    m_chip8topiaInputHandler.m_ExitChip8topiaEvent.subscribe(this, &Chip8topia::closeRequest);
    m_chip8topiaInputHandler.m_ToggleTurboModeEvent.subscribe(this, &Chip8topia::toggleTurboMode);
    m_chip8topiaInputHandler.m_CenterWindowEvent.subscribe(this, &Chip8topia::centerWindow);
    m_chip8topiaInputHandler.m_ToggleFullScreenEvent.subscribe(this, &Chip8topia::toggleFullScreen);
#endif

#if !defined(BUILD_RELEASE)
    m_chip8topiaInputHandler.m_DebugRomFastLoadEvent.subscribe(this, &Chip8topia::loadDebugRom);
#endif

    printDependenciesInfos();

    std::cout << "Logs:" << '\n';
#if !defined(__EMSCRIPTEN__)
    spdlog::info("Chip8topia initialized successfully");
#else
    std::cout << "Chip8topia initialized successfully" << '\n';
#endif

    return SUCCESS_CODE;
}

void Chip8topia::cleanup()
{
#ifndef __EMSCRIPTEN__
    m_chip8topiaInputHandler.m_ExitChip8topiaEvent.unsubscribe(this, &Chip8topia::closeRequest);
    m_chip8topiaInputHandler.m_ToggleTurboModeEvent.unsubscribe(this, &Chip8topia::toggleTurboMode);
    m_chip8topiaInputHandler.m_CenterWindowEvent.unsubscribe(this, &Chip8topia::centerWindow);
    m_chip8topiaInputHandler.m_ToggleFullScreenEvent.unsubscribe(this, &Chip8topia::toggleFullScreen);
#endif

#if !defined(BUILD_RELEASE)
    m_chip8topiaInputHandler.m_DebugRomFastLoadEvent.unsubscribe(this, &Chip8topia::loadDebugRom);
#endif

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();

#if !defined(__EMSCRIPTEN__)
    spdlog::info("Chip8topia cleaned up successfully");
#else
    std::cout << "Chip8topia cleaned up successfully" << '\n';
#endif
}

void Chip8topia::handleInputs()
{
    m_chip8topiaInputHandler.update(m_window);
}

void Chip8topia::handleUi(const float /*deltaTime*/)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    m_chip8topiaUi.drawUi(*this);
    ImGui::RenderNotifications();
    ImGui::Render();
}

void Chip8topia::handleGameUpdate(const float deltaTime)
{
    m_chip8Emulator->update(deltaTime);
}

void Chip8topia::handleScreenUpdate()
{
    const ImGuiIO& io = ImGui::GetIO();

    if (!m_isFullScreen)
    {
        glfwGetWindowPos(m_window, &m_windowedPosX, &m_windowedPosY);
        glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);
    }

#if defined(__EMSCRIPTEN__)
    emscripten_get_canvas_element_size(WEB_CANVAS_ID, &m_currentWidth, &m_currentHeight);
#else
    glfwGetFramebufferSize(m_window, &m_currentWidth, &m_currentHeight);
#endif
    glViewport(0, 0, m_currentWidth, m_currentHeight);
    //    glClearColor(CLEAR_COLOR.x * CLEAR_COLOR.w, CLEAR_COLOR.y * CLEAR_COLOR.w, CLEAR_COLOR.z * CLEAR_COLOR.w, CLEAR_COLOR.w);
    ImVec4& clearColor = m_chip8Emulator->getChip8VideoEmulation().getColor(0);
    glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    m_chip8Emulator->render(static_cast<float>(m_currentWidth), static_cast<float>(m_currentHeight));

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(m_window);
}

void Chip8topia::centerWindow()
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
            glfwSetWindowPos(m_window, x + (width - m_windowedWidth) / 2, y + (height - m_windowedHeight) / 2);
            break;
        }
    }
}

void Chip8topia::toggleFullScreen()
{
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

void Chip8topia::toggleTurboMode()
{
    glfwSwapInterval(m_isTurboMode ? 1 : 0); // 0 = no vsync, 1 = vsync
    m_isTurboMode = !m_isTurboMode;
    m_chip8Emulator->setIsTurboMode(m_isTurboMode);
}

#ifndef __EMSCRIPTEN__
void Chip8topia::setWindowIcon()
{
    int width = 0, height = 0, channelsCount = 0;
    unsigned char* imagePixels = stbi_load(CHIP8TOPIA_ICON_PATH, &width, &height, &channelsCount, 0);
    if (imagePixels == nullptr)
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, 3000, "Failed to load window icon" });
        return;
    }

    GLFWimage images;
    images.width = width;
    images.height = height;
    images.pixels = imagePixels;
    glfwSetWindowIcon(m_window, 1, &images);
}

void Chip8topia::setWindowTitle(const float fps)
{
    glfwSetWindowTitle(m_window, fmt::format("{} - {} - {} - {:.2f} fps", PROJECT_NAME, m_chip8Emulator->getConsoleName().c_str(), m_chip8Emulator->getRomName().c_str(), fps).c_str());
}
#endif

auto Chip8topia::getChip8Emulator() -> Chip8Emulator&
{
    return *m_chip8Emulator;
}

auto Chip8topia::getIsTurboMode() const -> bool
{
    return m_isTurboMode;
}

auto Chip8topia::getWindowDimensions() const -> std::pair<int, int>
{
    return { m_currentWidth, m_currentHeight };
}

auto Chip8topia::getWindowWidth() const -> int
{
    return m_currentWidth;
}
auto Chip8topia::getWindowHeight() const -> int
{
    return m_currentHeight;
}

// auto Chip8topia::getWindowMinimized() const -> bool {
//     return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0;
// }

auto Chip8topia::getOpenGLVendor() -> std::string_view
{
    return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

auto Chip8topia::getOpenGLVersion() -> std::string_view
{
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

auto Chip8topia::getGLSLVersion() -> std::string_view
{
    return reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
}

auto Chip8topia::getGLFWVersion() -> std::string
{
    return std::to_string(GLFW_VERSION_MAJOR) + "." + std::to_string(GLFW_VERSION_MINOR) + "." +
           std::to_string(GLFW_VERSION_REVISION);
}

auto Chip8topia::getGladVersion() -> std::string_view
{
    return "0.1.36";
}

auto Chip8topia::getImGuiVersion() -> std::string
{
    return IMGUI_VERSION;
}

void Chip8topia::printDependenciesInfos()
{
    std::cout << "System and dependencies infos:" << '\n'
              << " - OpenGL vendor " << Chip8topia::getOpenGLVendor() << '\n'
              << " - OpenGL version " << Chip8topia::getOpenGLVersion() << '\n'
              << " - OpenGL GLSL version " << Chip8topia::getGLSLVersion() << '\n'
              << " - GLFW version " << Chip8topia::getGLFWVersion() << '\n'
              << " - Glad version " << Chip8topia::getGladVersion() << '\n'
              << " - ImGui version " << Chip8topia::getImGuiVersion() << '\n'
              << '\n';
}

void Chip8topia::glfw_error_callback(int error, const char* description)
{
    Chip8topiaInputHandler::getInstance().m_ErrorEvent.trigger(fmt::format("Glfw Error {}: {}", error, description), []()
        { exit(1); });
}

void Chip8topia::glfw_drop_callback(GLFWwindow* window, int count, const char** paths)
{
    (void)count;
    static constexpr int INDEX = 0;
    const char* path = paths[INDEX];

    try
    {
        std::vector<uint8> rom = Chip8RomLoader::loadRomFromPath(path);
        auto* engine = reinterpret_cast<Chip8topia*>(glfwGetWindowUserPointer(window));
        engine->getChip8Emulator().loadRom(rom);
        engine->getChip8Emulator().setRomName(Chip8RomLoader::getRomNameFromPath(path));
        ImGui::InsertNotification({ ImGuiToastType::Success, 1000, "Rom loaded successfully" });
    }
    catch (const std::exception& e)
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, 3000, e.what() });
    }
}

#if !defined(BUILD_RELEASE)
void Chip8topia::loadDebugRom()
{
    try
    {
        std::vector<uint8> rom = Chip8RomLoader::loadRomFromPath(DEBUG_ROM_PATH);
        m_chip8Emulator->loadRom(rom);
        m_chip8Emulator->setRomName(Chip8RomLoader::getRomNameFromPath(DEBUG_ROM_PATH));
        ImGui::InsertNotification({ ImGuiToastType::Success, 1000, "Debug rom loaded successfully" });
    }
    catch (const std::exception& e)
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, 3000, e.what() });
    }
}
#endif

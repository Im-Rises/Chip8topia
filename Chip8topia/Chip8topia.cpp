#include "Chip8topia.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <imgui_emscripten/imgui_emscripten.h>
#else
#include <glad/glad.h>
#include <SDL_opengl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include <fmt/format.h>
#include <fstream>
#include <thread>

#include <consoleLogger/consoleLogger.h>
#include <iostream>
#include <ImGuiNotify.hpp>
#include <IconsFontAwesome6.h>

#include "Chip8Emulator/Chip8Emulator/Chip8RomLoader.h"

Chip8topia::Chip8topia() : m_window(nullptr), m_gl_context()
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
    m_deltaTime = 0.0F;

    m_chip8Emulator = std::make_unique<Chip8Emulator>();

#ifdef __EMSCRIPTEN__
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!m_closeRequested)
#endif
    {
        currentTime = std::chrono::high_resolution_clock::now();
        m_deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        lastTime = currentTime;

        handleInputs();
        handleUi();
        handleGameUpdate();
        handleScreenUpdate();

        auto delay = std::chrono::duration<float, std::chrono::milliseconds::period>(std::chrono::high_resolution_clock::now() - currentTime).count();

        if (!m_isTurboMode && delay < FRAME_TIME_MS)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(FRAME_TIME_MS - delay)));
        }
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    cleanup();

    return SUCCESS_CODE;
}

#ifndef __EMSCRIPTEN__
void Chip8topia::closeRequest()
{
    m_closeRequested = true;
}
#endif

auto Chip8topia::init() -> int
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        LOG_ERROR("Failed to initialize SDL");
        return SDL_INIT_ERROR_CODE;
    }

    // Decide GL+GLSL versions
#if defined(__EMSCRIPTEN__)
    const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(PLATFORM_MACOS)
    const char* glsl_version = "#version 330";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#else
    const char* glsl_version = "#version 330";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Get canvas size
#if defined(__EMSCRIPTEN__)
    emscripten_get_canvas_element_size(WEB_CANVAS_ID, &m_currentWidth, &m_currentHeight);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    auto window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_window = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_currentWidth, m_currentHeight, window_flags);
    if (m_window == nullptr)
    {
        LOG_ERROR("Failed to create window");
        return WINDOW_INIT_ERROR_CODE;
    }

    // Init window position variables
    SDL_GetWindowPosition(m_window, &m_windowedPosX, &m_windowedPosY);

    // Create OpenGL context
    m_gl_context = SDL_GL_CreateContext(m_window);
    if (SDL_GL_MakeCurrent(m_window, m_gl_context) != 0)
    {
        LOG_ERROR("Failed to create OpenGL context");
        return OPENGL_INIT_ERROR_CODE;
    }

    // Disable VSync
    setVsyncEnabled(false);

    // Initialize OpenGL loader
#ifndef __EMSCRIPTEN__
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == 0)
    {
        LOG_ERROR("Failed to initialize OpenGL loader");
        return GLAD_INIT_ERROR_CODE;
    }
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
        LOG_ERROR("Could not find font awesome file: " FONT_ICON_FILE_NAME_FAS);
        return FONT_AWESOME_INIT_ERROR_CODE;
    }

    static const ImWchar iconsRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize, &iconsConfig, iconsRanges);

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_gl_context);
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

#if !defined(BUILD_RELEASE) && !defined(__EMSCRIPTEN__)
    m_chip8topiaInputHandler.m_DebugRomFastLoadEvent.subscribe(this, &Chip8topia::loadDebugRom);
#endif

    std::cout << getDependenciesInfos() << '\n';
    LOG_INFO("Chip8topia initialized successfully");

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

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

    LOG_INFO("Chip8topia cleaned up successfully");
}

void Chip8topia::handleInputs()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            m_closeRequested = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(m_window))
            m_closeRequested = true;
        if (event.key.keysym.sym == SDLK_ESCAPE)
            m_closeRequested = true;

        if (event.type == SDL_DROPFILE && event.drop.file != nullptr)
        {
            std::string romPath(event.drop.file);
            SDL_free(event.drop.file);
            loadRomFromPath(romPath.c_str());
        }
    }

    //    m_chip8topiaInputHandler.update(m_window);
}

void Chip8topia::handleUi()
{
    auto start = std::chrono::high_resolution_clock::now();

    const ImGuiIO& io = ImGui::GetIO();
#ifndef __EMSCRIPTEN__
    updateWindowTitle(io.Framerate);
#endif

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    m_chip8topiaUi.drawUi(*this);
    ImGui::RenderNotifications();
    ImGui::Render();

    m_uiUpdateTime = std::chrono::duration<float, std::chrono::milliseconds::period>(std::chrono::high_resolution_clock::now() - start).count();
}

void Chip8topia::handleGameUpdate()
{
    auto start = std::chrono::high_resolution_clock::now();
    m_chip8Emulator->update(m_deltaTime);
    m_gameUpdateTime = std::chrono::duration<float, std::chrono::milliseconds::period>(std::chrono::high_resolution_clock::now() - start).count();
}

void Chip8topia::handleScreenUpdate()
{
    auto start = std::chrono::high_resolution_clock::now();

    const ImGuiIO& io = ImGui::GetIO();

    if (!m_isFullScreen)
    {
        SDL_GetWindowPosition(m_window, &m_windowedPosX, &m_windowedPosY);
        SDL_GetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);
    }

#if defined(__EMSCRIPTEN__)
    emscripten_get_canvas_element_size(WEB_CANVAS_ID, &m_currentWidth, &m_currentHeight);
#else
    SDL_GetWindowSize(m_window, &m_currentWidth, &m_currentHeight);
#endif
    glViewport(0, 0, m_currentWidth, m_currentHeight);
    //    glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    ImVec4& clearColor = m_chip8Emulator->getChip8VideoEmulation().getColor(0);
    glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    m_chip8Emulator->render(static_cast<float>(m_currentWidth), static_cast<float>(m_currentHeight));

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    SDL_GL_SwapWindow(m_window);

    m_screenUpdateTime = std::chrono::duration<float, std::chrono::milliseconds::period>(std::chrono::high_resolution_clock::now() - start).count();
}

void Chip8topia::centerWindow()
{
    const int displayIndex = SDL_GetWindowDisplayIndex(m_window);
    SDL_Rect displayBounds;
    SDL_GetDisplayBounds(displayIndex, &displayBounds);
    const int centerX = displayBounds.x + (displayBounds.w - m_currentWidth) / 2;
    const int centerY = displayBounds.y + (displayBounds.h - m_currentHeight) / 2;
    SDL_SetWindowPosition(m_window, centerX, centerY);
}

void Chip8topia::toggleFullScreen()
{
    if (!m_isFullScreen)
    {
        const int displayIndex = SDL_GetWindowDisplayIndex(m_window);
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(displayIndex, &displayMode);
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_SetWindowSize(m_window, displayMode.w, displayMode.h);
    }
    else
    {
        SDL_SetWindowFullscreen(m_window, 0);
        SDL_SetWindowSize(m_window, m_windowedWidth, m_windowedHeight);
        SDL_SetWindowPosition(m_window, m_windowedPosX, m_windowedPosY);
    }

    setVsyncEnabled(false);

    m_isFullScreen = !m_isFullScreen;
}

void Chip8topia::toggleTurboMode()
{
    m_isTurboMode = !m_isTurboMode;
}

void Chip8topia::setVsyncEnabled(const bool isVsyncEnabled)
{
    SDL_GL_SetSwapInterval(isVsyncEnabled ? 1 : 0);
}

#ifndef __EMSCRIPTEN__
void Chip8topia::setWindowIcon()
{
    int width = 0;
    int height = 0;
    int channelsCount = 0;
    unsigned char* imagePixels = stbi_load(CHIP8TOPIA_ICON_PATH, &width, &height, &channelsCount, 0);
    if (imagePixels == nullptr)
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, TOAST_DURATION_ERROR, "Failed to load window icon", fmt::format("Cannot load icon at {}", CHIP8TOPIA_ICON_PATH).c_str() });
        LOG_ERROR(fmt::format("Failed to load window icon at {}", CHIP8TOPIA_ICON_PATH));
        return;
    }

    SDL_Surface* icon = SDL_CreateRGBSurfaceWithFormatFrom(imagePixels, width, height, 32, width * 4, SDL_PIXELFORMAT_RGBA32);
    SDL_SetWindowIcon(m_window, icon);
    SDL_FreeSurface(icon);

    stbi_image_free(imagePixels);
}

void Chip8topia::updateWindowTitle(const float fps)
{
    SDL_SetWindowTitle(m_window, fmt::format("{} - {} - {} - {:.1f} fps", PROJECT_NAME, m_chip8Emulator->getConsoleName().c_str(), m_chip8Emulator->getRomName().c_str(), fps).c_str());
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

auto Chip8topia::getUiUpdateTime() const -> float
{
    return m_uiUpdateTime;
}

auto Chip8topia::getGameUpdateTime() const -> float
{
    return m_gameUpdateTime;
}

auto Chip8topia::getScreenUpdateTime() const -> float
{
    return m_screenUpdateTime;
}

auto Chip8topia::getDeltaTime() const -> float
{
    return m_deltaTime;
}

auto Chip8topia::getWindowPosition() const -> std::pair<int, int>
{
    return { m_windowedPosX, m_windowedPosY };
}

auto Chip8topia::getCurrentDimensions() const -> std::pair<int, int>
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

auto Chip8topia::getCurrentWidth() const -> int
{
    return m_currentWidth;
}

auto Chip8topia::getCurrentHeight() const -> int
{
    return m_currentHeight;
}

auto Chip8topia::getWindowMaximized() const -> bool
{
    return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_MAXIMIZED) != 0;
}

auto Chip8topia::getWindowMinimized() const -> bool
{
    return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_MINIMIZED) != 0;
}

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

auto Chip8topia::getSDLVersion() -> std::string
{
    return SDL_GetRevision();
}

auto Chip8topia::getGladVersion() -> std::string_view
{
    return "0.1.36";
}

auto Chip8topia::getImGuiVersion() -> std::string
{
    return IMGUI_VERSION;
}

#if !defined(__EMSCRIPTEN__)
auto Chip8topia::getStbImageVersion() -> std::string
{
    return std::to_string(STBI_VERSION);
}
#endif

auto Chip8topia::getFmtVersion() -> std::string
{
    return std::to_string(FMT_VERSION);
}

#if !defined(BUILD_RELEASE)
auto Chip8topia::getSpdlogVersion() -> std::string
{
    return std::to_string(SPDLOG_VERSION);
}
#endif

auto Chip8topia::getDependenciesInfos() -> std::string
{
    return fmt::format("System and dependencies infos:\n"
                       " - OpenGL vendor {}\n"
                       " - OpenGL version {}\n"
                       " - OpenGL GLSL version {}\n"
                       " - SDL version {}\n"
                       " - Glad version {}\n"
                       " - ImGui version {}\n"
#if !defined(__EMSCRIPTEN__)
                       " - stb_image version {}\n"
#endif
                       " - fmt version {}\n"
#if !defined(BUILD_RELEASE)
                       " - spdlog version {}\n"
#endif
        ,
        getOpenGLVendor(),
        getOpenGLVersion(),
        getGLSLVersion(),
        getSDLVersion(),
        getGladVersion(),
        getImGuiVersion(),
#if !defined(__EMSCRIPTEN__)
        getStbImageVersion(),
#endif
        getFmtVersion()
#if !defined(BUILD_RELEASE)
            ,
        getSpdlogVersion()
#endif
    );
}

void Chip8topia::loadRomFromPath(const std::string& filePath)
{
    try
    {
        std::vector<uint8> rom = Chip8RomLoader::loadRomFromPath(filePath);
        m_chip8Emulator->loadRom(rom);
        m_chip8Emulator->setRomName(Chip8RomLoader::getRomNameFromPath(filePath));
        ImGui::InsertNotification({ ImGuiToastType::Success, TOAST_DURATION_SUCCESS, "Rom loaded successfully" });
        LOG_INFO("Rom loaded successfully");
    }
    catch (const std::exception& e)
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, TOAST_DURATION_ERROR, e.what() });
        LOG_ERROR(e.what());
    }
}

#if !defined(BUILD_RELEASE) && !defined(__EMSCRIPTEN__)
void Chip8topia::loadDebugRom()
{
    loadRomFromPath(DEBUG_ROM_PATH);
}
#endif

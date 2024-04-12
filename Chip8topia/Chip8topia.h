#pragma once

#include <string_view>
#include <SDL.h>

#include "Chip8topiaUi/Chip8topiaUi.h"
#include "Chip8Emulator/Chip8Emulator.h"
#include "Chip8topiaInputHandler/Chip8topiaInputHandler.h"

class Chip8topia
{
public:
    static constexpr const char* const PROJECT_NAME = "Chip8topia";
    static constexpr const char* const PROJECT_VERSION = CHIP8TOPIA_VERSION;
    static constexpr const char* const PROJECT_LINK = "https://github.com/Im-Rises/Chip8topia";
    static constexpr const char* const PROJECT_AUTHOR = "Im-Rises (Quentin Morel)";
    static constexpr const char* const PROJECT_DESCRIPTION = "Chip8topia is a Chip8 emulator made with modern C++, ImGui and OpenGL."
                                                             "It is emulating the Chip8, SuperChip, SuperChipC and XO-Chip consoles.";
    static constexpr const char* const PROJECT_EMULATION_CONSOLE_NAME = "Chip8";
    static constexpr const char* const PROJECT_EMULATION_CONSOLE_DESCRIPTION = R"(
Chip8 is an interpreted programming language, developed by Joseph Weisbecker.
It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s.
It was made to allow video games to be more easily programmed for said computers.)";

    static constexpr const char* const WEB_CANVAS_ID = "#canvas";
    static constexpr const char* const CHIP8TOPIA_ICON_PATH = "res/icon.png";

    static constexpr float REFRESH_RATE = 60.0F;
    static constexpr float FRAME_TIME_MS = 1000.0F / REFRESH_RATE;

private:
    static constexpr int SUCCESS_CODE = 0;
    static constexpr int SDL_INIT_ERROR_CODE = 1;
    static constexpr int WINDOW_INIT_ERROR_CODE = 2;
    static constexpr int OPENGL_INIT_ERROR_CODE = 3;
    static constexpr int GLAD_INIT_ERROR_CODE = 4;
    static constexpr int FONT_AWESOME_INIT_ERROR_CODE = 5;
#if !defined(BUILD_RELEASE)
    //    static constexpr const char* const DEBUG_ROM_PATH = "trash/chip8-test-suite-main/bin/8-scrolling.ch8";
    //    static constexpr const char* const DEBUG_ROM_PATH = "trash/chip8-test-suite-main/bin/5-quirks.ch8";
    static constexpr const char* const DEBUG_ROM_PATH = "trash/games/t8nks.ch8";
//    static constexpr const char* const DEBUG_ROM_PATH = "trash/games/alien-inv8sion.ch8";
//    static constexpr const char* const DEBUG_ROM_PATH = "trash/games/color-scroll-test-xochip.ch8";
#endif
    static constexpr int TOAST_DURATION_SUCCESS = 1000;
    static constexpr int TOAST_DURATION_ERROR = 3000;

public:
    Chip8topia();
    Chip8topia(const Chip8topia&) = delete;
    auto operator=(const Chip8topia&) -> Chip8topia& = delete;
    Chip8topia(Chip8topia&&) = delete;
    auto operator=(Chip8topia&&) -> Chip8topia& = delete;
    ~Chip8topia();

public:
    auto run() -> int;
#ifndef __EMSCRIPTEN__
    void closeRequest();
#endif

private:
    auto init() -> int;
    void cleanup();

    void handleInputs();
    void handleUi();
    void handleGameUpdate();
    void handleScreenUpdate();

public:
    void loadRomFromPath(const std::string& filePath);

    void centerWindow();
    void toggleFullScreen();
    void toggleTurboMode();
    void setVsyncEnabled(const bool isVSyncEnabled);
#ifndef __EMSCRIPTEN__
    void setWindowIcon();
    void updateWindowTitle(const float fps);
#endif

    [[nodiscard]] auto getChip8Emulator() -> Chip8Emulator&;
    [[nodiscard]] auto getIsTurboMode() const -> bool;

    [[nodiscard]] auto getInputUpdateTime() const -> float;
    [[nodiscard]] auto getUiUpdateTime() const -> float;
    [[nodiscard]] auto getGameUpdateTime() const -> float;
    [[nodiscard]] auto getScreenUpdateTime() const -> float;
    [[nodiscard]] auto getDeltaTime() const -> float;

    [[nodiscard]] auto getWindowPosition() const -> std::pair<int, int>;
    [[nodiscard]] auto getCurrentDimensions() const -> std::pair<int, int>;
    [[nodiscard]] auto getWindowWidth() const -> int;
    [[nodiscard]] auto getWindowHeight() const -> int;
    [[nodiscard]] auto getCurrentWidth() const -> int;
    [[nodiscard]] auto getCurrentHeight() const -> int;
    [[nodiscard]] auto getWindowMaximized() const -> bool;
    [[nodiscard]] auto getWindowMinimized() const -> bool;

private:
    static auto getOpenGLVendor() -> std::string_view;
    static auto getOpenGLVersion() -> std::string_view;
    static auto getGLSLVersion() -> std::string_view;
    static auto getSDLVersion() -> std::string;
    static auto getGladVersion() -> std::string_view;
    static auto getImGuiVersion() -> std::string;
#if !defined(__EMSCRIPTEN__)
    static auto getStbImageVersion() -> std::string;
#endif
    static auto getFmtVersion() -> std::string;
#if !defined(BUILD_RELEASE)
    static auto getSpdlogVersion() -> std::string;
#endif
    static auto getDependenciesInfos() -> std::string;

#if !defined(BUILD_RELEASE) && !defined(__EMSCRIPTEN__)
    void loadDebugRom();
#endif

private:
    SDL_Window* m_window;
    SDL_GLContext m_gl_context;

    std::unique_ptr<Chip8Emulator> m_chip8Emulator;
    Chip8topiaUi m_chip8topiaUi;
    Chip8topiaInputHandler& m_chip8topiaInputHandler = Chip8topiaInputHandler::getInstance();

    bool m_closeRequested = false;
    bool m_isFullScreen = false;
    bool m_isTurboMode = false;

    int m_currentWidth = 1280;
    int m_currentHeight = 720;
    int m_windowedWidth = m_currentWidth;
    int m_windowedHeight = m_currentHeight;

    int m_windowedPosX = 0;
    int m_windowedPosY = 0;

    float m_inputUpdateTime = 0.0F;
    float m_uiUpdateTime = 0.0F;
    float m_gameUpdateTime = 0.0F;
    float m_screenUpdateTime = 0.0F;
    float m_deltaTime = 0.0F;
};

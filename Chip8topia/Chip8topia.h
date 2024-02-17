#pragma once

#include <string_view>
#include <plateformIdentifier/plateformIdentifier.h>

#include "Chip8topiaUi/Chip8topiaUi.h"
#include "Chip8Emulator/Chip8Emulator.h"
#include "Chip8topiaInputHandler/Chip8topiaInputHandler.h"

struct GLFWwindow;
class Chip8topia {
public:
    static constexpr auto PROJECT_NAME = "Chip8topia";
    static constexpr auto PROJECT_VERSION = "0.0.1";
    static constexpr auto PROJECT_LINK = "https://github.com/Im-Rises/Chip8topia";
    static constexpr auto PROJECT_AUTHOR = "Im-Rises (Quentin Morel)";
    static constexpr auto PROJECT_DESCRIPTION = "Chip8topia is a Chip8 emulator made with modern C++ and OpenGL."
                                                "It is currently emulating the Chip8 interpreter and the SuperChip8 interpreter. ";
    static constexpr auto PROJECT_EMULATION_CONSOLE_NAME = "Chip8";
    static constexpr auto PROJECT_EMULATION_CONSOLE_DESCRIPTION = R"(
Chip8 is an interpreted programming language, developed by Joseph Weisbecker.
It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s.
It was made to allow video games to be more easily programmed for said computers.)";

    static constexpr auto WEB_CANVAS_ID = "#webCanvas";

public:
    Chip8topia();
    Chip8topia(const Chip8topia&) = delete;
    auto operator=(const Chip8topia&) -> Chip8topia& = delete;
    Chip8topia(Chip8topia&&) = delete;
    auto operator=(Chip8topia&&) -> Chip8topia& = delete;
    ~Chip8topia();

public:
    auto run() -> int;
    void closeRequest();

private:
    auto init() -> int;
    void cleanup();

    void handleInputs();
    void handleUi(const float deltaTime);
    void handleGameUpdate(const float deltaTime);
    void handleScreenUpdate();

public:
    void centerWindow();

    void toggleFullScreen();
    void toggleTurboMode();

#ifndef __EMSCRIPTEN__
    void setWindowIcon();
    void setWindowTitle(const float fps);
#endif

    [[nodiscard]] auto getChip8Emulator() -> Chip8Emulator&;
    [[nodiscard]] auto getIsTurboMode() const -> bool;

private:
    static auto getOpenGLVendor() -> std::string_view;
    static auto getOpenGLVersion() -> std::string_view;
    static auto getGLSLVersion() -> std::string_view;
    static auto getGLFWVersion() -> std::string;
    static auto getGladVersion() -> std::string_view;
    static auto getImGuiVersion() -> std::string;
    void printDependenciesInfos();

#if !defined(BUILD_RELEASE)
    void loadDebugRom();
#endif

private:
    static constexpr ImVec4 CLEAR_COLOR = ImVec4(0.45F, 0.55F, 0.60F, 1.00F);

    GLFWwindow* m_window;
    std::unique_ptr<Chip8Emulator> m_chip8Emulator; // TODO: Change to be initialized once the game loop is started
    Chip8topiaUi m_chip8topiaUi;                    // TODO: Change to be initialized once the game loop is started
    Chip8topiaInputHandler& m_chip8topiaInputHandler = Chip8topiaInputHandler::getInstance();

    bool m_isFullScreen = false;
    bool m_isTurboMode = false;
    int m_currentWidth = 1280;
    int m_currentHeight = 720;
    int m_windowedWidth = m_currentWidth;
    int m_windowedHeight = m_currentHeight;
    int m_windowedPosX = 0;
    int m_windowedPosY = 0;
};

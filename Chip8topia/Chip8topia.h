#pragma once

#include <string_view>

#include "Chip8Core/Chip8Core.h"
#include "Chip8topiaUi/Chip8topiaUi.h"

struct GLFWwindow;

class Chip8topia {
public:
    static constexpr auto PROJECT_NAME = "Chip8topia";
    static constexpr auto PROJECT_VERSION = "0.0.1";
    static constexpr auto PROJECT_LINK = "https://github.com/Im-Rises/Chip8topia";
    static constexpr auto PROJECT_AUTHOR = "Im-Rises (Quentin Morel)";

public:
    Chip8topia();
    Chip8topia(const Chip8topia&) = delete;
    auto operator=(const Chip8topia&) -> Chip8topia& = delete;
    Chip8topia(Chip8topia&&) = delete;
    auto operator=(Chip8topia&&) -> Chip8topia& = delete;
    ~Chip8topia();

public:
    static auto getOpenGLVendor() -> std::string_view;
    static auto getOpenGLVersion() -> std::string_view;
    //    static auto getGLSLVersion() -> std::string_view;
    static auto getGLFWVersion() -> std::string;
    static auto getGladVersion() -> std::string_view;
    static auto getImGuiVersion() -> std::string;

public:
    auto run() -> int;

private:
    void handleInputs();
    void handleUi(float deltaTime);
    void handleGameUpdate(float deltaTime);
    void handleScreenUpdate();

private:
    void toggleFullscreen();
    void togglePause();
    void toggleStep();

private:
    GLFWwindow* m_window;
    Chip8Core m_chip8Core;
    Chip8topiaUi m_chip8topiaUi;
};

#pragma once

#include <string_view>

class Chip8topia {
public:
    static constexpr auto PROJECT_NAME = "Chip8topia";
    static constexpr auto PROJECT_VERSION = "0.0.1";
    static constexpr auto PROJECT_LINK = "";
    static constexpr auto PROJECT_AUTHOR = "Im-Rises (Quentin Morel)";
    
public:
    Chip8topia();
    Chip8topia(const Chip8topia&) = delete;
    auto operator=(const Chip8topia&) -> Chip8topia& = delete;
    Chip8topia(Chip8topia&&) = delete;
    auto operator=(Chip8topia&&) -> Chip8topia& = delete;
    ~Chip8topia();

public:
    int run();

private:
    void handleInputs();
    void handleUi(float deltaTime);
    void updateGame(float deltaTime);
    void updateScreen();

private:
    static auto getOpenGLVendor() -> std::string_view;
    static auto getOpenGLVersion() -> std::string_view;
    static auto getGLSLVersion() -> std::string_view;
    static auto getGLFWVersion() -> std::string;
    static auto getGladVersion() -> std::string_view;
    static auto getImGuiVersion() -> std::string;

};

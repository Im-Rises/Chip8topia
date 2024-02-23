#pragma once

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <imgui.h>
#include <array>

#include "../../Shader/Shader.h"
#include "../Chip8Core/Core/Ppu.h"

// TODO: Change this class to receive different width and height when initialising the shader

class Chip8VideoEmulation {
private:
#if defined(__EMSCRIPTEN__)
    static constexpr auto VERTEX_SHADER_PATH = "shaders/Chip8topia_es.vert";
    static constexpr auto FRAGMENT_SHADER_PATH = "shaders/Chip8topia_es.frag";
#else
    static constexpr auto VERTEX_SHADER_PATH = "shaders/Chip8topia.vert";
    static constexpr auto FRAGMENT_SHADER_PATH = "shaders/Chip8topia.frag";
#endif

    static constexpr std::array<GLfloat, 18> VERTICES = {
        // first triangle
        1.0F, 1.0F, 0.0F,  // top right
        1.0F, -1.0F, 0.0F, // bottom right
        -1.0F, 1.0F, 0.0F, // top left
        // second triangle
        1.0F, -1.0F, 0.0F,  // bottom right
        -1.0F, -1.0F, 0.0F, // bottom left
        -1.0F, 1.0F, 0.0F,  // top left
    };

public:
    Chip8VideoEmulation();
    Chip8VideoEmulation(const Chip8VideoEmulation&) = delete;
    Chip8VideoEmulation(Chip8VideoEmulation&&) = delete;
    auto operator=(const Chip8VideoEmulation&) -> Chip8VideoEmulation& = delete;
    auto operator=(Chip8VideoEmulation&&) -> Chip8VideoEmulation& = delete;
    ~Chip8VideoEmulation() = default;

public:
    void reset();

    void updateTexture(const std::array<uint8, Ppu::WIDTH * Ppu::HEIGHT>& videoMemory);
    void update();

    auto getBackgroundColorRef() -> ImVec4&;
    auto getForegroundColorRef() -> ImVec4&;

private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_texture;

    Shader m_shader;

    ImVec4 m_backgroundColor = { 0.3F, 0.3F, 0.3F, 1.0F };
    ImVec4 m_foregroundColor = { 0.8F, 0.8F, 0.8F, 1.0F };
};

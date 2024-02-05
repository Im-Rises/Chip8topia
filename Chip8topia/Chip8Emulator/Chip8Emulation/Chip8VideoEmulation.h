#pragma once

// TODO: Handle webgl and opengl screen drawing

#include <glad/glad.h>
#include <array>

#include "../../Shader/Shader.h"
#include "../Chip8Core/Core/Ppu.h"

class Chip8VideoEmulation {
private:
    static constexpr auto VERTEX_SHADER_PATH = "shaders/Chip8topia.vert";
    static constexpr auto FRAGMENT_SHADER_PATH = "shaders/Chip8topia.frag";

    static constexpr std::array<float, 18> VERTICES = {
        // first triangle
        1.0F, 1.0F, 0.0F,  // top right
        1.0F, -1.0F, 0.0F, // bottom right
        -1.0F, 1.0F, 0.0F, // top left
        // second triangle
        1.0F, -1.0F, 0.0F,  // bottom right
        -1.0F, -1.0F, 0.0F, // bottom left
        -1.0F, 1.0F, 0.0F,  // top left
    };

    struct Color {
        GLfloat m_r;
        GLfloat m_g;
        GLfloat m_b;
        GLfloat m_a;
    };

public:
    Chip8VideoEmulation();
    Chip8VideoEmulation(const Chip8VideoEmulation&) = delete;
    Chip8VideoEmulation(Chip8VideoEmulation&&) = delete;
    auto operator=(const Chip8VideoEmulation&) -> Chip8VideoEmulation& = delete;
    auto operator=(Chip8VideoEmulation&&) -> Chip8VideoEmulation& = delete;
    ~Chip8VideoEmulation() = default;

public:
    void createTexture(const std::array<uint8, Ppu::WIDTH * Ppu::HEIGHT>& videoMemory);
    void update();

    // TODO: Change to not use get as a setter
    auto getBackgroundColor() -> Color&;
    auto getForegroundColor() -> Color&;

private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_texture;

    Shader m_shader;

    //    Color m_backgroundColor = { 0.0F, 0.0F, 0.0F, 1.0F };
    //    Color m_backgroundColor = { 0.0F, 0.0F, 0.0F, 1.0F };

    Color m_backgroundColor = { 0.3F, 0.3F, 0.3F, 1.0F };
    Color m_foregroundColor = { 0.8F, 0.8F, 0.8F, 1.0F };

    //    Color m_backgroundColor = { 0.0F, 0.5F, 0.5F, 1.0F };
    //    Color m_foregroundColor = { 1.0F, 0.84F, 0.35F, 1.0F };

    //    Color m_backgroundColor = { 0.45F, 0.55F, 0.60F, 1.00F };
    //    Color m_foregroundColor = { 0.8, 0.8, 0.8, 1.0F };
};

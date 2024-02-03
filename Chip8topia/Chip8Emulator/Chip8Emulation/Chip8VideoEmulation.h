#pragma once

// TODO: Handle webgl and opengl screen drawing

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
        float r;
        float g;
        float b;
        float a;
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
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_texture;

    Shader m_shader;

    Color m_backgroundColor = { 0.0F, 0.0F, 0.0F, 1.0F };
    Color m_foregroundColor = { 1.0F, 1.0F, 1.0F, 1.0F };
};

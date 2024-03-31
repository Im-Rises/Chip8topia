#pragma once

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <array>
#include <binaryLib/binaryLib.h>

#include "../../ChipCores/XoChipCore/Core/XoChipPpu.h"
#include "../../../Shader/Shader.h"

struct ImVec4;
class ShaderXoChip
{
private:
#if defined(__EMSCRIPTEN__)
    static constexpr const char* const VERTEX_SHADER_PATH = "shaders/Chip8topiaXoChip_es.vert";
    static constexpr const char* const FRAGMENT_SHADER_PATH = "shaders/Chip8topiaXoChip_es.frag";
#else
    static constexpr const char* const VERTEX_SHADER_PATH = "shaders/Chip8topiaXoChip.vert";
    static constexpr const char* const FRAGMENT_SHADER_PATH = "shaders/Chip8topiaXoChip.frag";
#endif

    static constexpr std::array<GLfloat, 12> VERTICES = {
        // first triangle
        1.0F, 1.0F,  // top right
        1.0F, -1.0F, // bottom right
        -1.0F, 1.0F, // top left
        // second triangle
        1.0F, -1.0F,  // bottom right
        -1.0F, -1.0F, // bottom left
        -1.0F, 1.0F,  // top left
    };

public:
    ShaderXoChip(int width, int height);
    ShaderXoChip(const ShaderXoChip&) = delete;
    ShaderXoChip(ShaderXoChip&&) = delete;
    auto operator=(const ShaderXoChip&) -> ShaderXoChip& = delete;
    auto operator=(ShaderXoChip&&) -> ShaderXoChip& = delete;
    ~ShaderXoChip();

public:
    void reset();

    void updateTexture(const uint8* videoMemory, const uint8 planeIndex);
    void update(const std::array<ImVec4, XoChipPpu::COLOR_COUNT>& colors, const float xScale, const float yScale);

    auto getTexture(const int index) -> GLuint;

private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_textures[XoChipPpu::PLANE_COUNT];
    Shader m_shader;

    const int WIDTH;
    const int HEIGHT;
};

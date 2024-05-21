#include "ShaderXoChip.h"

#include <imgui.h>

ShaderXoChip::ShaderXoChip(int width, int height) : WIDTH(width), HEIGHT(height),
                                                    m_VAO(0), m_VBO(0), m_textures{ 0, 0 },
                                                    m_shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, VERTICES.size() * sizeof(float), VERTICES.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(XoChipPpu::PLANE_COUNT, m_textures);

    for (unsigned int m_texture : m_textures)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, WIDTH, HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

ShaderXoChip::~ShaderXoChip()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteTextures(XoChipPpu::PLANE_COUNT, m_textures);
}

void ShaderXoChip::reset()
{
    // This function should be called only on reset, not on every frame
    // There is surely a better way to do this
    std::vector<uint8> data(WIDTH * HEIGHT, 0);
    for (GLuint m_texture : m_textures)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RED, GL_UNSIGNED_BYTE, data.data());
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShaderXoChip::updateTexture(const uint8* videoMemory, const uint8 planeIndex)
{
    glBindTexture(GL_TEXTURE_2D, m_textures[planeIndex]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RED, GL_UNSIGNED_BYTE, videoMemory);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShaderXoChip::update(const std::array<ImVec4, XoChipPpu::COLOR_COUNT>& colors, const float xScale, const float yScale)
{
    m_shader.use();
    glBindVertexArray(m_VAO);
    for (int i = 0; i < XoChipPpu::PLANE_COUNT; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        m_shader.setInt("u_textures[" + std::to_string(i) + "]", i);
    }

    for (int i = 0; i < XoChipPpu::COLOR_COUNT; ++i)
    {
        m_shader.setVec4("u_colors[" + std::to_string(i) + "]", colors[i].x, colors[i].y, colors[i].z, colors[i].w);
    }

    m_shader.setVec2("u_scaleFactor", xScale, yScale);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto ShaderXoChip::getTexture(const int index) -> GLuint
{
    return m_textures[index];
}

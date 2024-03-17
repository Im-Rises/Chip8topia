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

    glGenTextures(2, m_textures);

    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, WIDTH, HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, WIDTH, HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
}

ShaderXoChip::~ShaderXoChip()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteTextures(2, m_textures);
}

void ShaderXoChip::updateTextures(const uint8* mainPlaneData, const uint8* subPlaneData)
{
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RED, GL_UNSIGNED_BYTE, mainPlaneData);

    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RED, GL_UNSIGNED_BYTE, subPlaneData);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShaderXoChip::update(const ImVec4& backgroundColor, const ImVec4& mainPlaneColor, const ImVec4& subPlaneColor, const ImVec4& commonPixelsColor, const float xScale, const float yScale)
{
    m_shader.use();
    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    m_shader.setInt("u_mainPlaneTexture", 0);
    m_shader.setInt("u_subPlaneTexture", 1);
    m_shader.setVec4("u_backgroundColor", backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
    m_shader.setVec4("u_mainPlaneColor", mainPlaneColor.x, mainPlaneColor.y, mainPlaneColor.z, mainPlaneColor.w);
    m_shader.setVec4("u_subPlaneColor", subPlaneColor.x, subPlaneColor.y, subPlaneColor.z, subPlaneColor.w);
    m_shader.setVec4("u_commonPixelsColor", commonPixelsColor.x, commonPixelsColor.y, commonPixelsColor.z, commonPixelsColor.w);
    m_shader.setVec2("u_scaleFactor", xScale, yScale);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShaderXoChip::reset()
{
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, WIDTH, HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, WIDTH, HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
}

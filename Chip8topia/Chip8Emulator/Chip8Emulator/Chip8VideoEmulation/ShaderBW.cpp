#include "ShaderBW.h"

ShaderBW::ShaderBW(int width, int height) : m_width(width), m_height(height),
                                            m_VAO(0), m_VBO(0), m_texture(0),
                                            m_shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) {
    // Setup main shader data
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, VERTICES.size() * sizeof(float), VERTICES.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Setup texture
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
}

ShaderBW::~ShaderBW() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteTextures(1, &m_texture);
}
#include <iostream>
void ShaderBW::updateTexture(const std::vector<uint8>& videoMemory) {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED, GL_UNSIGNED_BYTE, videoMemory.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShaderBW::update() {
    m_shader.use();
    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    m_shader.setVec4("u_backgroundColor", m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w);
    m_shader.setVec4("u_foregroundColor", m_foregroundColor.x, m_foregroundColor.y, m_foregroundColor.z, m_foregroundColor.w);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(VERTICES.size() / 3));
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void ShaderBW::reset() {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

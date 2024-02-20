#include "Chip8VideoEmulation.h"

static constexpr const char* const VERTEX_SHADER_SOURCE =
    R"(#version 300 es

            layout(location = 0) in vec3 a_texCoord;

            out vec2 v_texCoord;

            void main()
            {
            v_texCoord = vec2(a_texCoord.x * 0.5 + 0.5, 1.0 - (a_texCoord.y * 0.5 + 0.5));
            gl_Position = vec4(a_texCoord.x, a_texCoord.y, a_texCoord.z, 1.0);
            }
    )";

static constexpr const char* const FRAGMENT_SHADER_SOURCE =
    R"(#version 300 es

            precision mediump float;

            in vec2 v_texCoord;

            out vec4 o_fragColor;

            uniform sampler2D u_ourTexture;
            uniform vec4 u_backgroundColor;
            uniform vec4 u_foregroundColor;

            void main()
            {
                vec4 color = texture(u_ourTexture, v_texCoord);
                if (color.r > 0.0)
                {
                    o_fragColor = u_foregroundColor;
                }
                else
                {
                    o_fragColor = u_backgroundColor;
                }
            }
    )";

Chip8VideoEmulation::Chip8VideoEmulation() : m_VAO(0), m_VBO(0),
#if defined(__EMSCRIPTEN__)
                                             m_shader(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE)
#else
                                             m_shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH)
#endif
{
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, Ppu::WIDTH, Ppu::HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Chip8VideoEmulation::reset() {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, Ppu::WIDTH, Ppu::HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Chip8VideoEmulation::updateTexture(const std::array<uint8, Ppu::WIDTH * Ppu::HEIGHT>& videoMemory) {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Ppu::WIDTH, Ppu::HEIGHT, GL_RED, GL_UNSIGNED_BYTE, videoMemory.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Chip8VideoEmulation::update() {
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

auto Chip8VideoEmulation::getBackgroundColorRef() -> ImVec4& {
    return m_backgroundColor;
}

auto Chip8VideoEmulation::getForegroundColorRef() -> ImVec4& {
    return m_foregroundColor;
}

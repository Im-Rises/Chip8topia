#include "Chip8VideoEmulation.h"

#include <glad/glad.h>

Chip8VideoEmulation::Chip8VideoEmulation() : m_VAO(0), m_VBO(0), m_shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, VERTICES.size() * sizeof(float), VERTICES.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Chip8VideoEmulation::update() {
    m_shader.use();
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(VERTICES.size() / 3));
}

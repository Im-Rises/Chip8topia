#include "ShaderXoChip.h"

#include <imgui.h>

ShaderXoChip::ShaderXoChip(int width, int height) : m_width(width), m_height(height),
                                                    m_VAO(0), m_VBO(0), m_texture(0),
                                                    m_shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH)
{
}

ShaderXoChip::~ShaderXoChip()
{
}

void ShaderXoChip::updateTexture(const uint8* data)
{
}

void ShaderXoChip::update(const ImVec4& backgroundColor, const ImVec4& foregroundColor, const float xScale, const float yScale)
{
}

void ShaderXoChip::reset()
{
}

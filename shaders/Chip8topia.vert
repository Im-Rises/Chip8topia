#version 330 core

layout (location = 0) in vec2 a_textCoord;

out vec2 v_texCoord;

uniform vec2 u_scaleFactor;

void main()
{
    gl_Position = vec4(u_scaleFactor * a_textCoord, 0.0, 1.0);
    v_texCoord = 0.5 * a_textCoord + 0.5;
}

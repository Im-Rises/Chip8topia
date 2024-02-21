#version 300 es

layout(location = 0) in vec3 a_texCoord;

out vec2 v_texCoord;

void main()
{
    v_texCoord = vec2(a_texCoord.x * 0.5 + 0.5, 1.0 - (a_texCoord.y * 0.5 + 0.5));
    gl_Position = vec4(a_texCoord.x, a_texCoord.y, a_texCoord.z, 1.0);
}
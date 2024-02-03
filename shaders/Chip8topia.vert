#version 330 core

layout (location = 0) in vec3 a_texCoord;

out vec2 v_texCoord;

void main()
{
    //    v_texCoord = a_Pos.xy * 0.5 + 0.5;
    v_texCoord = vec2(a_texCoord.x * 0.5 + 0.5, 1.0 - (a_texCoord.y * 0.5 + 0.5));// flip y
    gl_Position = vec4(a_texCoord, 1.0);
}

#version 330 core

layout (location = 0) in vec3 a_texCoord;

out vec2 v_texCoord;

uniform float u_xScale;
uniform float u_yScale;

void main()
{
    // Apply scaling to the texture coordinates
    vec2 scaledTexCoord = vec2(a_texCoord.x * u_xScale, a_texCoord.y * u_yScale);

    // Convert to OpenGL texture coordinate space
    v_texCoord = vec2(scaledTexCoord.x * 0.5 + 0.5, 1.0 - (scaledTexCoord.y * 0.5 + 0.5));

    // Apply scaling to gl_Position
    gl_Position = vec4(a_texCoord.x * u_xScale, a_texCoord.y * u_yScale, a_texCoord.z, 1.0);
}

//#version 330 core
//
//layout (location = 0) in vec3 a_texCoord;
//
//out vec2 v_texCoord;
//
//uniform float u_xScale;
//uniform float u_yScale;
//
//void main()
//{
//    v_texCoord = vec2(a_texCoord.x * 0.5 + 0.5, 1.0 - (a_texCoord.y * 0.5 + 0.5));
//    gl_Position = vec4(a_texCoord.x, a_texCoord.y, a_texCoord.z, 1.0);
//}

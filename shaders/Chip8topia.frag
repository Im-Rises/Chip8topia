#version 300 es

precision mediump float;

in vec2 v_texCoord;

out vec4 o_fragColor;

uniform sampler2D u_ourTexture;
uniform vec4 u_backgroundColor;
uniform vec4 u_foregroundColor;

void main()
{
    vec4 color = texture(u_ourTexture, v_texCoord);
    if (color.r > 0)
    {
        o_fragColor = u_foregroundColor;
    }
    else
    {
        o_fragColor = u_backgroundColor;
    }
}

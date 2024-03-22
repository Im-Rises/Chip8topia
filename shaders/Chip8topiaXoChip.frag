#version 330 core

#define TEXTURES_COUNT 4
#define COLORS_COUNT 16

in vec2 v_texCoord;

out vec4 o_fragColor;

uniform sampler2D u_textures[TEXTURES_COUNT];
uniform vec4 u_colors[COLORS_COUNT];

void main()
{
    int colorIndex = 0;

    for (int i = 0; i < TEXTURES_COUNT; i++)
    {
        vec4 color = texture(u_textures[i], v_texCoord);

        if (color.r > 0.0)
        {
            colorIndex++;
        }
    }

    o_fragColor = u_colors[colorIndex];
}

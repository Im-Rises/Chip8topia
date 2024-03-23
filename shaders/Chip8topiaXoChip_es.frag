#version 300 es

precision mediump float;

#define TEXTURES_COUNT 4
#define COLORS_COUNT 16

in vec2 v_texCoord;

out vec4 o_fragColor;

uniform sampler2D u_textures[TEXTURES_COUNT];
uniform vec4 u_colors[COLORS_COUNT];

void main() {
    int colorIndex = 0;

    colorIndex = texture(u_textures[0], v_texCoord).r > 0.0 ? 1 : 0;
    colorIndex |= texture(u_textures[1], v_texCoord).r > 0.0 ? 2 : 0;
    colorIndex |= texture(u_textures[2], v_texCoord).r > 0.0 ? 4 : 0;
    colorIndex |= texture(u_textures[3], v_texCoord).r > 0.0 ? 8 : 0;

    o_fragColor = u_colors[colorIndex];
}

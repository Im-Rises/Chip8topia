#version 300 es

precision mediump float;

layout (location = 0) in vec2 a_textCoord;

out vec2 v_texCoord;

uniform vec2 u_scaleFactor;

void main()
{
    gl_Position = vec4(u_scaleFactor * a_textCoord, 0.0, 1.0);
    vec2 screenCordinate = 0.5 * a_textCoord + 0.5;
    v_texCoord = vec2(screenCordinate.x, 1.0 - screenCordinate.y);
}

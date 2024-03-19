#version 330 core

precision mediump float;

in vec2 v_texCoord;

out vec4 o_fragColor;

uniform sampler2D u_mainPlaneTexture;
uniform sampler2D u_subPlaneTexture;

uniform vec4 u_backgroundColor;
uniform vec4 u_mainPlaneColor;
uniform vec4 u_subPlaneColor;
uniform vec4 u_commonPixelsColor;

void main()
{
    vec4 colorMainPlane = texture(u_mainPlaneTexture, v_texCoord);
    vec4 colorSubPlane = texture(u_subPlaneTexture, v_texCoord);

    if (colorMainPlane.r > 0.0 && colorSubPlane.r > 0.0)
    {
        o_fragColor = u_commonPixelsColor;
    }
    else if (colorMainPlane.r > 0.0)
    {
        o_fragColor = u_mainPlaneColor;
    }
    else if (colorSubPlane.r > 0.0)
    {
        o_fragColor = u_subPlaneColor;
    }
    else
    {
        o_fragColor = u_backgroundColor;
    }
}

#version 330 core

in vec2 v_texCoord;

out vec4 o_fragColor;

uniform sampler2D u_ourTexture;

void main()
{
    //    FragColor = texture(ourTexture, TexCoord);
    vec4 color = texture(u_ourTexture, v_texCoord);
    if (color.r == 1.0)
    {
        o_fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    else
    {
        o_fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

//#version 330 core
//
//out vec4 FragColor;
//
//void main()
//{
//    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
//}

//precision highp float;
//
//in vec2 v_texcoord;
//uniform sampler2D u_texture;
//uniform vec4 u_backgroundColor;
//uniform vec4 u_foregroundColor;
//out vec4 o_outColor;
//
//void main() {
//    vec4 color = texture(u_texture, v_texcoord);
//
//    if (color.rgb == vec3(0.0))
//    {
//        color = u_backgroundColor;
//    }
//    else
//    {
//        color = u_foregroundColor;
//    }
//
//    o_outColor = color;
//}

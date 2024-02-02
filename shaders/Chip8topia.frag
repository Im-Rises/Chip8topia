#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

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

#version 300 es

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

//precision highp float;
//
//in vec4 a_texcoord;
//out vec2 v_texcoord;
//
//void main() {
//    v_texcoord = a_texcoord.xy * 0.5 + 0.5;
//    gl_Position = a_texcoord;
//}

#version 330 core

layout (location = 0) in vec3 a_Pos;

out vec2 v_texCoord;

void main()
{
    gl_Position = vec4(a_Pos, 1.0);
    v_texCoord = a_Pos.xy * 0.5 + 0.5;
    //    v_texCoord = a_Pos.xy;
}

//#version 330 core
//
//layout (location = 0) in vec3 aPos;
//
//void main()
//{
//    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
//}

//precision highp float;
//
//in vec4 a_texcoord;
//out vec2 v_texcoord;
//
//void main() {
//    v_texcoord = a_texcoord.xy * 0.5 + 0.5;
//    gl_Position = a_texcoord;
//}

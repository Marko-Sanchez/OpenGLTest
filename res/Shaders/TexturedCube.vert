#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tcords;

uniform mat4 u_mvp;

out vec2 v_TexCord;

void main()
{
    v_TexCord = tcords;
    gl_Position = u_mvp * vec4(pos, 1.0);
}

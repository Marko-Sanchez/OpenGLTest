#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCords;
layout (location = 2) in int  a_TexID;

out vec2     v_TexCords;
flat out int v_TexID;

void main()
{
    v_TexCords = a_TexCords;
    v_TexID    = a_TexID;

    gl_Position = vec4(a_Position, 1.0);
}

#version 330 core

out vec4 f_Color;

in vec2     v_TexCords;
flat in int v_TexID;

uniform sampler2D u_Textures[2];

void main()
{
    switch(v_TexID)
    {
        case 0: f_Color = texture(u_Textures[0], v_TexCords); break;
        case 1: f_Color = texture(u_Textures[1], v_TexCords); break;
        default: f_Color = vec4(v_TexCords, 0.0f, 1.0f);
    }
}

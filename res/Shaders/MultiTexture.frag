#version 330 core

out vec4 f_Color;

in vec2     v_TexCord;
flat in int v_TexIndex;

uniform sampler2D u_Textures[3];

void main()
{
    switch(v_TexIndex)
    {
        case 0: f_Color = texture(u_Textures[0], v_TexCord); break;
        case 1: f_Color = texture(u_Textures[1], v_TexCord); break;
        case 2: f_Color = texture(u_Textures[2], v_TexCord); break;
        default: f_Color = vec4(v_TexCord, 0.0f, 1.0f); break;
    }
}

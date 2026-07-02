#version 330 core

out vec4 f_Color;

in vec2 v_TexCoords;

uniform sampler2D texture_normal1;

void main()
{
    f_Color = texture(texture_normal1, v_TexCoords);
}

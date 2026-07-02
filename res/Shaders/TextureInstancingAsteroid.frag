#version 330 core

out vec4 f_Color;

in vec2 v_TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    f_Color = texture(texture_diffuse1, v_TexCoords);
}

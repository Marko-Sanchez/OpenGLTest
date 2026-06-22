#version 330 core

in vec3 TexCoords;

out vec4 o_fragcolor;

uniform samplerCube skybox;

void main()
{
    o_fragcolor = texture(skybox, TexCoords);
}

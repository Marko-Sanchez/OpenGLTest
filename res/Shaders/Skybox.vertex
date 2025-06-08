#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    TexCoords   = aPos;
    vec4 pos    = u_projection * u_view * vec4(aPos, 1.0);
    // trick the depth buffer by giving skybox the max depth buffer.
    // we want to render skybox last, but if we do it will render on top of
    // other objects; hence we alter its depth buffer.
    gl_Position = pos.xyww;
}

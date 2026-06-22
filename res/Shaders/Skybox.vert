#version 330 core

layout (location = 0) in vec3 a_position;

out vec3 v_texCoords;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    v_texCoords = a_position;
    vec4 pos    = u_projection * u_view * vec4(a_position, 1.0);

    // trick the depth buffer by giving skybox the max depth buffer.
    // we want to render skybox last, but if we do it will render on top of
    // other objects; hence we alter its depth buffer.
    gl_Position = pos.xyww;
}

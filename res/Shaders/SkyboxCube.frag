#version 330 core

out vec4 f_color;

in vec3 v_normal;
in vec3 v_position;

uniform vec3        u_viewPos;
uniform samplerCube u_skybox;

void main()
{
    vec3 I = normalize(v_position - u_viewPos);
    vec3 R = reflect(I, normalize(v_normal));

    // use skybox texture to create a reflection on the cube.
    f_color = vec4(texture(u_skybox, R).rgb, 1.0);
}

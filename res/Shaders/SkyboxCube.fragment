#version 330 core

in vec3 Normal;
in vec3 Position;

out vec4 FragColor;

uniform vec3        u_cameraPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(Position - u_cameraPos);
    vec3 R = reflect(I, normalize(Normal));

    // use skybox texture to create a reflection on the cube.
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}

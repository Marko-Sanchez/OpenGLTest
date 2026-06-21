#version 330 core
out vec4 color;

in vec3 FragNormal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 u_viewpos;

// diffuse
uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_diffuse2;

// specular
uniform sampler2D texture_specular1;
//uniform sampler2D texture_specular2;

// normal
//uniform sampler2D texture_normal1;
//uniform sampler2D texture_normal2;

void main()
{
    vec3 norm     = normalize(FragNormal);
    vec3 lightDir = normalize(vec3(1.f, 1.f, 1.f));
    vec3 viewDir  = normalize(u_viewpos - FragPos);
    vec3 halfDir  = normalize(lightDir + viewDir);

    float ambient = 0.15;

    // diffuse: how directly does the light hit the surface.
    float diff          = max(dot(norm, lightDir), 0.0);
    vec4  diffuseSample = texture(texture_diffuse1, TexCoords);

    // specular: how shiny a pixel is.
    float spec           = pow(max(dot(norm, halfDir), 0.0), 32.0);
    vec3  specularSample = texture(texture_specular1, TexCoords).rgb;

    vec3 result = diffuseSample.rgb * (ambient + diff) // diffuse contribution.
                 + specularSample * spec;              // specular contribution.

    color = vec4(result, diffuseSample.a);
}

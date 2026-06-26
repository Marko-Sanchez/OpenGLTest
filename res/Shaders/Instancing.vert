#version 330 core

// per-vertex attributes.
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec4 a_color;

// per-instance attribute.
layout (location = 2) in vec2 a_translation;

out vec4 v_color;

void main()
{
    // 'pos' is not needed just being used to demonstrate gl_InstanceID.
    vec2 pos    = a_position * (gl_InstanceID / 100.0);
    gl_Position = vec4(pos + a_translation, 0.0f, 1.0f);
    v_color     = a_color;
}

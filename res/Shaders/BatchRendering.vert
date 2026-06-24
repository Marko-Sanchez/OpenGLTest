#version 330 core

// location gets set in VertexArray: glVertexAttribPointer
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec4 a_color;

// send color to fragment.
out vec4 v_color;

uniform mat4 u_MVP;

void main()
{
    v_color = a_color;
    gl_Position = u_MVP * vec4(a_position, 0.0, 1.0);
}

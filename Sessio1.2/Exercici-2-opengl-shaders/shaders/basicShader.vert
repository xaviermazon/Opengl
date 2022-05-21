#version 330 core

in vec3 vertex;
out vec2 u_resolution;

void main()  {
    gl_Position = vec4 (vertex, 1.0);
    u_resolution = gl_Position.xy;
}
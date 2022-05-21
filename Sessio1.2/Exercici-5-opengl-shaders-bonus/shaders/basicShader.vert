#version 330 core

in vec3 vertex;
in vec3 color;
out vec2 u_resolution;
out vec3 fcolor;

void main() {
    vec3 vertexMini = vertex*0.5;
    gl_Position = vec4 (vertexMini, 1.0);
    u_resolution = gl_Position.xy;
    fcolor = color;
}
#version 330 core

in vec3 vertex;
out vec2 u_resolution;

void main() {
    vec3 vertexMini = vertex*0.5;
    gl_Position = vec4 (vertexMini, 1.0);
    u_resolution = gl_Position.xy;
}
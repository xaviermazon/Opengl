#version 330 core

out vec4 FragColor;
in vec2 u_resolution;

void main() {
    FragColor = vec4(0.);
    if(u_resolution.x < 0 && u_resolution.y > 0) FragColor = vec4(1., 0., 0., 1.);
    if(u_resolution.x > 0 && u_resolution.y > 0) FragColor = vec4(0., 0., 1., 1.);
    if(u_resolution.x < 0 && u_resolution.y < 0) FragColor = vec4(1., 1., 0., 1.);
    if(u_resolution.x > 0 && u_resolution.y < 0) FragColor = vec4(0., 1., 0., 1.);
}


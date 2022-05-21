#version 330 core

out vec4 FragColor;
in vec2 u_resolution;

void main() {
    FragColor = vec4(0.);
    if(u_resolution.x < 0 && u_resolution.y > 0) FragColor = vec4(1., 0., 0., 1.);
    if(u_resolution.x > 0 && u_resolution.y > 0) FragColor = vec4(0., 0., 1., 1.);
    if(u_resolution.x < 0 && u_resolution.y < 0) FragColor = vec4(1., 1., 0., 1.);
    if(u_resolution.x > 0 && u_resolution.y < 0) FragColor = vec4(0., 1., 0., 1.);
    for(float i = -1.0; i < u_resolution.y; i+=0.06) {
        if(u_resolution.y > i && u_resolution.y < i+0.03) discard;
    }
}


#version 330 core

out vec4 FragColor;
in vec2 u_resolution;
in vec3 fcolor;

void main() {
    FragColor = vec4(fcolor.rgb, 1.);
    //if(u_resolution.x < 1 && u_resolution.y < 1) FragColor = vec4(fcolor.rbg, 1.);
    //if(u_resolution.y > -1) FragColor = vec4(fcolor.rbg, 1.);
}
#version 330 core

// Location variables
layout (location = 0) in vec2 l_vert;
layout (location = 1) in float l_tex_coord;


// Uniform variables
uniform vec2 u_scale;
uniform vec2 u_pos;


// Out variables
out float pos_x;
out float tex_coord;


// Main function
void main() {
    // Set the out variables
    pos_x = l_vert.x;
    tex_coord = l_tex_coord;

    // Set the vertex position
    gl_Position = vec4((l_vert + u_pos) * u_scale, 0.0F, 1.0F);
}
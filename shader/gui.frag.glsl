#version 330 core

// Out color
out vec4 color;


// Uniform variables
uniform int u_shape;
uniform vec3 u_color;
uniform sampler1D u_trans_func;


// In variables
in float tex_coord;


// Main function
void main () {
    // Sample the texture or color
    color = u_shape == 1 ? texture(u_trans_func, tex_coord) : vec4(u_color * tex_coord, 1.0F);
}
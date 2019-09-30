#version 330 core

// Out color
out vec4 color;


// Uniform variables
uniform sampler3D u_tex;
uniform sampler1D u_trans_func;


// In variables
in vec3 tex_coord;


// Main function
void main () {
    // Get the data from the texture and map to the transfer function
    color = texture(u_trans_func, texture(u_tex, tex_coord).r);
}
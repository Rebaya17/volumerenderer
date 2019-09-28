#version 330 core

// Out color
out vec4 color;


// Uniform variables
uniform sampler3D u_tex;


// In variables
in vec3 tex_coord;


// Main function
void main () {
    // Get the data from the texture
    float val = texture(u_tex, tex_coord).r;

    // Set the normal as color
    color = vec4(val);
}
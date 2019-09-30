#version 330 core

// Out color
out vec4 color;


// Uniform variables
uniform vec3 u_color;


// Main function
void main () {
    // Set the color
    color = vec4(u_color, 1.0F);
}
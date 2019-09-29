#version 330 core

// Location variables
layout (location = 0) in vec2 l_vert;


// Main function
void main() {
    // Set the vertex position
    gl_Position = vec4(l_vert, 0.0F, 1.0F);
}
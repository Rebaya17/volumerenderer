#version 330 core

// Location variables
layout (location = 0) in vec2 l_position;


// Uniform variables
uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;

uniform mat4 u_model_mat;
uniform float slice;


// Out variables
out Vertex {
    vec3 position;
    vec2 uv_coord;
} vertex;


// Main function
void main() {
    // Vertex position
    vec4 pos = u_model_mat * vec4(vec3(l_position, slice), 1.0F);

    // Set out variables
    vertex.position = pos.xyz;
    vertex.uv_coord = l_position;

    // Set vertex position
    gl_Position = u_projection_mat * u_view_mat * pos;
}
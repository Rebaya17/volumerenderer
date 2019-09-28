#version 330 core

// Location variables
layout (location = 0) in vec2 l_quad;


// Uniform variables
uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;

uniform mat4 u_model_mat;
uniform mat4 u_volume_mat;
uniform float u_slice;


// Out variables
out vec3 tex_coord;


// Main function
void main() {
    // Homogeneous quad
    vec4 quad = vec4(l_quad.x, l_quad.y, u_slice, 1.0F);

    // Set out variables
    tex_coord = (u_volume_mat * quad).stp;
    tex_coord.t = 1.0F - tex_coord.t;

    // Set vertex position
    gl_Position = u_projection_mat * u_view_mat * u_model_mat * quad;
}
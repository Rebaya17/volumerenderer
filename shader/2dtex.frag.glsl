#version 330 core

// Out color
out vec4 color;


// Uniform variables
uniform sampler2D u_tex;


// In variables
in Vertex {
    vec3 position;
    vec2 uv_coord;
} vertex;


// Main function
void main () {
    // Get the data from the texture
    float val = texture(u_tex, vertex.uv_coord).r;;

    // Discard out of range
    if (val < 0.2F || val > 0.8F) {
        discard;
    }

    // Set the normal as color
    color = vec4(vec3(val), 1.0F);
}
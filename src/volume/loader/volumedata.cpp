#include "volumedata.hpp"


// Volume data constructor
VolumeData::VolumeData(const std::string &path, const VolumeData::Format &format) :
    // Status
    open(false),

    // Path
    path(path),
    
    // Format
    format(format),
    
    // Resolution
    resolution(0U),

    // Buffers
    vao(GL_FALSE),
    vbo(GL_FALSE),

    // Textures array
    texture(GL_FALSE) {}


// Destructor

// Volume data destructor
VolumeData::~VolumeData() {
    // Vertex buffer object
    if (vbo != GL_FALSE) {
        glDeleteBuffers(1, &vbo);
        vbo = GL_FALSE;
    }

    // Vertex array object
    if (vao != GL_FALSE) {
        glDeleteBuffers(1, &vao);
        vao = GL_FALSE;
    }

    // Texture
    if (texture != GL_FALSE) {
        glDeleteTextures(1, &texture);
        texture = GL_FALSE;
    }
}
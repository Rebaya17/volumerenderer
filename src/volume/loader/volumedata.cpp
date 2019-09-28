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
    if (texture != GL_FALSE) {
        glDeleteTextures(1, &texture);
        texture = GL_FALSE;
    }
}
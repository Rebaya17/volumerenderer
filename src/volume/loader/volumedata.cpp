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
    texture(nullptr) {}


// Destructor

// Volume data destructor
VolumeData::~VolumeData() {
    if (texture != nullptr) {
        glDeleteTextures(resolution.z, texture);
        delete[] texture;
    }
}
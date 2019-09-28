#include "volumeloader.hpp"

#include "rawloader.hpp"
#include "pvmloader.hpp"

#include <iostream>


// Private constructors

/** Volume loader constructor */
VolumeLoader::VolumeLoader(const std::string &path, const VolumeData::Format &format) :
    // Volume data
    volume_data(new VolumeData(path, format)),

    // Voxel data
    voxel(nullptr),
    size(0U) {}


// Private methods

// Load data to GPU
void VolumeLoader::load() {
    // Generate and load textures
    const GLenum bytes = volume_data->format == VolumeData::RAW8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT;
    glGenTextures(1, &volume_data->texture);

    // Bind texture
    glBindTexture(GL_TEXTURE_3D, volume_data->texture);

    // Texture parameters
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Load texture
    const unsigned int x = volume_data->resolution.x;
    const unsigned int y = volume_data->resolution.y;
    const unsigned int z = volume_data->resolution.z;
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, x, y, z, 0, GL_RED, bytes, voxel);

    // Unbind texture
    glBindTexture(GL_TEXTURE_3D, GL_FALSE);


    // Vertex array object
    glGenVertexArrays(1, &volume_data->vao);
    glBindVertexArray(volume_data->vao);

    // Vertex buffer object
    glGenBuffers(1, &volume_data->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, volume_data->vbo);

    // XY square data to draw as triangle strip
    const GLfloat square[] = {-0.5F, -0.5F, -0.5F, 0.5F, 0.5F, -0.5F, 0.5F, 0.5F};
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) << 1, nullptr);

    // Unbind vertex array object
    glBindVertexArray(GL_FALSE);
}


// Destructor

// Virtual volume loader destructor
VolumeLoader::~VolumeLoader() {
    if (voxel != nullptr) {
        delete[] voxel;
    }
}


// Public static methods

// Read and load data
VolumeData *VolumeLoader::load(const std::string &path, const VolumeData::Format &format, const unsigned int &width, const unsigned int &height, const unsigned int &depth) {
    // Create a null volume loader
    VolumeLoader *loader = nullptr;

    // Instanciate the loader
    switch (format) {
        // RAW formats
        case VolumeData::RAW8:
        case VolumeData::RAW16: loader = static_cast<VolumeLoader *>(new RAWLoader(path, format)); break;
        
        // PVM format
        case VolumeData::PVM: loader = static_cast<VolumeLoader *>(new PVMLoader(path, format)); break;

        // Return empty volume data if the format is unknown
        default:
            std::cerr << "error: unknown volume format `" << format << "'" << std::endl;
            return new VolumeData(path);
    }

    // Read and load data
    if (loader->read(width, height, depth)) {
        loader->volume_data->open = true;
        loader->load();
    }

    // Get the volume data and delete loader
    VolumeData *volume_data = loader->volume_data;
    delete loader;

    // Return the volume data
    return volume_data;
}
#include "rawloader.hpp"

#include <iostream>
#include <fstream>

#include <cmath>


// Private methods

// Read data from file
bool RAWLoader::read() {
    // Open the model file and check it
    std::ifstream file(volume_data->path);
    if (!file.is_open()) {
        std::cerr << "error: could not open the volume `" << volume_data->path << "'" << std::endl;
        return false;
    }

    // Read the resolution
    volume_data->resolution.x = 256U;
    volume_data->resolution.y = 256U;
    volume_data->resolution.z = 512U;

    // Read the voxel data
    size = static_cast<std::size_t>(volume_data->resolution.x * volume_data->resolution.y * volume_data->resolution.z);
    voxel = new GLushort[size];
    file.read(reinterpret_cast<char *>(voxel), sizeof(GLushort) * size);

    // Close the file and set the volume open
    file.close();

    return true;
}


// Constructor

// RAW loader constructor
RAWLoader::RAWLoader(const std::string &path, const VolumeData::Format &format) :
   VolumeLoader(path, format) {}
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
    unsigned short resolution[3];
    file.read(reinterpret_cast<char *>(&resolution[0]), VolumeLoader::USHORT_SIZE * 3U);
    volume_data->resolution.x = static_cast<unsigned int>(resolution[0]);
    volume_data->resolution.y = static_cast<unsigned int>(resolution[1]);
    volume_data->resolution.z = static_cast<unsigned int>(resolution[2]);

    // Set the YZ step and the voxel data size
    step = static_cast<std::size_t>(resolution[1]) * static_cast<std::size_t>(resolution[2]);
    size = static_cast<std::size_t>(resolution[0]) * step;

    // Read the voxel data
    voxel = new unsigned short[size];
    file.read(reinterpret_cast<char *>(&voxel[0]), VolumeLoader::USHORT_SIZE * size);

    return true;
}


// Constructor

// RAW loader constructor
RAWLoader::RAWLoader(const std::string &path, const VolumeData::Format &format) :
   VolumeLoader(path, format) {}
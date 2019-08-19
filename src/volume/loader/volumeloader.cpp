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
    voxel(nullptr) {}


// Private methods

// Load data to GPU
void VolumeLoader::load() {

}


// Destructor

// Virtual volume loader destructor
VolumeLoader::~VolumeLoader() {
    if (voxel != nullptr) {
        delete voxel;
    }
}


// Public static methods

// Read and load data
VolumeData *VolumeLoader::load(const std::string &path, const VolumeData::Format &format) {
    // Create a null volume loader
    VolumeLoader *loader = nullptr;

    // Instanciate the loader
    switch (format) {
        // RAW format
        case VolumeData::RAW: loader = static_cast<VolumeLoader *>(new RAWLoader(path, format)); break;
        
        // PVM format
        case VolumeData::PVM: loader = static_cast<VolumeLoader *>(new PVMLoader(path, format)); break;

        // Return empty volume data if the format is unknown
        default:
            std::cerr << "error: unknown volume format `" << format << "'" << std::endl;
            return new VolumeData(path);
    }

    // Read and load data
    if (loader->read()) {
        loader->load();
    }

    // Get the volume data and delete loader
    VolumeData *volume_data = loader->volume_data;
    delete loader;

    // Return the volume data
    return volume_data;
}
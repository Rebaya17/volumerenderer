#include "pvmloader.hpp"


// Private methods

// Read data from file
bool PVMLoader::read(const unsigned int &, const unsigned int &, const unsigned int &) {
    return false;
}


// Constructor

// PVM loader constructor
PVMLoader::PVMLoader(const std::string &path, const VolumeData::Format &format) :
   VolumeLoader(path, format) {}
#include "rawloader.hpp"


// Private methods

// Read data from file
bool RAWLoader::read() {
    return false;
}


// Constructor

// RAW loader constructor
RAWLoader::RAWLoader(const std::string &path, const VolumeData::Format &format) :
   VolumeLoader(path, format) {}
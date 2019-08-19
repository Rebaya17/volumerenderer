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
    resolution(0U) {}
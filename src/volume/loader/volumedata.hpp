#ifndef __VOLUME_DATA_HPP_
#define __VOLUME_DATA_HPP_

#include <glm/vec3.hpp>

#include <string>


/** Volume data class */
class VolumeData {
    private:
        // Constructors

        /** Disable the default constructor */
        VolumeData() = delete;

        /** Disable the default copy constructor */
        VolumeData(const VolumeData &) = delete;

        /** Disable the assignation operator */
        VolumeData &operator=(const VolumeData &) = delete;


    public:
        // Enumerations
        
        /** Formats */
        enum Format {
            /** RAW */
            RAW,

            /** PVM */
            PVM,

            /** Unknown */
            UNKOWN
        };


        // Attributes

        /** Open status */
        bool open;


        /** Volume path */
        std::string path;

        /** Volume format */
        VolumeData::Format format;


        /** Resolution */
        glm::uvec3 resolution;


        // Constructor

        /** Volume data constructor */
        VolumeData(const std::string &path, const VolumeData::Format &format = VolumeData::UNKOWN);
};

#endif // __VOLUME_DATA__
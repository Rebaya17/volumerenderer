#ifndef __VOLUME_DATA_HPP_
#define __VOLUME_DATA_HPP_

#include "../../glad/glad.h"
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
            /** RAW 8 bits */
            RAW8,

            /** RAW 16 bits */
            RAW16,

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


        /** Vertex array object */
        GLuint vao;

        /** Vertex buffer object */
        GLuint vbo;


        /** Textures array */
        GLuint *texture;


        // Constructor

        /** Volume data constructor */
        VolumeData(const std::string &path, const VolumeData::Format &format = VolumeData::UNKOWN);


        // Destructor
        virtual ~VolumeData();
};

#endif // __VOLUME_DATA__
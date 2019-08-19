#ifndef __VOLUME_LOADER_HPP_
#define __VOLUME_LOADER_HPP_

#include "volumedata.hpp"

#include <glm/vec3.hpp>

#include <string>


/** Volume loader class */
class VolumeLoader {
    private:
        // Constructors

        /** Disable the default constructor */
        VolumeLoader() = delete;

        /** Disable the default copy constructor */
        VolumeLoader(const VolumeLoader &) = delete;

        /** Disable the assignation operator */
        VolumeLoader &operator=(const VolumeLoader &) = delete;


    protected:
        // Constructors

        /** Volume loader constructor */
        VolumeLoader(const std::string &path, const VolumeData::Format &format);


        // Attributes

        /** Volume data */
        VolumeData *volume_data;

        /** Voxel data */
        glm::uvec3 *voxel;


        // Methods

        /** Read file */
        virtual bool read() = 0;

        /** Load data to the GPU */
        void load();


    public:
        // Destructor

        /** Volume loader destructor */
        virtual ~VolumeLoader();


        // Static methods

        /** Read volume */
        static VolumeData *load(const std::string &path, const VolumeData::Format &format = VolumeData::UNKOWN);
};

#endif // __VOLUME_LOADER_HPP_
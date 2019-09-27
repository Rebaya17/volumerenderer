#ifndef __RAW_LOADER_HPP_
#define __RAW_LOADER_HPP_

#include "volumeloader.hpp"


/** Raw volume loader class */
class RAWLoader : public VolumeLoader {
    private:
        // Methods

        /** Read file */
        virtual bool read(const unsigned int &width = 0U, const unsigned int &height = 0U, const unsigned int &depth = 0U);


    public:
        // Constructors

        /** RAW loader constructor */
        RAWLoader(const std::string &path, const VolumeData::Format &format = VolumeData::UNKOWN);
};

#endif // __RAW_LOADER_HPP_
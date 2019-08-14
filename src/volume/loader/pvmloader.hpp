#ifndef __PVM_LOADER_HPP_
#define __PVM_LOADER_HPP_

#include "volumeloader.hpp"


/** PVM volume loader class */
class PVMLoader : public VolumeLoader {
    private:
        // Methods

        /** Read file */
        virtual bool read();


    public:
        // Constructors

        /** PVM loader constructor */
        PVMLoader(const std::string &path, const VolumeData::Format &format = VolumeData::UNKOWN);
};

#endif // __PVM_LOADER_HPP_
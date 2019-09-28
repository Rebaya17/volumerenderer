#ifndef __TRANSFER_FUNCTION_HPP_
#define __TRANSFER_FUNCTION_HPP_

#include "../scene/glslprogram.hpp"

#include "../glad/glad.h"


/** The transfer function class */
class TransferFunction {
    private:
        // Attributes

        /** Texture index */
        GLint index;

        /** Texture ID */
        GLuint texture;

        /** Texture data */
        GLubyte texture_data[1024];


        // Constructors

        /** Disable the default copy constructor */
        TransferFunction(const TransferFunction &) = delete;

        /** Disable the assignation operator */
        TransferFunction &operator=(const TransferFunction &) = delete;


        // Methods

        /** Update the texture */
        void update() const;



    public:
        // Constructors

        /** Transfer function constructor */
        TransferFunction(const GLint &index = 0);


        // Methods

        /** Bind the transfer function */
        void bind(GLSLProgram *const program);

        /** Reset */
        void reset();


        // Destructor

        /** Transfer function destructor */
        ~TransferFunction();
};

#endif // __TRANSFER_FUNCTION_HPP_
#include "transferfunction.hpp"


// Private methods

// Update the texture
void TransferFunction::update() const {
    // Bind texture
    glBindTexture(GL_TEXTURE_1D, texture);

    // Update the data
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

    // Unbind texture
    glBindTexture(GL_TEXTURE_1D, GL_FALSE);
}


// Constructors

/** Transfer function constructor */
TransferFunction::TransferFunction(const GLint &index) :
    // Texture attributes
    index(index),
    texture(GL_FALSE) {
    // Initialize the data array
    for (unsigned int i = 0, j = 0; i < 256; i++, j += 4) {
        texture_data[j    ] = i;
        texture_data[j + 1] = i;
        texture_data[j + 2] = i;
        texture_data[j + 3] = i;
    }

    // Generate the texture
    glGenTextures(1, &texture);

    // Bind texture
    glBindTexture(GL_TEXTURE_1D, texture);

    // Textur parameters
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Unbind texture
    glBindTexture(GL_TEXTURE_1D, GL_FALSE);

    // Update the texture data
    update();
}


// Methods

// Bind the transfer function
void TransferFunction::bind(GLSLProgram *const program) {
    // Check the program status
    if ((program == nullptr) || (!program->isValid())) {
        return;
    }

    // Set uniform
    program->setUniform("u_trans_func", index);

    // Bind texture
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_1D, texture);
}


// Destructor

// Transfer function destructor
TransferFunction::~TransferFunction() {
    glDeleteTextures(1, &texture);
}
#include "volume.hpp"

#include "../dirsep.h"


// Private methods

// Load the volume from the volume path
void Volume::load() {
    // Load the volume
    VolumeData *volume_data = VolumeLoader::load(path, format, resolution.x, resolution.y, resolution.z);

    // Set the open statuses
    open = volume_data->open;

    // Set the resolution
    resolution = volume_data->resolution;

    // Set the buffers
    vao = volume_data->vao;
    vbo = volume_data->vbo;

    // Set the texture
    texture = volume_data->texture;
    diagonal = glm::length(glm::vec3(resolution));
    step = 1.0F / diagonal;
    tex_dim = glm::vec3(resolution) / diagonal;

    // Clean up the loader data
    volume_data->texture = GL_FALSE;
    delete volume_data;
}

// Makes the volume empty
void Volume::clear() {
    open = false;
}

// Update the matrices
void Volume::updateMatrices() {
    // Identity matrix
    const glm::mat4 identity = glm::mat4(1.0F);

    // Update the matrices
    model_mat = glm::scale(glm::translate(identity, position), dimension);
    volume_mat = glm::inverse(glm::mat4_cast(rotation) * glm::translate(glm::scale(identity, tex_dim), glm::vec3(-0.5F)));
}

// Constructor

// Empty volume constructor
Volume::Volume() :
    VolumeData(std::string()),

    // Enabled
    enabled(true),

    // Geometry
    position(0.0F),
    rotation(glm::quat(1.0F, 0.0F, 0.0F, 0.0F)),
    dimension(1.0F),

    // Texture
    diagonal(0.0F),
    step(0.0F),
    tex_dim(0.0F),

    // Matrices
    model_mat(1.0F),
    volume_mat(1.0F) {}

// Volume constructor
Volume::Volume(const std::string &path, const VolumeData::Format &format) :
    VolumeData(path, format),

    // Enabled
    enabled(true),

    // Geometry
    position(0.0F),
    rotation(1.0F, 0.0F, 0.0F, 0.0F),
    dimension(1.0F),

    // Texture
    diagonal(0.0F),
    step(0.0F),
    tex_dim(0.0F),

    // Matrices
    model_mat(1.0F),
    volume_mat(1.0F) {
    // Load the volume
    load();
}


// Getters

// Get the enabled status
bool Volume::isEnabled() const {
    return enabled;
}

// Get the open status
bool Volume::isOpen() const {
    return open;
}


// Get the volume file path
std::string Volume::getPath() const {
    return path;
}

// Get the volume name
std::string Volume::getName() const {
    return path.substr(path.find_last_of(DIR_SEP) + 1U);
}


// Get the resolution
glm::uvec3 Volume::getResolution() const {
    return open ? resolution : glm::uvec3();
}


// Get the position
glm::vec3 Volume::getPosition() const {
    return open ? position : glm::vec3();
}

// Get the rotation quaternion
glm::quat Volume::getRotation() const {
    return open ? rotation : glm::quat(0.0F, 0.0F, 0.0F, 1.0F);
}

// Get the rotation angles
glm::vec3 Volume::getRotationAngles() const {
    return glm::degrees(glm::eulerAngles(rotation));
}

// Get the scale
glm::vec3 Volume::getScale() const {
    return dimension;
}


// Get the model matrix
glm::mat4 Volume::getModelMatrix() const {
    return model_mat;
}

// Get the Volume matrix
glm::mat4 Volume::getVolumeMatrix() const {
    return volume_mat;
}


// Setters

// Set the enabled status
void Volume::setEnabled(const bool &status) {
    enabled = status;
}


// Set the new path
void Volume::setPath(const std::string &new_path, const VolumeData::Format &new_format, const unsigned int &width, const unsigned int &height, const unsigned int &depth) {
    path = new_path;
    format = new_format;
    resolution.x = width;
    resolution.y = height;
    resolution.z = depth;
    reload();
}


// Set the new position
void Volume::setPosition(const glm::vec3 &new_position) {
    position = new_position;
    updateMatrices();
}

// Set the new rotation angles in degrees
void Volume::setRotation(const glm::vec3 &new_rotation) {
    rotation = glm::quat(glm::radians(new_rotation));
    updateMatrices();
}

// Set the new rotation quaternion
void Volume::setRotation(const glm::quat &new_rotation) {
    rotation = new_rotation;
    updateMatrices();
}

// Set the new scale
void Volume::setScale(const glm::vec3 &new_scale) {
    dimension = new_scale;
    updateMatrices();
}


// Methods

// Reload volume
void Volume::reload() {
    // Clear the volume data
    clear();

    // Load if the path is not empty
    if (!path.empty()) {
        load();
        resetGeometry();
    }
}

// Reset geometry
void Volume::resetGeometry() {
    // Set the default values
    position = glm::vec3(0.0F);
    rotation = glm::quat(1.0F, 0.0F, 0.0F, 0.0F);
    dimension = glm::vec3(1.0F);

    // Update matrices
    updateMatrices();
}


// Draw the volume
void Volume::draw(GLSLProgram *const program) const {
    // Check volume and program status
    if (!enabled || !open || (program == nullptr) || (!program->isValid())) {
        return;
    }

    // Use the program
    program->use();

    // Set volume uniforms
    program->setUniform("u_model_mat", model_mat);
    program->setUniform("u_volume_mat", volume_mat);
    program->setUniform("u_tex", GL_TEXTURE0);

    // Bind the vertex array object and texture
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_3D, texture);

    // Draw the volume slices
    for (float i = -0.5F; i < 0.5F; i += step) {
        // Set the slice position
        program->setUniform("u_slice", i);

        // Draw square
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // Unbind the vertex array object and texture
    glBindTexture(GL_TEXTURE_3D, GL_FALSE);
    glBindVertexArray(GL_FALSE);
}


// Translate the volume
void Volume::translate(const glm::vec3 &delta) {
    position += delta;
    updateMatrices();
}

// Rotate certain degrees
void Volume::rotate(const glm::vec3 &delta) {
    rotation = glm::normalize(glm::quat(glm::radians(delta)) * rotation);
    updateMatrices();
}

// Rotate applying the quaternion
void Volume::rotate(const glm::quat &factor) {
    rotation = glm::normalize(factor * rotation);
    updateMatrices();
}

// Scale the given factor
void Volume::scale(const glm::vec3 &factor) {
    // Scale
    dimension *= factor;

    // Fix not a number
    if (!std::isfinite(dimension.x)) dimension.x = 0.001F;
    if (!std::isfinite(dimension.y)) dimension.y = 0.001F;
    if (!std::isfinite(dimension.z)) dimension.z = 0.001F;

    // Update matrices
    updateMatrices();
}


// Destructor

// Volume destructor
Volume::~Volume() {
    clear();
}
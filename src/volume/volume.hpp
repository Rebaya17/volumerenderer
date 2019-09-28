#ifndef __VOLUME_HPP_
#define __VOLUME_HPP_

#include "loader/volumedata.hpp"
#include "loader/volumeloader.hpp"
#include "../scene/glslprogram.hpp"

#include "../glad/glad.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

#include <string>


/** Volume class */
class Volume : private VolumeData {
    private:
        // Attributes

        /** Enabled status */
        bool enabled;


        /** Position */
        glm::vec3 position;

        /** Rotation */
        glm::quat rotation;

        /** Dimension */
        glm::vec3 dimension;


        /** Model matrix */
        glm::mat4 model_mat;

        /** Model matrix */
        glm::mat4 volume_mat;


        // Constructors

        /** Disable the default copy constructor */
        Volume(const Volume &) = delete;

        /** Disable the assignation operator */
        Volume &operator=(const Volume &) = delete;


        // Methods

        /** Load the volume from the volume pah */
        void load();

        /** Makes the volume empty */
        void clear();

        /** Update volume and normal matrices */
        void updateMatrices();


    public:
        // Constructor

        /** Empty volume */
        Volume();

        /** Volume constructor */
        Volume(const std::string &path, const VolumeData::Format &format = VolumeData::UNKOWN);


        // Getters

        /** Get the enabled status */
        bool isEnabled() const;

        /** Get the open status */
        bool isOpen() const;


        /** Get the volume path*/
        std::string getPath() const;

        /** Get the volume name */
        std::string getName() const;


        /** Get the resolution */
        glm::uvec3 getResolution() const;


        /** Get the position */
        glm::vec3 getPosition() const;

        /** Get the rotation quaternion */
        glm::quat getRotation() const;

        /** Get the rotation angles */
        glm::vec3 getRotationAngles() const;

        /** Get the scale */
        glm::vec3 getScale() const;


        /** Get the model matrix */
        glm::mat4 getModelMatrix() const;

        /** Get the volume matrix */
        glm::mat4 getVolumeMatrix() const;


        // Setters

        /** Set the enabled status */
        void setEnabled(const bool &status);


        /** Set the new path */
        void setPath(const std::string &new_path, const VolumeData::Format &new_format = VolumeData::UNKOWN, const unsigned int &width = 0U, const unsigned int &height = 0U, const unsigned int &depth = 0U);


        /** Set the new position */
        void setPosition(const glm::vec3 &new_position);

        /** Set the new rotation angles in degrees */
        void setRotation(const glm::vec3 &new_rotation);

        /** Set the new rotation quaternion */
        void setRotation(const glm::quat &new_rotation);

        /** Set the new scale */
        void setScale(const glm::vec3 &new_scale);


        // Methods

        /** Reload the volume */
        void reload();

        /** Reset geometry */
        void resetGeometry();


        /** Draw the volume */
        void draw(GLSLProgram *const program) const;


        /** Translate the volume */
        void translate(const glm::vec3 &delta);

        /** Rotate the volume certain degrees */
        void rotate(const glm::vec3 &delta);

        /** Rotate the volume applying the quaternion */
        void rotate(const glm::quat &factor);

        /** Scale the volume by given factor */
        void scale(const glm::vec3 &factor);


        // Destructor

        /** Volume destructor */
        virtual ~Volume();
};

#endif // __VOLUME_HPP_
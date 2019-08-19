#ifndef __MOUSE_HPP_
#define __MOUSE_HPP_

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>


class Mouse {
    private:
        // Attributes

        /** Enabled status */
        bool enabled;

        /** Pressed status */
        bool pressed;


        /** Window resolution */
        glm::uvec2 resolution;


        /** Translation point */
        glm::vec2 translation;

        /** Rotation point */
        glm::vec3 rotation;


        // Constructor

        /** Delete the default cosntructor */
        Mouse() = delete;


        // Static const attributes

        /** Rotation angle */
        static const float ROTATION_ANGLE;


    public:
        // Constructor

        /** Mouse constructor */
        Mouse(const int &width, const int &height);


        // Getters

        /** Get the enabled status */
        bool isEnabled() const;

        /** Get the pressed status */
        bool isPressed() const;


        /** Get the window resolution */
        glm::uvec2 getResolution() const;


        /** Get translation point */
        glm::vec2 getTranslationPoint() const;

        /** Get rotation point */
        glm::vec3 getRotationPoint() const;


        // Setters

        /** Set the enabled status */
        void setEnabled(const bool &status);

        /** Set the pressed status */
        void setPressed(const bool &status);


        /** Set the window resolution */
        void setResolution(const glm::uvec2 &new_resolution);

        /** Set the translation point */
        void setTranslationPoint(const double &x, const double &y);

        /** Set the rotation point */
        void setRotationPoint(const double &x, const double &y);


        // Methods

        /** Make a translation */
        glm::vec2 translate(const double &x, const double &y);

        /** Make a rotation */
        glm::quat rotate(const double &x, const double &y);


        /** Normalize a point */
        glm::vec2 normalizeToWindow(const double &x, const double &y);

        /** Project point to sphere */
        glm::vec3 projectToSphere(const double &x, const double &y);
};

#endif // __MOUSE_HPP_
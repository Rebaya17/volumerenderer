#ifndef __SCENE_HPP_
#define __SCENE_HPP_

#include "../volume/volume.hpp"

#include "camera.hpp"
#include "glslprogram.hpp"

#include "../glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

#include <string>

#include <map>


class Scene {
    protected:
        // Attributes

        /** Window */
        GLFWwindow *window;

        /** Window title */
        std::string title;

        /** Frame buffer width */
        int width;

        /** Frame buffer height */
        int height;


        /** Background color */
        glm::vec3 background_color;


        /** Camera */
        Camera *camera;

        /** Volume */
        Volume *volume;

        /** Program */
        GLSLProgram *program;


        /** Frames */
        unsigned long long int frames;


        // Constructors

        /** Disable the default constructor */
        Scene() = delete;

        /** Disable the default copy constructor */
        Scene(const Scene &) = delete;

        /** Disable the assignation operator */
        Scene &operator=(const Scene &) = delete;


        // Methods

        /** Draw the scene */
        void drawScene();


        // Static attributes

        /** Instances counter */
        static std::size_t instances;

        /** Glad loaded flag */
        static bool initialized_glad;


        /** Screen width */
        static GLsizei screen_width;

        /** Screen height */
        static GLsizei screen_height;


        /** OpenGL vendor */
        static const GLubyte *opengl_vendor;

        /** OpenGL renderer */
        static const GLubyte *opengl_renderer;

        /** OpenGL version */
        static const GLubyte *opengl_version;

        /** GLSL version */
        static const GLubyte *glsl_version;


        // Static methods

        /** GLFW error callback */
        static void errorCallback(int error, const char *description);

        /** GLFW framebuffer size callback */
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);


    public:
        // Constructor

        /** Scene constructor */
        Scene(const std::string &title, const int &width = 800, const int &height = 600, const int &context_ver_maj = 3, const int &context_ver_min = 3);


        // Getters

        /** Get the valid status */
        bool isValid();


        /** Get resolution */
        std::string getTitle() const;

        /** Get resolution */
        glm::vec2 getResolution() const;

        /** Get the background color */
        glm::vec3 getBackgroundColor() const;


        /** Get the camera */
        Camera *getCamera() const;

        /** Get the volume */
        Volume *getVolume() const;

        /** Get the program */
        GLSLProgram *getProgram() const;


        /** Get frames */
        unsigned long long int getFrames() const;


        // Setters

        /** Set title */
        void setTitle(const std::string &new_title);

        /** Set the background color */
        void setBackgroundColor(const glm::vec3 &color);


        // Methods

        /** Render main loop */
        virtual void mainLoop();


        // Destructor

        /** Scene destructor */
        virtual ~Scene();


        // Static getters

        /** Get the OpenGL vendor */
        static const GLubyte *getOpenGLVendor();

        /** Get the OpenGL renderer */
        static const GLubyte *getOpenGLRenderer();

        /** Get the OpenGL version */
        static const GLubyte *getOpenGLVersion();

        /** Get the GLSL version */
        static const GLubyte *getGLSLVersion();
};

#endif // __SCENE_HPP_
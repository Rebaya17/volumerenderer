#ifndef __INTERACTIVE_SCENE_HPP_
#define __INTERACTIVE_SCENE_HPP_

#include "../scene.hpp"

#include "mouse.hpp"


class InteractiveScene : public Scene {
    private:
        // Attributes

        /** Mouse */
        Mouse *const mouse;


        // Constructors

        /** Disable the default constructor */
        InteractiveScene() = delete;

        /** Disable the default copy constructor */
        InteractiveScene(const InteractiveScene &) = delete;

        /** Disable the assignation operator */
        InteractiveScene &operator=(const InteractiveScene &) = delete;


        // Methods

        /** Draw the GUI */
        void drawGUI();


        /** Process keyboard input */
        void processKeyboardInput();


        // Static methods

        /** GLFW framebuffer size callback */
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

        /** GLFW mouse button callback */
        static void mouseButtonCallback(GLFWwindow *window, int button, int action, int);

        /** GLFW cursor callback */
        static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);

        /** GLFW scroll callback */
        static void scrollCallback(GLFWwindow *window, double, double yoffset);

        /** GLFW key callback */
        static void keyCallback(GLFWwindow *window, int key, int, int action, int modifier);


    public:
        // Constructor

        /** Interactive scene constructor */
        InteractiveScene(const std::string &title, const int &width = 800, const int &height = 600, const int &context_ver_maj = 3, const int &context_ver_min = 3);


        // Getters

        /** Get the mouse */
        Mouse *getMouse() const;


        // Methods

        /** Render main loop */
        void mainLoop();


        // Destructor

        /** Interactive scene destructor */
        ~InteractiveScene();
};

#endif // __INTERACTIVE_SCENE_HPP_
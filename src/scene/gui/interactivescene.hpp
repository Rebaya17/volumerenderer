#ifndef __INTERACTIVE_SCENE_HPP_
#define __INTERACTIVE_SCENE_HPP_

#include "../scene.hpp"

#include "mouse.hpp"


class InteractiveScene : public Scene {
    private:
        // Enumerations

        /** Focus */
        enum Focus {
            VOLUME,
            FUNCTION,
            PREVIOUS,
            NEXT,
            ALPHA,
            BLUE,
            GREEN,
            RED
        };


        // Attributes

        /** Showing GUI status */
        bool showing_gui;

        /** Focus */
        InteractiveScene::Focus focus;


        /** Two pixels high */
        GLfloat height_2;

        /** Two and half pixels high */
        GLfloat height_2_5;

        /** Five pixels height */
        GLfloat height_5;

        /** Sixty-four pixels height */
        GLfloat height_64;

        /** Two and half pixels wide */
        GLfloat width_2_5;

        /** Five pixels wide */
        GLfloat width_5;

        /** Eight pixels wide */
        GLfloat width_8;

        /** Width scale */
        GLfloat width_scale;


        /** Vertex array object */
        GLuint vao;

        /** Vertex buffer object */
        GLuint vbo;


        /** GUI program */
        GLSLProgram *gui_program;

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

        /** Update the GUI data */
        void updateGUI();

        /** Draw the GUI */
        void drawGUI();


        /** updateFocus */
        void updateFocus(const glm::ivec2 &pos);


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

        /** Get the showing fui status */
        bool isShowingGUI() const;

        /** Get the GUI program */
        GLSLProgram *getGUIProgram() const;

        /** Get the mouse */
        Mouse *getMouse() const;


        // Setters

        /** Set the showing GUI status */
        void showGUI(const bool &status);


        // Methods

        /** Render main loop */
        void mainLoop();


        // Destructor

        /** Interactive scene destructor */
        ~InteractiveScene();
};

#endif // __INTERACTIVE_SCENE_HPP_
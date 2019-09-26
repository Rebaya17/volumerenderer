#include "interactivescene.hpp"

#include <iostream>

// Private statics methods

// GLFW framebuffer size callback
void InteractiveScene::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    // Execute the scene framebuffer size callback
    Scene::framebufferSizeCallback(window, width, height);

    // Resize the mouse resolution
    static_cast<InteractiveScene *>(glfwGetWindowUserPointer(window))->mouse->setResolution(glm::uvec2(static_cast<unsigned int>(width), static_cast<unsigned int>(height)));
}

// GLFW mouse button callback
void InteractiveScene::mouseButtonCallback(GLFWwindow *window, int, int action, int) {

}

// GLFW cursor callback
void InteractiveScene::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {

}

// GLFW scroll callback
void InteractiveScene::scrollCallback(GLFWwindow *window, double, double yoffset) {

}

// GLFW key callback
void InteractiveScene::keyCallback(GLFWwindow *window, int key, int, int action, int modifier) {
    // Get the pressed status
    bool pressed = action != GLFW_RELEASE;

    // Get the interactive scene
    InteractiveScene *const scene = static_cast<InteractiveScene *>(glfwGetWindowUserPointer(window));

    switch (key) {
        // Reload shaders
        case GLFW_KEY_R:
            if (pressed && (modifier == GLFW_MOD_CONTROL)) {
                scene->program->link();
            }
    }
}


// Private methods

// Draw the GUI
void InteractiveScene::drawGUI() {

}

// Process keyboard input
void InteractiveScene::processKeyboardInput() {
    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W))                                           camera->travell(Camera::FRONT);
    if (glfwGetKey(window, GLFW_KEY_S))                                           camera->travell(Camera::BACK);
    if (glfwGetKey(window, GLFW_KEY_A)     || glfwGetKey(window, GLFW_KEY_LEFT))  camera->travell(Camera::LEFT);
    if (glfwGetKey(window, GLFW_KEY_D)     || glfwGetKey(window, GLFW_KEY_RIGHT)) camera->travell(Camera::RIGHT);
    if (glfwGetKey(window, GLFW_KEY_SPACE) || glfwGetKey(window, GLFW_KEY_UP))    camera->travell(Camera::UP);
    if (glfwGetKey(window, GLFW_KEY_C)     || glfwGetKey(window, GLFW_KEY_DOWN))  camera->travell(Camera::DOWN);
}


// Constructor

// Interactive scene constructor
InteractiveScene::InteractiveScene(const std::string &title, const int &width, const int &height, const int &context_ver_maj, const int &context_ver_min) :
    // Scene
    Scene(title, width, height, context_ver_maj, context_ver_min),

    // Mouse
    mouse(new Mouse(width, height)) {
    // Set the user pointer to this scene and setup callbacks
    if (window != nullptr) {
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, InteractiveScene::framebufferSizeCallback);
        glfwSetMouseButtonCallback(window, InteractiveScene::mouseButtonCallback);
        glfwSetCursorPosCallback(window, InteractiveScene::cursorPosCallback);
        glfwSetScrollCallback(window, InteractiveScene::scrollCallback);
        glfwSetKeyCallback(window, InteractiveScene::keyCallback);
    }
}


// Getters

// Get the mouse
Mouse *InteractiveScene::getMouse() const {
    return mouse;
}


// Methods

// Render main loop
void InteractiveScene::mainLoop() {
    // Check the window status
    if (window == nullptr) {
        std::cerr << "error: there is no window" << std::endl;
        return;
    }

    // Check the default geometry pass valid status
    if ((program == nullptr) || !program->isValid()) {
        std::cerr << "warning: the program has not been set or is not valid" << std::endl;
    }

    // The rendering main loop
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        // Draw the scene and GUI
        drawScene();
        drawGUI();

        // Process the keyboard input
        processKeyboardInput();

        // Poll events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);

        // Count frame
        frames++;
    }
}


// Destructor

// Interactive scene destructor
InteractiveScene::~InteractiveScene() {
    // Delete the mouse
    delete mouse;
}
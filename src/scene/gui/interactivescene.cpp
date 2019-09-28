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
void InteractiveScene::mouseButtonCallback(GLFWwindow *window, int button, int action, int) {
    // Get the mouse and set the pressed status
    Mouse *const mouse = static_cast<InteractiveScene *>(glfwGetWindowUserPointer(window))->mouse;

    // Check the enabled status
    if (!mouse->isEnabled()) return;

    // Update and check the pressed status
    const bool pressed = action != GLFW_RELEASE;
    mouse->setPressed(pressed);
    if (!pressed) {
        return;
    }

    // Get the cursor position
    double xpos;
    double ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Update mouse points
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:   mouse->setRotationPoint(xpos, ypos); return;
        case GLFW_MOUSE_BUTTON_RIGHT:
        case GLFW_MOUSE_BUTTON_MIDDLE: mouse->setTranslationPoint(xpos, ypos);
    }
}

// GLFW cursor callback
void InteractiveScene::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    // Get the interactive scene
    InteractiveScene *const scene = static_cast<InteractiveScene *>(glfwGetWindowUserPointer(window));

    // Check the pressed satus
    Mouse *const mouse = scene->mouse;
    if (!mouse->isEnabled() && !mouse->isPressed()) {
        return;
    }

    // Rotate the volume
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        scene->volume->rotate(mouse->rotate(xpos, ypos));
    }

    // Translate the volume
    else if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) | glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) == GLFW_PRESS) {
        scene->volume->translate(glm::vec3(mouse->translate(xpos, ypos), 0.0F));
    }
}

// GLFW scroll callback
void InteractiveScene::scrollCallback(GLFWwindow *window, double, double yoffset) {
    static_cast<InteractiveScene *>(glfwGetWindowUserPointer(window))->camera->zoom(yoffset);
}

// GLFW key callback
void InteractiveScene::keyCallback(GLFWwindow *window, int key, int, int action, int) {
    // Get the pressed status
    const bool pressed = action != GLFW_RELEASE;

    // Get the interactive scene
    InteractiveScene *const scene = static_cast<InteractiveScene *>(glfwGetWindowUserPointer(window));

    // Options
    switch (key) {
        // Toggle the camera perspective
        case GLFW_KEY_P:
            if (pressed) {
                scene->camera->setOrthogonal(!scene->camera->isOrthogonal());
            }
            return;

        // Toggle the camera boost
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            Camera::setBoosted(pressed);
            return;

        // Reload volume
        case GLFW_KEY_F5:
            if (pressed) {
                scene->volume->reload();
            }
            return;

        // Reload program
        case GLFW_KEY_F6:
            if (pressed) {
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
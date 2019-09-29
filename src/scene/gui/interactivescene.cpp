#include "interactivescene.hpp"

#include <iostream>

// Private statics methods

// GLFW framebuffer size callback
void InteractiveScene::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    // Execute the scene framebuffer size callback
    Scene::framebufferSizeCallback(window, width, height);

    // Get the interactive scene
    InteractiveScene *const scene = static_cast<InteractiveScene *>(glfwGetWindowUserPointer(window));

    // Resize the mouse resolution and update the GUI
    scene->mouse->setResolution(glm::uvec2(static_cast<unsigned int>(width), static_cast<unsigned int>(height)));
    scene->updateGUI();
}

// GLFW mouse button callback
void InteractiveScene::mouseButtonCallback(GLFWwindow *window, int button, int action, int) {
    // Get the scene, mouse and set the pressed status
    InteractiveScene *scene = static_cast<InteractiveScene *>(glfwGetWindowUserPointer(window));
    Mouse *const mouse = scene->mouse;

    // Check the enabled status
    if (!mouse->isEnabled()) return;

    // Update and check the pressed status
    const bool pressed = action != GLFW_RELEASE;
    mouse->setPressed(pressed);
    if (!pressed) {
        return;
    }

    // Get the cursor position and update focus
    double xpos;
    double ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    scene->updateFocus(glm::ivec2(xpos, ypos));

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
        // Toggle the showing GUI status
        case GLFW_KEY_I:
            if (pressed) {
                scene->showing_gui = !scene->showing_gui;
            }
            return;

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
                scene->gui_program->link();
            }
    }
}


// Private methods

void InteractiveScene::updateGUI() {
    // Height proportions
    height_2   =   4.0F / static_cast<GLfloat>(height);
    height_2_5 =   5.0F / static_cast<GLfloat>(height);
    height_5   =  10.0F / static_cast<GLfloat>(height);
    height_64  = 128.0F / static_cast<GLfloat>(height);

    // Width proportions
    width_2_5   =  5.0F / static_cast<GLfloat>(width);
    width_5     = 10.0F / static_cast<GLfloat>(width);
    width_8     = 16.0F / static_cast<GLfloat>(width);
    width_scale = (static_cast<GLfloat>(width) - 26.0F) / static_cast<GLfloat>(width);

    // GUI data
    const GLfloat gui[] = {
        // Vertices             // Texture coordinate

        // Transfer function and channels
        -1.0F, -1.0F,                   0.0F,
         1.0F, -1.0F,                   1.0F,
        -1.0F, -1.0F + height_5,        0.0F,

        -1.0F, -1.0F + height_5,        0.0F,
         1.0F, -1.0F,                   1.0F,
         1.0F, -1.0F + height_5,        1.0F,

         // Graph
        -1.0F, -1.0F,                   1.0F,
         1.0F, -1.0F,                   1.0F,
        -1.0F, -1.0F + height_64,       1.0F,

        -1.0F, -1.0F + height_64,       1.0F,
         1.0F, -1.0F,                   1.0F,
         1.0F, -1.0F + height_64,       1.0F,

        // Down arrow
        -1.0F,             -1.0F,               1.0F,
        -1.0F + width_2_5, -1.0F + height_5,    1.0F,
        -1.0F - width_2_5, -1.0F + height_5,    1.0F,

        // Left arrow
        -1.0F,           -1.0F + height_2_5,    1.0F,
        -1.0F + width_5, -1.0F,                 1.0F,
        -1.0F + width_5, -1.0F + height_5,      1.0F,

        // Right arrow
        -1.0F,           -1.0F,                 1.0F,
        -1.0F + width_5, -1.0F + height_2_5,    1.0F,
        -1.0F,           -1.0F + height_5,      1.0F,
    };

    // Bind the vertex buffer object
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Update the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, 252, gui);

    // Unbind the vertex array object
    glBindVertexArray(GL_FALSE);
}

// Draw the GUI
void InteractiveScene::drawGUI() {
    // Check the showing GUI status
    if (!showing_gui) {
        return;
    }

    // Disable the depth test and blend
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    // Bind the transfer function
    volume->getTransferFunction()->bind(gui_program);

    // Use the program
    gui_program->use();

    // Bind the vertex array object
    glBindVertexArray(vao);

    // Draw the transfer function
    float height = height_5;

    gui_program->setUniform("u_scale", glm::vec2(width_scale, 1.0F));
    gui_program->setUniform("u_shape", 1);
    gui_program->setUniform("u_pos", glm::vec2(0.0F, height));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw the graphic
    height += height_5;
    gui_program->setUniform("u_shape", 0);
    gui_program->setUniform("u_color", glm::vec3(0.75F));
    gui_program->setUniform("u_pos", glm::vec2(0.0F, height));
    glDrawArrays(GL_TRIANGLES, 6, 6);

    // Draw the node arrows and selector
    height += height_64;
    gui_program->setUniform("u_scale", glm::vec2(1.0F));
    gui_program->setUniform("u_color", glm::vec3(0.0F));
    gui_program->setUniform("u_pos", glm::vec2(width_5, height));
    glDrawArrays(GL_TRIANGLES, 15, 3);

    gui_program->setUniform("u_pos", glm::vec2(2.0F * (1.0F - width_5), height));
    glDrawArrays(GL_TRIANGLES, 18, 3);

    gui_program->setUniform("u_pos", glm::vec2(width_5 + width_8, height));
    glDrawArrays(GL_TRIANGLES, 12, 3);

    // Prepare channels
    static const glm::vec3 color[] = {glm::vec3(1.0F), glm::vec3(0.0F, 0.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(1.0F, 0.0F, 0.0F)};

    // Draw channels
    for (int i = 0; i < 4; i++) {
        // Bar
        height += height_5 + height_2;
        gui_program->setUniform("u_scale", glm::vec2(width_scale, 1.0F));
        gui_program->setUniform("u_color", color[i]);
        gui_program->setUniform("u_pos", glm::vec2(0.0F, height));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Level
        height += height_5;
        gui_program->setUniform("u_scale", glm::vec2(1.0F));
        gui_program->setUniform("u_color", glm::vec3(0.0F));
        gui_program->setUniform("u_pos", glm::vec2(width_5 + width_8, height));
        glDrawArrays(GL_TRIANGLES, 12, 3);
    }

    // Unbind the vertex array object
    glBindVertexArray(GL_FALSE);
}


// updateFocus
void InteractiveScene::updateFocus(const glm::ivec2 &pos) {
    // Variables
    const int x = pos.x;
    const int y = pos.y;
    const int left = 13;
    const int right = width - 12;
    int bottom = height - 4;
    int top = bottom - 1;

    // Focus on transfer function
    top -= 74;
    if ((x > left) && (x < right) && (y > top) && (y < bottom)) {
        focus = InteractiveScene::FUNCTION;
        return;
    }

    // Focus on previous node selector
    bottom -= 69;
    if ((x > 4) && (x < 10) && (y > top) && (y < bottom)) {
        focus = InteractiveScene::PREVIOUS;
        return;
    }

    // Focus on previous node selector
    if ((x > (width - 10)) && (x < (width - 4)) && (y > top) && (y < bottom)) {
        focus = InteractiveScene::NEXT;
        return;
    }

    // Alpha channel
    bottom -= 7;
    top -= 12;
    if ((x > left) && (x < right) && (y > top) && (y < bottom)) {
        focus = InteractiveScene::ALPHA;
        return;
    }

    // Blue channel
    bottom -= 12;
    top -= 12;
    if ((x > left) && (x < right) && (y > top) && (y < bottom)) {
        focus = InteractiveScene::BLUE;
        return;
    }

    // Green channel
    bottom -= 12;
    top -= 12;
    if ((x > left) && (x < right) && (y > top) && (y < bottom)) {
        focus = InteractiveScene::GREEN;
        return;
    }

    // Red channel
    bottom -= 12;
    top -= 12;
    if ((x > left) && (x < right) && (y > top) && (y < bottom)) {
        focus = InteractiveScene::GREEN;
        return;
    }

    // Volume
    focus = InteractiveScene::VOLUME;
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

    // Showing GUI status
    showing_gui(true),
    focus(InteractiveScene::VOLUME),

    // Buffers
    vao(GL_FALSE),
    vbo(GL_FALSE),

    // GUI program
    gui_program(new GLSLProgram()),

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

    // GUI vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // GUI vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Load the GUI data
    const std::size_t stride = sizeof(GLfloat) * 3;
    glBufferData(GL_ARRAY_BUFFER, 252, nullptr, GL_DYNAMIC_DRAW);

    // Vertex attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, nullptr);

    // Texture attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(sizeof(GLfloat) << 1));

    // Unbind vertex array object
    glBindVertexArray(GL_FALSE);

    // Update the gui data
    updateGUI();
}


// Getters

// Get the showing fui status
bool InteractiveScene::isShowingGUI() const {
    return showing_gui;
}


// Get the GUI program
GLSLProgram *InteractiveScene::getGUIProgram() const {
    return gui_program;
}

// Get the mouse
Mouse *InteractiveScene::getMouse() const {
    return mouse;
}


// Setters

// Set the showing GUI status
void InteractiveScene::showGUI(const bool &status) {
    showing_gui = status;
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
    // Buffers
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vao);

    // GUI program
    delete gui_program;

    // Delete the mouse
    delete mouse;
}
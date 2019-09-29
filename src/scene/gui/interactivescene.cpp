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

    // Process mouse input
    scene->GUIInteraction(static_cast<float>(xpos), button);

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
    if (!mouse->isEnabled() || !mouse->isPressed()) {
        return;
    }

    // Get the pressed button
    int button;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        button = GLFW_MOUSE_BUTTON_RIGHT;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        button = GLFW_MOUSE_BUTTON_LEFT;
    }
    else {
        return;
    }

    // Interaction with the GUI or volume
    switch(scene->focus) {
        // Volume interaction
        case VOLUME:
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:  scene->volume->rotate(mouse->rotate(xpos, ypos)); return;
                case GLFW_MOUSE_BUTTON_RIGHT: scene->volume->translate(glm::vec3(mouse->translate(xpos, ypos), 0.0F)); return;
            }
            return;

        // GUI
        default:
            scene->GUIInteraction(static_cast<float>(xpos), button);
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
                scene->updateTransferFunction();
            }
            return;

        // Reload program
        case GLFW_KEY_F6:
            if (pressed) {
                scene->program->link();
                scene->program_gui->link();
                scene->program_func->link();
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
    width_13    = 26.0F / static_cast<GLfloat>(width);
    width_gui   = static_cast<GLfloat>(width - 26);
    width_scale = (width_gui) / static_cast<GLfloat>(width);

    // GUI data
    const GLfloat margin_x = 1.0F - width_13;
    const GLfloat gui[] = {
        // Vertices                             // Texture coordinate

        // Transfer function and channels
        -margin_x, -1.0F,                       0.0F,
         margin_x, -1.0F,                       1.0F,
        -margin_x, -1.0F + height_5,            0.0F,

        -margin_x, -1.0F + height_5,            0.0F,
         margin_x, -1.0F,                       1.0F,
         margin_x, -1.0F + height_5,            1.0F,

         // Graph
        -margin_x, -1.0F,                       1.0F,
         margin_x, -1.0F,                       1.0F,
        -margin_x, -1.0F + height_64,           1.0F,

        -margin_x, -1.0F + height_64,           1.0F,
         margin_x, -1.0F,                       1.0F,
         margin_x, -1.0F + height_64,           1.0F,

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

    // Bind the GUI vertex buffer object
    glBindVertexArray(vao_gui);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_gui);

    // Update the GUI data
    glBufferSubData(GL_ARRAY_BUFFER, 0, 252, gui);

    // Unbind the GUI vertex array object
    glBindVertexArray(GL_FALSE);


    // Update the transfer function
    updateTransferFunction();
}

// Update the transfer function data
void InteractiveScene::updateTransferFunction() const {
    // Transfer function data
    GLfloat func[2048];
    const GLubyte *source = volume->getTransferFunction()->getData();
    const GLfloat bottom = 2.0F * height_5;
    const GLfloat height_scale = 64.0F / static_cast<GLfloat>(height);
    for (unsigned int i = 0, j = 0; i < 1024; i += 4, j += 2) {
        // Horizontal position
        const GLfloat x = width_13 + (static_cast<GLfloat>(j >> 1) * width_scale / 127.5F - 1.0F);

        // Red
        func[j    ] = x;
        func[j + 1] = bottom + (static_cast<GLfloat>(source[i]) * height_scale / 127.5F - 1.0F);

        // Green
        func[j + 512] = x;
        func[j + 513] = bottom + (static_cast<GLfloat>(source[i + 1]) * height_scale / 127.5F - 1.0F);

        // Blue
        func[j + 1024] = x;
        func[j + 1025] = bottom + (static_cast<GLfloat>(source[i + 2]) * height_scale / 127.5F - 1.0F);

        // Alpha
        func[j + 1536] = x;
        func[j + 1537] = bottom + (static_cast<GLfloat>(source[i + 3]) * height_scale / 127.5F - 1.0F);
    }

    // Bind the transfer function vertex buffer object
    glBindVertexArray(vao_func);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_func);

    // Update the transfer function data
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8192, func);

    // Unbind the transfer function vertex array object
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
    TransferFunction *const trans_func = volume->getTransferFunction();
    trans_func->bind(program_gui);

    // Use the GUI program
    program_gui->use();

    // Bind the vertex array object
    glBindVertexArray(vao_gui);

    // Draw the transfer function
    float height = height_5;

    program_gui->setUniform("u_shape", 1);
    program_gui->setUniform("u_pos", glm::vec2(0.0F, height));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw the graphic
    height += height_5;
    program_gui->setUniform("u_shape", 0);
    program_gui->setUniform("u_color", glm::vec3(0.75F));
    program_gui->setUniform("u_pos", glm::vec2(0.0F, height));
    glDrawArrays(GL_TRIANGLES, 6, 6);

    // Draw the node arrows
    height += height_64;
    program_gui->setUniform("u_color", glm::vec3(0.0F));
    program_gui->setUniform("u_pos", glm::vec2(width_5, height));
    glDrawArrays(GL_TRIANGLES, 15, 3);

    program_gui->setUniform("u_pos", glm::vec2(2.0F * (1.0F - width_5), height));
    glDrawArrays(GL_TRIANGLES, 18, 3);

    // Draw the current node
    program_gui->setUniform("u_pos", glm::vec2(width_13 + static_cast<float>(trans_func->getCurrentNodeIndex()) / 127.5F * width_scale, height));
    glDrawArrays(GL_TRIANGLES, 12, 3);

    // Prepare channels
    static const glm::vec3 color[] = {glm::vec3(1.0F), glm::vec3(0.0F, 0.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(1.0F, 0.0F, 0.0F)};
    const glm::vec4 level = glm::vec4(trans_func->getCurrentNode()) / 127.5F * width_scale;

    // Draw channels
    for (int i = 0; i < 4; i++) {
        // Bar
        height += height_5 + height_2;
        program_gui->setUniform("u_color", color[i]);
        program_gui->setUniform("u_pos", glm::vec2(0.0F, height));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Level
        height += height_5;
        program_gui->setUniform("u_color", glm::vec3(0.0F));
        program_gui->setUniform("u_pos", glm::vec2(width_13 + level[3 - i], height));
        glDrawArrays(GL_TRIANGLES, 12, 3);
    }

    // Unbind the GUI vertex array object
    glBindVertexArray(GL_FALSE);


    // Use the transfer function program
    program_func->use();

    // Bind the transfer function vertex array object
    glBindVertexArray(vao_func);

    // Alpha channel
    program_func->setUniform("u_color", glm::vec3(0.0F));
    glDrawArrays(GL_LINE_STRIP, 768, 256);

    // Blue channel
    program_func->setUniform("u_color", glm::vec3(0.0F, 0.0F, 1.0F));
    glDrawArrays(GL_LINE_STRIP, 512, 256);

    // Green channel
    program_func->setUniform("u_color", glm::vec3(0.0F, 1.0F, 0.0F));
    glDrawArrays(GL_LINE_STRIP, 256, 256);

    // Red channel
    program_func->setUniform("u_color", glm::vec3(1.0F, 0.0F, 0.0F));
    glDrawArrays(GL_LINE_STRIP, 0, 256);

    // Unbind the GUI vertex array object
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
    top -= 76;
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
        focus = InteractiveScene::RED;
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

// Process mouse input
void InteractiveScene::GUIInteraction(const float &xpos, const int &button) {
    // Calculate the node or channel level
    const GLubyte selected = static_cast<GLubyte>(255.0F * static_cast<float>(xpos - 13.0F) / width_gui); 

    // Update GUI
    glm::vec4 color;
    switch (focus) {
        // Nothing if the focus is in the volume
        case VOLUME: return;

        // Select the previous node
        case PREVIOUS:
            volume->getTransferFunction()->selectPreviousNode();
            return;

        // Select the next node
        case NEXT:
            volume->getTransferFunction()->selectNextNode();
            return;

        // Select or remove node
        case FUNCTION:
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT: volume->getTransferFunction()->setCurrentNodeIndex(selected); break;
                case GLFW_MOUSE_BUTTON_RIGHT: volume->getTransferFunction()->removeCurrentNode(); break;
            }
            updateTransferFunction();
            return;

        // Select the alpha level
        case ALPHA:
            color = volume->getTransferFunction()->getCurrentNode();
            color.a = selected;
            break;

        // Select the alpha level
        case BLUE:
            color = volume->getTransferFunction()->getCurrentNode();
            color.b = selected;
            break;

        // Select the alpha level
        case GREEN:
            color = volume->getTransferFunction()->getCurrentNode();
            color.g = selected;
            break;

        // Select the alpha level
        case RED:
            color = volume->getTransferFunction()->getCurrentNode();
            color.r = selected;
            break;
    }

    // Update the transfer function
    volume->getTransferFunction()->setCurrentNode(color);
    updateTransferFunction();
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
    vao_gui(GL_FALSE),
    vao_func(GL_FALSE),
    vbo_gui(GL_FALSE),
    vbo_func(GL_FALSE),

    // GLSL programs
    program_gui(new GLSLProgram()),
    program_func(new GLSLProgram()),

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
    glGenVertexArrays(1, &vao_gui);
    glBindVertexArray(vao_gui);

    // GUI vertex buffer object
    glGenBuffers(1, &vbo_gui);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_gui);

    // Initialize the GUI data buffer
    std::size_t stride = sizeof(GLfloat) * 3;
    glBufferData(GL_ARRAY_BUFFER, 252, nullptr, GL_DYNAMIC_DRAW);

    // Vertex attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, nullptr);

    // Texture attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(sizeof(GLfloat) << 1));

    // Unbind vertex array object
    glBindVertexArray(GL_FALSE);


    // Transfer function unction vertex array object
    glGenVertexArrays(1, &vao_func);
    glBindVertexArray(vao_func);

    // Transfer function vertex buffer object
    glGenBuffers(1, &vbo_func);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_func);

    // Initialize the transfer function data buffer
    stride = sizeof(GLfloat) << 1;
    glBufferData(GL_ARRAY_BUFFER, 8192, nullptr, GL_DYNAMIC_DRAW);

    // Vertex attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, nullptr);


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
    return program_gui;
}

// Get the transfer function program
GLSLProgram *InteractiveScene::getTransferFunctionProgram() const {
    return program_func;
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
    glDeleteBuffers(1, &vbo_gui);
    glDeleteBuffers(1, &vbo_func);

    // Bertex arrays
    glDeleteVertexArrays(1, &vao_gui);
    glDeleteVertexArrays(1, &vao_func);

    // Programs
    delete program_gui;
    delete program_func;

    // Delete the mouse
    delete mouse;
}
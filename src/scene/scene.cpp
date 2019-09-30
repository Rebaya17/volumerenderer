#include "scene.hpp"

#include <iostream>


// Private static attributes

// Instances counter
std::size_t Scene::instances = 0U;

// Glad loaded flag
bool Scene::initialized_glad = false;


// Screen width
GLsizei Scene::screen_width = 0U;

// Screen
GLsizei Scene::screen_height = 0U;


// OpenGL vendor
const GLubyte *Scene::opengl_vendor = nullptr;

// OpenGL renderer
const GLubyte *Scene::opengl_renderer = nullptr;

// OpenGL version
const GLubyte *Scene::opengl_version = nullptr;

// GLSL version
const GLubyte *Scene::glsl_version = nullptr;


// Private static methods

// GLFW error callback
void Scene::errorCallback(int error, const char *description) {
    std::cerr << "error " << error << ": " << description << std::endl;
}

// GLFW framebuffer size callback
void Scene::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    // Resize viewport
    glViewport(0, 0, width, height);

    // Get the scene and update the window resolution
    Scene *const scene = static_cast<Scene *>(glfwGetWindowUserPointer(window));
    scene->width = width;
    scene->height = height;

    // Update the camera resolution
    scene->camera->setResolution(glm::uvec2(static_cast<unsigned int>(width), static_cast<unsigned int>(height)));
}


// Private methods

// Draw the scene
void Scene::drawScene() {
    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable the depth test and blend
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // Check the volume
    if (volume->isOpen()) {
        camera->bind(program);
        volume->draw(program);
    }
}


// Constructor

// Scene constructor
Scene::Scene(const std::string &title, const int &width, const int &height, const int &context_ver_maj, const int &context_ver_min) :
    // Defaults for window
    window(nullptr),
    title(title),
    width(width),
    height(height),

    // Clear color
    background_color(0.0F),

    // Camera
    camera(nullptr),

    // Volume
    volume(nullptr),

    // Program
    program(nullptr) {
    // Create window flag
    bool create_window = true;

    // Initialize GLFW if there are no instances
    if (Scene::instances == 0U) {
        // Setup error callback
        glfwSetErrorCallback(Scene::errorCallback);

        // Initialize the library
        if (glfwInit() != GLFW_TRUE) {
            std::cerr << "error: cannot initialize GLFW" << std::endl;
            create_window = false;
        }
    }

    // Create window
    if (create_window) {
        // Set the window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_ver_maj);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_ver_min);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create the scene window
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    }

    // Check the window creation
    if (window == nullptr) {
        std::cerr << "error: cannot create the application window" << std::endl;
    }

    // Setup the context
    else {
        // Set the user pointer to this scene and setup the current context
        glfwSetWindowUserPointer(window, this);
        glfwMakeContextCurrent(window);

        // Initialize Glad and check errors
        if (!Scene::initialized_glad && (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)) {
            std::cerr << "error: cannot initialize glad" << std::endl;

            // Destroy window
            glfwDestroyWindow(window);
            window = nullptr;
        }

        // Finalize conext setup
        else {
            // Set true the initialized Glad flag
            Scene::initialized_glad = true;

            // Set the OpenGL strings
            Scene::opengl_vendor   = glGetString(GL_VENDOR);
            Scene::opengl_renderer = glGetString(GL_RENDERER);
            Scene::opengl_version  = glGetString(GL_VERSION);
            Scene::glsl_version    = glGetString(GL_SHADING_LANGUAGE_VERSION);

            // Create the default objects
            camera = new Camera(width, height);
            volume = new Volume();
            program = new GLSLProgram();

            // Set the resize window callback and maximize window
            glfwSetFramebufferSizeCallback(window, Scene::framebufferSizeCallback);
            glfwMaximizeWindow(window);

            // Setup the swap interval
            glfwSwapInterval(1);

            // Set the blend function
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }

    // Count instance
    Scene::instances++;
}


// Getters

// Get the valid status
bool Scene::isValid() {
    return window != nullptr;
}


// Get title
std::string Scene::getTitle() const {
    return title;
}

// Get resolution
glm::vec2 Scene::getResolution() const {
    return glm::vec2(width, height);
}

// Get the background color
glm::vec3 Scene::getBackgroundColor() const {
    return background_color;
}


// Get the camera
Camera *Scene::getCamera() const {
    return camera;
}

// Get the volume
Volume *Scene::getVolume() const {
    return volume;
}

// Get program by ID
GLSLProgram *Scene::getProgram() const {
    return program;
}


// Get frames
unsigned long long int Scene::getFrames() const {
    return frames;
}


// Setters

// Set title
void Scene::setTitle(const std::string &new_title) {
    title = new_title;
    glfwSetWindowTitle(window, title.c_str());
}

// Set the background color
void Scene::setBackgroundColor(const glm::vec3 &color) {
    background_color = color;
    glClearColor(background_color.r, background_color.g, background_color.b, 1.0F);
}


// Methods

// Render main loop
void Scene::mainLoop() {
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
        // Draw the scene
        drawScene();

        // Poll events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);

        // Count frame
        frames++;
    }
}


// Destructor

// Scene destructor
Scene::~Scene() {
    // Delete the camera
    if (camera != nullptr) {
        delete camera;
    }

    // Delete the volume
    if (volume != nullptr) {
        delete volume;
    }

    // Delete the program
    if (program != nullptr) {
        delete program;
    }


    // Destroy window
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }

    // If is the last instance
    if ((Scene::instances == 1U) && Scene::initialized_glad) {
        // Terminate GLFW
        glfwTerminate();

        // Reset initialized Glad flag
        Scene::initialized_glad = false;

        // Reset OpenGL strings
        Scene::opengl_vendor   = nullptr;
        Scene::opengl_renderer = nullptr;
        Scene::opengl_version  = nullptr;
        Scene::glsl_version    = nullptr;
    }

    // Discount instance
    Scene::instances--;
}


// Static getters

// Get the OpenGL vendor
const GLubyte *Scene::getOpenGLVendor() {
    return Scene::opengl_vendor;
}

// Get the OpenGL renderer
const GLubyte *Scene::getOpenGLRenderer() {
    return Scene::opengl_renderer;
}

// Get the OpenGL version
const GLubyte *Scene::getOpenGLVersion() {
    return Scene::opengl_version;
}

// Get the GLSL version
const GLubyte *Scene::getGLSLVersion() {
    return Scene::glsl_version;
}
#include "scene/gui/interactivescene.hpp"

#include <iostream>

/** Main function */
int main (int argc, char **argv) {
    // Print arguments
    for (int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }

    // Create the scene and check it
    InteractiveScene *scene = new InteractiveScene("OBJViewer");

    // Exit with error if the scene is not valid
    if (!scene->isValid()) {
        delete scene;
        return 1;
    }


    // Esecute the main loop
    scene->mainLoop();


    // Clean up and normal exit
    delete scene;
    return 0;
}
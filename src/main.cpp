#include "scene/gui/interactivescene.hpp"
#include "volume/loader/volumedata.hpp"

#include "dirsep.h"

#include <iostream>


/** Main function */
int main (int argc, char **argv) {
    // Print arguments
    for (int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }

    // Create the scene and check it
    InteractiveScene *scene = new InteractiveScene("VolumeRenderer");

    // Exit with error if the scene is not valid
    if (!scene->isValid()) {
        delete scene;
        return 1;
    }


    // Set the background color and camera
    scene->setBackgroundColor(glm::vec3(0.45F, 0.55F, 0.60F));

    // Setup directories
    const std::string bin_path = argv[0];
    const std::string relative = bin_path.substr(0U, bin_path.find_last_of(DIR_SEP) + 1U);

    const std::string volume_path = relative + ".." + DIR_SEP + "volume" + DIR_SEP;
    const std::string shader_path = relative + ".." + DIR_SEP + "shader" + DIR_SEP;

    // Setup the interactive scene programs
    scene->getGUIProgram()->link(shader_path + "gui.vert.glsl", shader_path + "gui.frag.glsl");
    scene->getTransferFunctionProgram()->link(shader_path + "func.vert.glsl", shader_path + "func.frag.glsl");

    // Set the program and volume
    scene->getProgram()->link(shader_path + "vap.vert.glsl", shader_path + "vap.frag.glsl");
    scene->getVolume()->setPath(volume_path + "carp.dat", VolumeData::RAW16, 256, 256, 512);


    // Esecute the main loop
    scene->mainLoop();


    // Clean up and normal exit
    delete scene;
    return 0;
}
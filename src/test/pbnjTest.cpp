#include "Camera.h"
#include "Configuration.h"
#include "DataFile.h"
#include "Renderer.h"
#include "Volume.h"

#include "pbnj.h"

#include <iostream>

int main(int argc, const char **argv) {
    std::cout << "Hello world!" << std::endl;

    pbnj::Configuration *config = new pbnj::Configuration(
            "../configs/turbulence.json");

    std::cout << "CONFIG FILE: " << "dataFilename " << config->dataFilename << std::endl;
    std::cout << "CONFIG FILE: " << "x " << config->dataXDim << std::endl;
    std::cout << "CONFIG FILE: " << "y " << config->dataYDim << std::endl;
    std::cout << "CONFIG FILE: " << "z " << config->dataZDim << std::endl;
    std::cout << "CONFIG FILE: " << "width " << config->imageWidth << std::endl;
    std::cout << "CONFIG FILE: " << "height " << config->imageHeight << std::endl;
    std::cout << "CONFIG FILE: " << "imageFilename " << config->imageFilename << std::endl;

    // initialization has to go after configuration!
    // otherwise it causes a segfault in OSPRay
    pbnj::pbnjInit(&argc, argv);

    std::string filename(
    "/export/share/data/turbulence/tacc-turbulence-256-volume/tacc-turbulence-256-volume.raw");

    pbnj::Volume *volume = new pbnj::Volume(filename, 256, 256, 256);

    pbnj::Camera *camera = new pbnj::Camera(800, 800);
    camera->setPosition(127.5, 127.5, 512.0);

    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setVolume(volume);
    renderer->setCamera(camera);

    renderer->renderImage("/export/share/data/turbulence/test.ppm");

    return 0;
}

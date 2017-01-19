#include "Camera.h"
#include "Configuration.h"
#include "DataFile.h"
#include "Renderer.h"
#include "TransferFunction.h"
#include "Volume.h"

#include "pbnj.h"

#include <iostream>

int main(int argc, const char **argv) {
    std::cout << "Hello world!" << std::endl;

#if 1

    pbnj::Configuration *config = new pbnj::Configuration(
            "../configs/magnetic.json");

    // initialization has to go after configuration!
    // otherwise it causes a segfault in OSPRay
    pbnj::pbnjInit(&argc, argv);

    pbnj::Volume *volume = new pbnj::Volume(config->dataFilename,
            config->dataXDim, config->dataYDim, config->dataZDim);
    volume->attenuateOpacity(0.1);
    volume->setColorMap(pbnj::spectralReverse);

    pbnj::Camera *camera = new pbnj::Camera(config->imageWidth, 
            config->imageHeight);
    camera->setPosition((config->dataXDim-1)/2.0, (config->dataYDim-1)/2.0,
            config->dataZDim*2);

    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setVolume(volume);
    renderer->setCamera(camera);
    renderer->renderImage(config->imageFilename);

#else

    pbnj::pbnjInit(&argc, argv);

    std::string basename("/export/share/data/turbulence/");
    std::string filename( basename + 
            "tacc-turbulence-256-volume/tacc-turbulence-256-volume.raw");
    std::string imageFilename(basename + "turbulence_hardcode.ppm");

    pbnj::Volume *volume = new pbnj::Volume(filename, 256, 256, 256);
    pbnj::Camera *camera = new pbnj::Camera(800, 800);
    camera->setPosition(127.5, 127.5, 512.0);
    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setVolume(volume);
    renderer->setCamera(camera);
    renderer->renderImage(imageFilename);

#endif

    return 0;
}

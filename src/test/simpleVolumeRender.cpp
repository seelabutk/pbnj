#include "pbnj.h"
#include "Camera.h"
#include "Configuration.h"
#include "Renderer.h"
#include "TransferFunction.h"
#include "Volume.h"

#include <iostream>

int main(int argc, const char **argv)
{
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.json>" << std::endl;
        return 1;
    }

    pbnj::Configuration *config = new pbnj::Configuration(argv[1]);

    pbnj::pbnjInit(&argc, argv);

    pbnj::Volume *volume = new pbnj::Volume(config->dataFilename,
            config->dataXDim, config->dataYDim, config->dataZDim);
    volume->setColorMap(config->colorMap);
    volume->attenuateOpacity(config->opacityAttenuation);

    pbnj::Camera *camera = new pbnj::Camera(config->imageWidth, 
            config->imageHeight);
    camera->centerView(volume);
    /*
    camera->setPosition((config->dataXDim-1)/2.0, (config->dataYDim-1)/2.0,
            config->dataZDim*2);
    */
    camera->setPosition((config->dataXDim-1)/2.0, -(config->dataYDim-1)/2.0 - 1, config->dataZDim);

    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setVolume(volume);
    renderer->setCamera(camera);
    renderer->renderImage(config->imageFilename);

    std::cout << "Rendered image to " << config->imageFilename << std::endl;

    return 0;
}

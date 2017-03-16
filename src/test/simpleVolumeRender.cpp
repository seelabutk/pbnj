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

    if(config->dataFilename.empty()) {
        if(!config->globbedFilenames.empty()) {
            std::cerr << "This example does not support time series data";
            std::cerr << std::endl;
            std::cerr << "But here are the filenames matched:" << std::endl;
            for(std::string fname : config->globbedFilenames)
                std::cerr << fname << std::endl;
            return 1;
        }
        else {
            std::cerr << "No filename given!" << std::endl;
            return 1;
        }
    }
    else {
        std::cout << "DEBUG: dataFilename is not empty!" << std::endl;
        std::cout << "DEBUG: " << config->dataFilename << std::endl;
    }

    pbnj::Volume *volume;
    if(config->dataVariable.empty()) {
        volume = new pbnj::Volume(config->dataFilename, config->dataXDim,
                config->dataYDim, config->dataZDim);
    }
    else {
        volume = new pbnj::Volume(config->dataFilename, config->dataVariable,
                config->dataXDim, config->dataYDim, config->dataZDim);
    }
    volume->setColorMap(config->colorMap);
    volume->setOpacityMap(config->opacityMap);
    volume->attenuateOpacity(config->opacityAttenuation);

    pbnj::Camera *camera = new pbnj::Camera(config->imageWidth, 
            config->imageHeight);
    camera->setPosition(config->cameraX, config->cameraY, config->cameraZ);
    camera->setUpVector(config->cameraUpX, config->cameraUpY, config->cameraUpZ);

    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setVolume(volume);
    renderer->setCamera(camera);
    renderer->setSamples(config->samples);
    renderer->renderImage(config->imageFilename);

    std::cout << "Rendered image to " << config->imageFilename << std::endl;

    return 0;
}

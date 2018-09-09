#include "pbnj.h"
#include "Camera.h"
#include "ConfigReader.h"
#include "Configuration.h"
#include "Renderer.h"
#include "Streamlines.h"

#include <iostream>
#include <string>

int main(int argc, const char **argv)
{
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <configuration file>";
        std::cerr << std::endl;
        return 1;
    }

    pbnj::ConfigReader *reader = new pbnj::ConfigReader();
    rapidjson::Document json;
    reader->parseConfigFile(argv[1], json);
    pbnj::Configuration *config = new pbnj::Configuration(json);

    pbnj::pbnjInit(&argc, argv);
    pbnj::Streamlines *sl = new pbnj::Streamlines(config->dataFilename);

    pbnj::Camera *camera = new pbnj::Camera(config->imageWidth,
            config->imageHeight);
    camera->setPosition(config->cameraX, config->cameraY, config->cameraZ);
    camera->setUpVector(config->cameraUpX, config->cameraUpY,
            config->cameraUpZ);
    //camera->centerView();
    camera->setView(-config->cameraX, -config->cameraY, -50);

    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setBackgroundColor(config->bgColor);
    renderer->setCamera(camera);
    renderer->addSubject(sl, 0.8);
    renderer->setSamples(config->samples);

    renderer->renderImage(config->imageFilename);

    return 0;
}

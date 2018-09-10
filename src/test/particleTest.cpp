#include "pbnj.h"

#include "Camera.h"
#include "ConfigReader.h"
#include "Configuration.h"
#include "Particles.h"
#include "Renderer.h"

int main(int argc, const char **argv)
{
    pbnj::ConfigReader *reader = new pbnj::ConfigReader();
    rapidjson::Document json;
    reader->parseConfigFile(argv[1], json);
    pbnj::Configuration *config = new pbnj::Configuration(json);

    pbnj::pbnjInit(&argc, argv);
    pbnj::Particles *particles = new pbnj::Particles(config->dataFilename, true);

    pbnj::Camera *camera = new pbnj::Camera(config->imageWidth,
            config->imageHeight);
    camera->setPosition(config->cameraX, config->cameraY, config->cameraZ);
    camera->setUpVector(0, 1, 0);
    camera->centerView();

    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setBackgroundColor(config->bgColor);
    renderer->addSubject(particles);
    renderer->setCamera(camera);
    renderer->setSamples(config->samples);

    renderer->renderImage(config->imageFilename);
    std::cout << "Rendered image to " << config->imageFilename << std::endl;

    return 0;
}

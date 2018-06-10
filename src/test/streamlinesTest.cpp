#include "pbnj.h"
#include "Camera.h"
#include "Renderer.h"
#include "Streamlines.h"

#include <iostream>
#include <string>

int main(int argc, const char **argv)
{
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <xml filename>" << std::endl;
        return 1;
    }

    pbnj::pbnjInit(&argc, argv);
    pbnj::Streamlines *sl = new pbnj::Streamlines(std::string(argv[1]));

    pbnj::Camera *camera = new pbnj::Camera(1024, 1024);
    camera->setPosition(0.75, 0.1, 0.75);
    camera->setUpVector(0, 1, 0);
    camera->centerView();

    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setCamera(camera);
    renderer->setStreamlines(sl);
    renderer->setSamples(8);

    renderer->renderImage("/home/ahota/projects/pbnj/renders/streamlines.png");

    return 0;
}

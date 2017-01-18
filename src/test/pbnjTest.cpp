#include "Camera.h"
#include "DataFile.h"
#include "Renderer.h"
#include "Volume.h"

#include "pbnj.h"

#include <iostream>

int main(int argc, const char **argv) {
    std::cout << "Hello world!" << std::endl;

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

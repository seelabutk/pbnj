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
    "/export/share/data/magnetic/magnetic-512-volume/magnetic-512-volume.raw");

    pbnj::DataFile *datafile = new pbnj::DataFile(512, 512, 512);
    datafile->loadFromFile(filename);

    datafile->printStatistics();
    datafile->calculateStatistics();
    datafile->printStatistics();

    pbnj::Volume *volume = new pbnj::Volume(datafile);

    pbnj::Camera *camera = new pbnj::Camera(800, 800);
    camera->setPosition(255.5, 255.5, 1000.0);

    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setVolume(volume);
    renderer->setCamera(camera);

    renderer->renderImage("/export/share/data/magnetic/test.ppm");

    return 0;
}

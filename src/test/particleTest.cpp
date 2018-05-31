#include "pbnj.h"

#include "Camera.h"
#include "Particles.h"
#include "Renderer.h"

int main(int argc, const char **argv)
{
    pbnj::pbnjInit(&argc, argv);
    //pbnj::ParticleDataFile *pdf = new pbnj::ParticleDataFile();
    //pdf->loadFromFile("/home/ahota/projects/pbnj/configs/test.xyz");
    std::string filename = "/home/ahota/projects/pbnj/configs/pyridine.xyz";
    pbnj::Particles *particles = new pbnj::Particles(filename);

    pbnj::Camera *camera = new pbnj::Camera(512, 512);
    camera->setPosition(8, 0, 4);
    camera->setUpVector(0, 1, 0);
    camera->centerView();

    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setBackgroundColor(0, 100, 100, 255);
    renderer->setParticles(particles);
    renderer->setCamera(camera);

    renderer->renderImage("/home/ahota/projects/pbnj/renders/particles.png");

    return 0;
}

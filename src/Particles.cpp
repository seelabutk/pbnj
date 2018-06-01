#include "Particles.h"
#include "ParticleDataFile.h"

#include <ospray/ospray.h>

namespace pbnj {

Particles::Particles(std::string filename)
{
    this->ID = createID();
    this->dataFile = new ParticleDataFile();
    this->dataFile->loadFromFile(filename);

    float color[] = {0.8, 0.8, 0.7};
    // should this iterator over particleData?
    for(std::string particleType : this->dataFile->particleTypes) {
        OSPGeometry spheres = ospNewGeometry("spheres");
        float *pData = this->dataFile->getParticleData(particleType);
        OSPData sphereDataArray = ospNewData(
                this->dataFile->getNumParticles(particleType)*3,
                OSP_FLOAT, pData);
        ospSetData(spheres, "spheres", sphereDataArray);
        ospSet1i(spheres, "bytes_per_sphere", 12);
        ospSet3fv(spheres, "color", color);
        ospSet1f(spheres, "radius", 0.5);
        ospCommit(spheres);

        this->sphereSets.push_back(spheres);
    }
}

Particles::~Particles()
{
    for(OSPGeometry spheres : this->sphereSets) {
        ospRemoveParam(spheres, "spheres");
        ospRemoveParam(spheres, "bytes_per_sphere");
        ospRemoveParam(spheres, "color");
        ospRemoveParam(spheres, "radius");
        ospRelease(spheres);
    }
}

std::vector<OSPGeometry> Particles::asOSPRayObject()
{
    return this->sphereSets;
}

}

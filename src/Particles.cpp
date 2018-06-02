#include "Particles.h"
#include "ParticleDataFile.h"

#include <iostream>

#include <ospray/ospray.h>

namespace pbnj {

Particles::Particles(std::string filename)
{
    this->ID = createID();
    this->dataFile = new ParticleDataFile();
    this->dataFile->loadFromFile(filename);

    this->oSpheres = ospNewGeometry("spheres");
    OSPData sphereDataArray = ospNewData(this->dataFile->numParticles,
            OSP_FLOAT3, this->dataFile->particleData);
    OSPData sphereColorDataArray = ospNewData(this->dataFile->numParticles,
            OSP_FLOAT3, this->dataFile->particleColorData);

    ospSet1i(this->oSpheres, "bytes_per_sphere", 12);
    ospSetData(this->oSpheres, "spheres", sphereDataArray);
    ospSetData(this->oSpheres, "color", sphereColorDataArray);
    ospSet1f(this->oSpheres, "radius", 1.0);
    ospCommit(this->oSpheres);
}

Particles::~Particles()
{
    ospRemoveParam(this->oSpheres, "spheres");
    ospRemoveParam(this->oSpheres, "bytes_per_sphere");
    ospRemoveParam(this->oSpheres, "color");
    ospRemoveParam(this->oSpheres, "radius");
    ospRelease(this->oSpheres);
}

OSPGeometry Particles::asOSPRayObject()
{
    return this->oSpheres;
}

}

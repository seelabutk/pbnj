#include "Particles.h"
#include "ParticleDataFile.h"

#include <iostream>
#include <string>

#include <ospray/ospray.h>

namespace pbnj {

Particles::Particles(std::string filename, bool center, bool autocolor, float radius)
{
    this->dataFile = new ParticleDataFile();
    this->dataFile->loadFromFile(filename, center, autocolor);

    this->oSpheres = ospNewGeometry("spheres");
    OSPData sphereDataArray = ospNewData(this->dataFile->numParticles,
            OSP_FLOAT3, this->dataFile->particleData);
    OSPData sphereColorDataArray = ospNewData(this->dataFile->numParticles,
            OSP_FLOAT3, this->dataFile->particleColorData);

    ospSet1i(this->oSpheres, "bytes_per_sphere", 12);
    ospSetData(this->oSpheres, "spheres", sphereDataArray);
    ospSetData(this->oSpheres, "color", sphereColorDataArray);
    ospSet1f(this->oSpheres, "radius", radius);
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

std::vector<float> Particles::getParticleCenter()
{
    return std::vector<float>({
            this->dataFile->midX,
            this->dataFile->midY,
            this->dataFile->midZ,
    });
}

OSPGeometry Particles::asOSPRayObject()
{
    return this->oSpheres;
}

bool Particles::isSurface()
{
    return true;
}

std::string Particles::getRenderType()
{
    return "particles";
}

}

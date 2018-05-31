#include "Particles.h"
#include "ParticleDataFile.h"

#include <ospray/ospray.h>

namespace pbnj {

Particles::Particles(std::string filename)
{
    this->ID = createID();
    this->dataFile = new ParticleDataFile();
    this->dataFile->loadFromFile(filename);

    // eventually will need a vector of ospray sphere objects
    // to support multiple named particle types (i.e. atoms)
    this->oSpheres = ospNewGeometry("spheres");
    OSPData sphereDataArray = ospNewData(this->dataFile->numParticles*3,
            OSP_FLOAT, this->dataFile->particleData);
    float color[] = {0.8, 0.8, 0.7};
    ospSetData(this->oSpheres, "spheres", sphereDataArray);
    ospSet1i(this->oSpheres, "bytes_per_sphere", 12);
    ospSet3fv(this->oSpheres, "color", color);
    ospSet1f(this->oSpheres, "radius", 0.5);
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

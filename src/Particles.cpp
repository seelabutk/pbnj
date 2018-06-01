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

    float defaultColor[] = {1.0, 0.0, 0.0};
    // should this iterator over particleData?
    for(std::string particleType : this->dataFile->particleTypes) {
        unsigned int numOfType = this->dataFile->getNumParticles(particleType);
        std::cerr << "DEBUG: adding " << particleType << " ";
        std::cerr << numOfType << std::endl;

        OSPGeometry spheres = ospNewGeometry("spheres");
        float *pData = this->dataFile->getParticleData(particleType);
        OSPData sphereDataArray = ospNewData(
                this->dataFile->getNumParticles(particleType)*3,
                OSP_FLOAT, pData);
        ospSetData(spheres, "spheres", sphereDataArray);
        ospSet1i(spheres, "bytes_per_sphere", 12);

        OSPData sphereColorDataArray;
        float rgb[numOfType * 3];
        if(CPKcolors.find(particleType) != CPKcolors.end()) {
            std::cerr << "DEBUG: found color for " << particleType << ": ";
            for(float val : CPKcolors[particleType])
                std::cerr << val << " ";
            std::cerr << std::endl;

            for(int p = 0; p < numOfType; p++) {
                rgb[p*3 + 0] = CPKcolors[particleType][0];
                rgb[p*3 + 1] = CPKcolors[particleType][1];
                rgb[p*3 + 2] = CPKcolors[particleType][2];
            }
        }
        else {
            for(int p = 0; p < numOfType; p++) {
                rgb[p*3 + 0] = defaultColor[0];
                rgb[p*3 + 1] = defaultColor[1];
                rgb[p*3 + 2] = defaultColor[2];
            }
        }
        sphereColorDataArray = ospNewData(numOfType, OSP_FLOAT3, rgb);

        ospSetData(spheres, "color", sphereColorDataArray);
        ospSet1f(spheres, "radius", 1.0);
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

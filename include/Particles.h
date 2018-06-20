#ifndef PBNJ_PARTICLES_H
#define PBNJ_PARTICLES_H

#include <pbnj.h>
#include <ParticleDataFile.h>

#include <string>
#include <vector>

#include <ospray/ospray.h>

namespace pbnj {

    class Particles {

        public:
            Particles(std::string filename);
            ~Particles();

            OSPGeometry asOSPRayObject();
            std::vector<float> getParticleCenter();

            std::string ID;

        private:
            OSPGeometry oSpheres;
            ParticleDataFile *dataFile;
    };
}

#endif

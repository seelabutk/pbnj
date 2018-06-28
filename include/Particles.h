#ifndef PBNJ_PARTICLES_H
#define PBNJ_PARTICLES_H

#include <pbnj.h>
#include <Subject.h>
#include <ParticleDataFile.h>

#include <string>
#include <vector>

#include <ospray/ospray.h>

namespace pbnj {

    class Particles : public Subject {

        public:
            Particles(std::string filename);
            ~Particles();

            OSPGeometry asOSPRayObject();
            bool isSurface();
            std::vector<float> getParticleCenter();
            std::string getRenderType();

            std::string ID;

        private:
            OSPGeometry oSpheres;
            ParticleDataFile *dataFile;
    };
}

#endif

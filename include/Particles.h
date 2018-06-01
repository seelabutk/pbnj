#ifndef PBNJ_PARTICLES_H
#define PBNJ_PARTICLES_H

#include <pbnj.h>
#include <ParticleDataFile.h>

#include <string>

#include <ospray/ospray.h>

namespace pbnj {

    class Particles {

        public:
            Particles(std::string filename);
            ~Particles();

            std::vector<OSPGeometry> asOSPRayObject();

            std::string ID;

        private:
            ParticleDataFile *dataFile;
            
            std::vector<OSPGeometry> sphereSets;
    };
}

#endif

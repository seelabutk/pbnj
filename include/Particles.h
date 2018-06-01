#ifndef PBNJ_PARTICLES_H
#define PBNJ_PARTICLES_H

#include <pbnj.h>
#include <ParticleDataFile.h>

#include <map>
#include <string>

#include <ospray/ospray.h>

namespace pbnj {

    // mapping between atom names and CPK colors
    extern std::map<std::string, std::vector<float>> CPKcolors;

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

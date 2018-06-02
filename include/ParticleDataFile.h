#ifndef PBNJ_PARTICLEDATAFILE_H
#define PBNJ_PARTICLEDATAFILE_H

#include <string>
#include <vector>
#include <unordered_map>

#include <pbnj.h>
#include <DataFile.h>

#include <map>

namespace pbnj {

    // mapping between atom names and CPK colors
    extern std::map<std::string, std::vector<float>> CPKcolors;
    static float defaultColor[3] = {0.8, 0.8, 0.6};

    class ParticleDataFile {
        public:
            ParticleDataFile();
            ~ParticleDataFile();

            void loadFromFile(std::string filename);

            float *particleData;
            float *particleColorData;
            unsigned int numParticles;

        private:
            FILETYPE getFiletype();
            FILETYPE filetype;
            std::string filename;
    };
}

#endif

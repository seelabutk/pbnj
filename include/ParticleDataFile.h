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

    class ParticleDataFile {
        public:
            ParticleDataFile();
            ~ParticleDataFile();

            void loadFromFile(std::string filename, bool center=false);

            float *particleData;
            float *particleColorData;
            unsigned int numParticles;

            float midX, midY, midZ;

        private:
            FILETYPE getFiletype();
            FILETYPE filetype;
            std::string filename;

            bool firstLineHasNumParticles(FILE *dataFile);
            void readAsMolecule(FILE *dataFile);
            void readAsGenericParticles(FILE *dataFile);
    };
}

#endif

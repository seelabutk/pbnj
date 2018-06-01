#ifndef PBNJ_PARTICLEDATAFILE_H
#define PBNJ_PARTICLEDATAFILE_H

#include <string>
#include <vector>
#include <unordered_map>

#include <pbnj.h>
#include <DataFile.h>

namespace pbnj {

    class ParticleDataFile {
        public:
            ParticleDataFile();
            ~ParticleDataFile();

            void loadFromFile(std::string filename);
            
            // unique particle types (atom names)
            // these are used to do lookups into the particleData map
            std::vector<std::string> particleTypes;

            unsigned int getNumParticles(std::string particleType);
            float *getParticleData(std::string particleType);

        private:
            FILETYPE getFiletype();
            FILETYPE filetype;
            std::string filename;
            unsigned int numParticles;

            // maps atom name to <x, y, z>
            std::unordered_map<std::string, std::vector<float> > particleData;
    };
}

#endif

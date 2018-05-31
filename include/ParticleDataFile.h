#ifndef PBNJ_PARTICLEDATAFILE_H
#define PBNJ_PARTICLEDATAFILE_H

#include <string>
#include <vector>

#include <pbnj.h>
#include <DataFile.h>

namespace pbnj {

    class ParticleDataFile {
        public:
            ParticleDataFile();
            ~ParticleDataFile();

            void loadFromFile(std::string filename);

        private:
            FILETYPE getFiletype();
            FILETYPE filetype;
            std::string filename;

            float *particleData;
            unsigned int numParticles;
    };
}

#endif

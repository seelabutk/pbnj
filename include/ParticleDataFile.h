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

            void loadFromFile(std::string filename);

            float *particleData;
            float *particleColorData;
            unsigned int numParticles;

            float averageXPos;
            float averageYPos;
            float averageZPos;
            std::vector<float> bounds;
            std::vector<float> extents;
            float dataXDim;
            float dataYDim;
            float dataZDim;

        private:
            FILETYPE getFiletype();
            FILETYPE filetype;
            std::string filename;

            void checkBounds(float &xMin, float &xMax, float &value);
    };
}

#endif

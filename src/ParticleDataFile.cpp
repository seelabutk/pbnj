#include "DataFile.h" // only for FILETYPE enum
#include "ParticleDataFile.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <stdlib.h>
#include <stdio.h>

namespace pbnj {

ParticleDataFile::ParticleDataFile()
    : numParticles(0)
{
}

ParticleDataFile::~ParticleDataFile()
{
}

void ParticleDataFile::loadFromFile(std::string filename)
{
    this->filename = filename;
    this->filetype = getFiletype();

    if(this->filetype == UNKNOWN) {
        std::cerr << "Unknown filetype!" << std::endl;
    }
    else if(this->filetype == PARTICLE) {
        FILE *dataFile = fopen(filename.c_str(), "r");
        if(dataFile == NULL) {
            std::cerr << "Could not open file!" << std::endl;
        }
        else {
            // find number of particles in the file
            char numParticlesLine[1024];
            char *res = fgets(numParticlesLine, 1024, dataFile);
            int read = sscanf(numParticlesLine, "%u ", &(this->numParticles));
            if(read != 1) {
                std::cerr << "Unexpected formatting error on first line";
                std::cerr << std::endl;
            }
            else {
                std::cout << "DEBUG: " << numParticlesLine;
                /*
                this->particleData = std::unordered_map<std::string,
                    std::vector<float> >();
                this->particleTypes.reserve(this->numParticles);
                */

                // skip the comment line
                char commentLine[1024];
                res = fgets(commentLine, 1024, dataFile);
                std::cerr << "DEBUG: " << commentLine;

                // start reading particles
                char particleLine[1024];
                char particleType[512];
                float curX, curY, curZ;
                int numRead = 0;
                for(int lineIndex = 0; lineIndex < this->numParticles;
                        lineIndex++) {
                    res = fgets(particleLine, 1024, dataFile);
                    read = sscanf(particleLine, "%s %f %f %f ",
                            particleType, &curX, &curY, &curZ);
                    if(read != 4) {
                        std::cerr << "Error reading line " << lineIndex;
                        std::cerr << std::endl;
                        std::cerr << particleLine << std::endl;
                        break;
                    }
                    std::cerr << "DEBUG: " << particleType << " ";
                    std::cerr << curX << " " << curY << " " << curZ;
                    std::cerr << std::endl;

                    // check if we've seen this type before
                    std::string pType(particleType);
                    if(this->particleData.find(pType) == this->particleData.end()) {
                        std::cerr << "DEBUG: found new particle type ";
                        std::cerr << pType << std::endl;
                        this->particleData[pType] = std::vector<float>({
                                curX, curY, curZ });
                        this->particleTypes.push_back(pType);
                    }
                    else {
                        this->particleData[pType].push_back(curX);
                        this->particleData[pType].push_back(curY);
                        this->particleData[pType].push_back(curZ);
                    }
                    numRead++;
                }
                std::cerr << "DEBUG: read " << numRead << " particles";
                std::cerr << std::endl;
                // sanity check
                if(this->numParticles != numRead)
                    std::cerr << "DEBUG: HEY THIS IS BAD" << std::endl;
                std::cerr << "DEBUG: " << this->particleTypes.size() << " types ";
                for(int i = 0; i < this->particleTypes.size(); i++)
                    std::cerr << this->particleTypes[i] << " ";
                std::cerr << std::endl;
            }
            fclose(dataFile);

        }
    }
    else {
        std::cerr << "Invalid filetype for particle data!" << std::endl;
    }
}

// TODO this should be a pbnj namespace function
// not a per-data-type function
FILETYPE ParticleDataFile::getFiletype()
{
    std::stringstream ss;
    ss.str(this->filename);
    std::string token;
    char delim = '.';
    
    while(std::getline(ss, token, delim)) {
    }

    if(token.compare("xyz") == 0) {
        return PARTICLE;
    }
    else {
        return UNKNOWN;
    }
}

unsigned int ParticleDataFile::getNumParticles(std::string particleType)
{
    if(particleType.empty() ||
       this->particleData.find(particleType) == this->particleData.end()) {
        std::cerr << "Invalid particle type requested";
        std::cerr << std::endl;
        return 0;
    }

    return this->particleData[particleType].size();
}

float *ParticleDataFile::getParticleData(std::string particleType)
{
    if(particleType.empty() ||
       this->particleData.find(particleType) == this->particleData.end()) {
        std::cerr << "Invalid particle type requested";
        std::cerr << std::endl;
        return nullptr;
    }

    return this->particleData[particleType].data();
}

}

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
            fgets(numParticlesLine, 1024, dataFile);
            int read = sscanf(numParticlesLine, "%u ", &(this->numParticles));
            if(read != 1) {
                std::cerr << "Unexpected formatting error on first line";
                std::cerr << std::endl;
            }
            else {
                std::cout << "DEBUG: " << numParticlesLine;
                this->particleData = 
                    (float *)malloc(this->numParticles * sizeof(float) * 3);

                // skip the comment line
                char commentLine[1024];
                fgets(commentLine, 1024, dataFile);
                std::cerr << "DEBUG: " << commentLine;

                // start reading particles
                char particleLine[1024];
                char particleType[512];
                float curX, curY, curZ;
                int numRead = 0;
                for(int lineIndex = 0; lineIndex < this->numParticles;
                        lineIndex++) {
                    fgets(particleLine, 1024, dataFile);
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
                    int particleOffset = lineIndex*3;
                    this->particleData[particleOffset + 0] = curX;
                    this->particleData[particleOffset + 1] = curY;
                    this->particleData[particleOffset + 2] = curZ;
                    numRead++;
                }
                std::cerr << "DEBUG: read " << numRead << " particles";
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

}

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
                this->particleData = 
                    (float *)malloc(this->numParticles * 3 * sizeof(float));
                this->particleColorData =
                    (float *)malloc(this->numParticles * 3 * sizeof(float));

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

                    this->particleData[lineIndex*3 + 0] = curX;
                    this->particleData[lineIndex*3 + 1] = curY;
                    this->particleData[lineIndex*3 + 2] = curZ;

                    // check if the particle type is a known type
                    // so we can add a known color
                    float *rgb;
                    if(CPKcolors.find(particleType) != CPKcolors.end()) {
                        rgb = CPKcolors[particleType].data();
                    }
                    // if it isn't, use a default color
                    else {
                        rgb = defaultColor;
                    }
                    this->particleColorData[lineIndex*3 + 0] = rgb[0];
                    this->particleColorData[lineIndex*3 + 1] = rgb[1];
                    this->particleColorData[lineIndex*3 + 2] = rgb[2];
                    numRead++;
                }
                std::cerr << "DEBUG: read " << numRead << " particles";
                std::cerr << std::endl;
                // sanity check
                if(this->numParticles != numRead)
                    std::cerr << "DEBUG: HEY THIS IS BAD" << std::endl;
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

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
    : numParticles(0), averageXPos(0), averageYPos(0), averageZPos(0)
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
            if(this->firstLineHasNumParticles(dataFile)) {
                std::cerr << "DEBUG: " << this->numParticles << " atoms in file";
                std::cerr << std::endl;
                // skip the comment line
                char commentLine[1024];
                char *res = fgets(commentLine, 1024, dataFile);
                std::cerr << "DEBUG: " << commentLine;

                // read the rest of the file as if it was a molecule
                this->readAsMolecule(dataFile);
            }
            else {
                // no line stating num particles
                // assume there is no comment line
                // read the file as if it's just particle data (e.g. lidar)

                rewind(dataFile);
                this->readAsGenericParticles(dataFile);
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

bool ParticleDataFile::firstLineHasNumParticles(FILE *dataFile)
{
    // find number of particles in the file
    char numParticlesLine[1024];
    char *res = fgets(numParticlesLine, 1024, dataFile);

    char buffer[512];
    int sofar;
    int read = sscanf(numParticlesLine, "%u %s %n",
            &(this->numParticles), buffer, &sofar);
    return (sofar == 1);
}

void ParticleDataFile::readAsMolecule(FILE *dataFile)
{
    this->particleData = 
        (float *)malloc(this->numParticles * 3 * sizeof(float));
    this->particleColorData =
        (float *)malloc(this->numParticles * 3 * sizeof(float));
    double totalX = 0, totalY = 0, totalZ = 0;

    // start reading particles
    char particleLine[1024];
    char particleType[512];
    char particleColor[512];
    float curX, curY, curZ;
    int numRead = 0;


    for(int lineIndex = 0; lineIndex < this->numParticles; lineIndex++) {
        char *res = fgets(particleLine, 1024, dataFile);
        int read = sscanf(particleLine, "%s %f %f %f %[^\n]",
                particleType, &curX, &curY, &curZ, particleColor);
        bool useCustomColor = (read == 5);
        if(read > 5 || read < 4) {
            std::cerr << "Error reading line " << lineIndex;
            std::cerr << std::endl;
            std::cerr << particleLine << std::endl;
            break;
        }

        this->particleData[lineIndex*3 + 0] = curX;
        this->particleData[lineIndex*3 + 1] = curY;
        this->particleData[lineIndex*3 + 2] = curZ;

        totalX += curX; totalY += curY; totalZ += curZ;

        // check if the file supplied a color
        float *rgb;
        if(useCustomColor) {
            float r, g, b;
            read = sscanf(particleColor, "%f %f %f ", &r, &g, &b);
            if(read != 3) {
                rgb = CPKcolors["default"].data();
            }
            else {
                rgb = (float *)malloc(3 * sizeof(float));
                rgb[0] = r; rgb[1] = g; rgb[2] = b;
            }
        }
        // check if the particle type is a known type
        // so we can add a known color
        else if(CPKcolors.find(particleType) != CPKcolors.end()) {
            rgb = CPKcolors[particleType].data();
        }
        // if it isn't, use a default color
        else {
            rgb = CPKcolors["default"].data();
        }
        this->particleColorData[lineIndex*3 + 0] = rgb[0];
        this->particleColorData[lineIndex*3 + 1] = rgb[1];
        this->particleColorData[lineIndex*3 + 2] = rgb[2];
        numRead++;
    }

    this->averageXPos = totalX / this->numParticles;
    this->averageYPos = totalY / this->numParticles;
    this->averageZPos = totalZ / this->numParticles;

    std::cerr << "DEBUG: read " << numRead << " atoms";
    std::cerr << std::endl;
    std::cerr << "DEBUG: average position: ";
    std::cerr << this->averageXPos << " ";
    std::cerr << this->averageYPos << " ";
    std::cerr << this->averageZPos << std::endl;
}

void ParticleDataFile::readAsGenericParticles(FILE *dataFile)
{
    char particleLine[1024];
    char particleColor[512];
    double totalX = 0, totalY = 0, totalZ = 0;
    int numRead = 0;
    float curX, curY, curZ;

    while(fgets(particleLine, 1024, dataFile) != NULL) {
        this->numParticles++;
    }

    rewind(dataFile);

    this->particleData =
        (float *)malloc(this->numParticles * 3 * sizeof(float));
    this->particleColorData =
        (float *)malloc(this->numParticles * 3 * sizeof(float));

    for(int lineIndex = 0; lineIndex < this->numParticles; lineIndex++) {
        // check for three valid coordinates in each line
        int read = sscanf(particleLine, "%f %f %f %[^\n]",
                &curX, &curY, &curZ, particleColor);
        bool useCustomColor = (read == 4);
        if(read > 4 || read < 3) {
            std::cerr << "Error reading line " << lineIndex;
            std::cerr << std::endl;
            std::cerr << particleLine << std::endl;
            break;
        }
        numRead++;

        totalX += curX;
        totalY += curY;
        totalZ += curZ;

        this->particleData[lineIndex + 0] = curX;
        this->particleData[lineIndex + 1] = curY;
        this->particleData[lineIndex + 2] = curZ;

        if(useCustomColor) {
            float r, g, b;
            read = sscanf(particleColor, "%f %f %f", &r, &g, &b);
            if(read != 3) {
                r = g = b = 1.f;
            }
            this->particleColorData[lineIndex + 0] = r;
            this->particleColorData[lineIndex + 1] = g;
            this->particleColorData[lineIndex + 2] = b;
        }
        else {
            this->particleColorData[lineIndex + 0] = 1.f;
            this->particleColorData[lineIndex + 1] = 1.f;
            this->particleColorData[lineIndex + 2] = 1.f;
        }

    }

    this->averageXPos = totalX / this->numParticles;
    this->averageYPos = totalY / this->numParticles;
    this->averageZPos = totalZ / this->numParticles;

    std::cerr << "DEBUG: read " << numRead << " particles";
    std::cerr << std::endl;
    std::cerr << "DEBUG: average position: ";
    std::cerr << this->averageXPos << " ";
    std::cerr << this->averageYPos << " ";
    std::cerr << this->averageZPos << std::endl;
}

}

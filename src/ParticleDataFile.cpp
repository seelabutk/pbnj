#include "DataFile.h" // only for FILETYPE enum
#include "ParticleDataFile.h"
#include "TransferFunction.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

void checkMinMax(float &minVal, float &maxVal, float &val)
{
    minVal = std::min(minVal, val);
    maxVal = std::max(maxVal, val);
}

namespace pbnj {

ParticleDataFile::ParticleDataFile()
    : numParticles(0), midX(0), midY(0), midZ(0)
{
    resetMinMax();
}

ParticleDataFile::~ParticleDataFile()
{
}

void ParticleDataFile::loadFromFile(std::string filename, bool center, bool autocolor)
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
                std::cerr << std::endl;
                // skip the comment line
                char commentLine[1024];
                char *res = fgets(commentLine, 1024, dataFile);

                // read the rest of the file as if it was a molecule
                this->readAsMolecule(dataFile, autocolor);
            }
            else {
                // no line stating num particles
                // assume there is no comment line
                // read the file as if it's just particle data (e.g. lidar)

                rewind(dataFile);
                this->readAsGenericParticles(dataFile, autocolor);
            }
            fclose(dataFile);

            if(center) {
                float totalX = 0, totalY = 0, totalZ = 0;
                this->resetMinMax();
                for(int pIndex = 0; pIndex < this->numParticles; pIndex++) {
                    this->particleData[pIndex*3 + 0] -= this->midX;
                    checkMinMax(this->minX, this->maxX, this->particleData[pIndex*3 + 0]);
                    this->particleData[pIndex*3 + 1] -= this->midY;
                    checkMinMax(this->minY, this->maxY, this->particleData[pIndex*3 + 1]);
                    this->particleData[pIndex*3 + 2] -= this->midZ;
                    checkMinMax(this->minZ, this->maxZ, this->particleData[pIndex*3 + 2]);
                }
                this->midX = this->minX + (this->maxX - this->minX) / 2;
                this->midY = this->minY + (this->maxY - this->minY) / 2;
                this->midZ = this->minZ + (this->maxZ - this->minZ) / 2;
            }

            fprintf(stderr, "DEBUG: min %f %f %f\n", this->minX, this->minY, this->minZ);
            fprintf(stderr, "DEBUG: mid %f %f %f\n", this->midX, this->midY, this->midZ);
            fprintf(stderr, "DEBUG: max %f %f %f\n", this->maxX, this->maxY, this->maxZ);
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

    char c;
    int read = sscanf(numParticlesLine, "%u%c", &(this->numParticles), &c);

    return (read == 2 && isspace(c));
}

void ParticleDataFile::readAsMolecule(FILE *dataFile, bool autocolor)
{
    std::cerr << "DEBUG: reading as molecule" << std::endl;
    this->particleData = 
        (float *)malloc(this->numParticles * 3 * sizeof(float));
    this->particleColorData =
        (float *)malloc(this->numParticles * 4 * sizeof(float));
    float totalX = 0, totalY = 0, totalZ = 0;

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
            std::cerr << " of molecule" << std::endl;
            std::cerr << particleLine << std::endl;
            break;
        }

        this->particleData[lineIndex*3 + 0] = curX;
        this->particleData[lineIndex*3 + 1] = curY;
        this->particleData[lineIndex*3 + 2] = curZ;

        totalX += curX; totalY += curY; totalZ += curZ;

        checkMinMax(this->minX, this->maxX, curX);
        checkMinMax(this->minY, this->maxY, curY);
        checkMinMax(this->minZ, this->maxZ, curZ);

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
        this->particleColorData[lineIndex*4 + 0] = rgb[0];
        this->particleColorData[lineIndex*4 + 1] = rgb[1];
        this->particleColorData[lineIndex*4 + 2] = rgb[2];
        this->particleColorData[lineIndex*4 + 3] = 1.f;
        numRead++;
    }

    this->midX = this->minX + (this->maxX - this->minX) / 2;
    this->midY = this->minY + (this->maxY - this->minY) / 2;
    this->midZ = this->minZ + (this->maxZ - this->minZ) / 2;
}

void ParticleDataFile::readAsGenericParticles(FILE *dataFile, bool autocolor)
{
    std::cerr << "DEBUG: reading as generic data" << std::endl;
    char particleLine[1024];
    char particleColor[512];
    float totalX = 0, totalY = 0, totalZ = 0;
    unsigned int numRead = 0;
    float curX = 0;
    float curY = 0;
    float curZ = 0;

    this->numParticles = 0;
    while(fgets(particleLine, 1024, dataFile) != NULL) {
        this->numParticles++;
    }

    std::cerr << "DEBUG: " << this->numParticles << " particles found" << std::endl;

    rewind(dataFile);

    this->particleData =
        (float *)malloc(this->numParticles * 3 * sizeof(float));
    this->particleColorData =
        (float *)malloc(this->numParticles * 4 * sizeof(float));

    for(int lineIndex = 0; lineIndex < this->numParticles; lineIndex++) {
        char *res = fgets(particleLine, 1024, dataFile);
        // check for three valid coordinates in each line
        int read = sscanf(particleLine, "%f %f %f %[^\n]",
                &curX, &curZ, &curY, particleColor);
        bool useCustomColor = (read == 4);
        if(read > 4 || read < 3) {
            std::cerr << "Error reading line " << lineIndex;
            std::cerr << " of particle data" << std::endl;
            std::cerr << particleLine << std::endl;
            break;
        }
        numRead++;

        curZ *= 2.0;

        totalX += curX;
        totalY += curY;
        totalZ += curZ;

        checkMinMax(this->minX, this->maxX, curX);
        checkMinMax(this->minY, this->maxY, curY);
        checkMinMax(this->minZ, this->maxZ, curZ);

        this->particleData[lineIndex*3 + 0] = curX;
        this->particleData[lineIndex*3 + 1] = curY;
        this->particleData[lineIndex*3 + 2] = curZ;


        if(useCustomColor) {
            float r, g, b;
            read = sscanf(particleColor, "%f %f %f ", &r, &g, &b);
            if(read != 3) {
                this->particleColorData[lineIndex*4 + 0] = 1.f;
                this->particleColorData[lineIndex*4 + 1] = 1.f;
                this->particleColorData[lineIndex*4 + 2] = 1.f;
                this->particleColorData[lineIndex*4 + 3] = 1.f;
            }
            else {
                this->particleColorData[lineIndex*4 + 0] = r;
                this->particleColorData[lineIndex*4 + 1] = g;
                this->particleColorData[lineIndex*4 + 2] = b;
                this->particleColorData[lineIndex*4 + 3] = 1.f;
                autocolor = false; // disable autocolor if color provided
            }
        }
        else {
            this->particleColorData[lineIndex*4 + 0] = 1.f;
            this->particleColorData[lineIndex*4 + 1] = 1.f;
            this->particleColorData[lineIndex*4 + 2] = 1.f;
            this->particleColorData[lineIndex*4 + 3] = 1.f;
        }
    }

    this->midX = this->minX + (this->maxX - this->minX) / 2;
    this->midY = this->minY + (this->maxY - this->minY) / 2;
    this->midZ = this->minZ + (this->maxZ - this->minZ) / 2;

    if(autocolor) { this->doAutocolor(); }
}

void ParticleDataFile::resetMinMax()
{
    this->minX = this->minY = this->minZ = std::numeric_limits<float>::max();
    this->maxX = this->maxY = this->maxZ = std::numeric_limits<float>::min();
}

void ParticleDataFile::doAutocolor()
{
    // assume z axis
    float rangeZ = this->maxZ - this->minZ;
    for(int pIndex = 0; pIndex < this->numParticles; pIndex++) {
        float gray = (this->particleData[pIndex*3 + 2] - this->minZ) / rangeZ;
        int cmIndex = (int)(gray * colormaps["gist_earth"].size()/3);
        cmIndex = std::max(0, std::min(cmIndex, (int)colormaps["gist_earth"].size()/3));
        this->particleColorData[pIndex*4 + 0] = colormaps["gist_earth"][cmIndex*3 + 0];
        this->particleColorData[pIndex*4 + 1] = colormaps["gist_earth"][cmIndex*3 + 1];
        this->particleColorData[pIndex*4 + 2] = colormaps["gist_earth"][cmIndex*3 + 2];
        this->particleColorData[pIndex*4 + 3] = 1.f;
    }
}

}

#include "DataFile.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

#include <algorithm>

#ifdef PBNJ_NETCDF
#include <netcdf>
#include <map>
#endif

namespace pbnj {

DataFile::DataFile(int x, int y, int z, unsigned int components) :
    xDim(x), yDim(y), zDim(z), numValues(x*y*z), statsCalculated(false),
    numComponents(components)
{
    this->numValues = xDim * yDim * zDim;
}

DataFile::~DataFile()
{
    if(this->data != NULL) {
        if(this->wasMemoryMapped) {
            int mresult = munmap(this->data, this->numValues*sizeof(float));
            if(mresult == -1)
                std::cerr << "WARNING: Couldn't unmap data!" << std::endl;
        }
        else
            free(this->data);
        this->data = NULL;
    }
}

void DataFile::loadFromFile(std::string filename, std::string var_name,
        bool memmap)
{
    //check if the filetype is known
    this->filename = filename;
    this->filetype = getFiletype();

    if(this->filetype == UNKNOWN) {
        std::cerr << "Unknown filetype!" << std::endl;
    }
    else if(this->filetype == NETCDF) {
#ifdef PBNJ_NETCDF
        // no explicit close needed, destructor calls it
        netCDF::NcFile dataFile(filename.c_str(), netCDF::NcFile::read);

        netCDF::NcVar variable;
        if(var_name.compare("") == 0) {
            // only get the first variable
            const std::multimap<std::string, netCDF::NcVar> varmap = 
                dataFile.getVars();
            variable = varmap.begin()->second;
        }
        else {
            variable = dataFile.getVar(var_name);
        }

        // overwrite any configured values with the file's values
        this->xDim = (long unsigned int) variable.getDim(2).getSize();
        this->yDim = (long unsigned int) variable.getDim(1).getSize();
        this->zDim = (long unsigned int) variable.getDim(0).getSize();
        this->numValues = this->xDim * this->yDim * this->zDim;

        // load data
        this->data = (float *)malloc(this->numValues * sizeof(float));
        variable.getVar(this->data);
#else
        std::cerr << "PBNJ was not built with NetCDF support!" << std::endl;
#endif
    }
    else {
        FILE *dataFile = fopen(filename.c_str(), "r");

        if(dataFile == NULL) {
            std::cerr << "Could not open file!" << std::endl;
        }
        else {
            if(memmap) {
                if(this->numComponents == 1) {
                    int fd = fileno(dataFile);
                    this->data = (float *)mmap(NULL, this->numValues*sizeof(float),
                            PROT_READ, MAP_SHARED, fd, 0);
                }
                else {
                    this->data = (float *)malloc(this->numValues * sizeof(float));
                    if(this->data == MAP_FAILED) {
                        switch(errno) {
                            case ENOMEM:
                                std::cerr << "ENOMEM out of memory" << std::endl;
                                break;
                        }
                    }
                    float *chunk = (float *)malloc(this->numComponents*sizeof(float));
                    for(int i = 0; i < this->numValues; i++) {
                        size_t numbytes = fread(chunk, sizeof(float),
                                this->numComponents, dataFile);
                        this->data[i] = std::sqrt(chunk[0]*chunk[0] +
                                chunk[1]*chunk[1] + chunk[2]*chunk[2]);
                    }
                }
            }
            else {
                this->data = (float *)malloc(this->numValues * sizeof(float));
                if(this->data == MAP_FAILED) {
                    switch(errno) {
                        case ENOMEM:
                            std::cerr << "ENOMEM out of memory" << std::endl;
                            break;
                    }
                }
                if(this->numComponents == 1) {
                    size_t bytes = fread(this->data, sizeof(float),
                            this->numValues, dataFile);
                }
                else {
                    float *chunk = (float *)malloc(this->numComponents*sizeof(float));
                    for(int i = 0; i < this->numValues; i++) {
                        size_t numbytes = fread(chunk, sizeof(float),
                                this->numComponents, dataFile);
                        this->data[i] = std::sqrt(chunk[0]*chunk[0] +
                                chunk[1]*chunk[1] + chunk[2]*chunk[2]);
                    }
                }
            }
            fclose(dataFile);
        }
    }
    this->wasMemoryMapped = memmap;
}

FILETYPE DataFile::getFiletype()
{
    std::stringstream ss;
    ss.str(this->filename);
    std::string token;
    char delim = '.';
    // keep the last token after splitting on dots
    while(std::getline(ss, token, delim)) {
    }

    if(token.compare("bin") == 0 ||
       token.compare("dat") == 0 ||
       token.compare("raw") == 0) {
        return BINARY;
    }
    else if(token.compare("nc") == 0) {
        return NETCDF;
    }
    else {
        return UNKNOWN;
    }
}

void DataFile::calculateStatistics()
{
    // calculate min, max, avg, stddev
    // stddev and avg may be useful for automatic diverging color maps
    this->minVal = data[0];
    this->maxVal = data[0];
    double total = 0, totalSquares = 0;
    for(int i = 1; i < this->numValues; i++) {
        if(data[i] < this->minVal)
            this->minVal = data[i];
        if(data[i] > this->maxVal)
            this->maxVal = data[i];
        total += data[i];
        totalSquares += data[i]*data[i];
    }
    this->avgVal = total / this->numValues;
    this->stdDev = std::sqrt(totalSquares/this->numValues -
                             this->avgVal*this->avgVal);
    this->statsCalculated = true;
}

void DataFile::printStatistics()
{
    // debugging purposes
    if(!this->statsCalculated) {
        std::cerr << "Statistics not calculated for this data!" << std::endl;
        return;
    }

    std::cout << this->filename << std::endl;
    std::cout << "dimensions: " << this->xDim << ", "
                                << this->yDim << ", "
                                << this->zDim << std::endl;
    std::cout << "minimum:    " << this->minVal << std::endl;
    std::cout << "maximum:    " << this->maxVal << std::endl;
    std::cout << "mean:       " << this->avgVal << std::endl;
    std::cout << "std. dev.:  " << this->stdDev << std::endl;
}

// experimental
void DataFile::bin(unsigned int num_bins)
{
    if(!this->statsCalculated)
        this->calculateStatistics();

    float bin_width = (this->maxVal - this->minVal) / num_bins;
    unsigned int *histogram = (unsigned int *) calloc(num_bins, 
            sizeof(unsigned int));

    for(int i = 0; i < this->numValues; i++) {
        float bin = (this->data[i] - this->minVal) / bin_width;
        unsigned int hist_index = std::min(num_bins - 1, (unsigned int) bin);
        histogram[hist_index]++;
    }

    std::cout << "Calculated histogram:" << std::endl;
    for(int i = 0; i < num_bins; i++)
        std::cout << histogram[i] << std::endl;

    free(histogram);
}

}

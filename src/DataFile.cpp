#include "DataFile.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <stdlib.h>
#include <stdio.h>

namespace pbnj {

DataFile::DataFile(int x, int y, int z) :
    xDim(x), yDim(y), zDim(z), numValues(x*y*z), statsCalculated(false)
{
}

DataFile::~DataFile()
{
    if(this->data != NULL) {
        free(this->data);
    }
}

void DataFile::loadFromFile(std::string filename)
{
    //check if the filetype is known
    //only binary is known for now
    this->filename = filename;
    this->filetype = getFiletype();
    if(this->filetype == UNKNOWN) {
        std::cerr << "Unknown filetype!" << std::endl;
    }

    FILE *dataFile = fopen(filename.c_str(), "r");

    if(dataFile == NULL) {
        std::cerr << "Could not open file!" << std::endl;
    }
    else {
        this->data = (float *)malloc(this->numValues * sizeof(float));
        fread(this->data, sizeof(float), this->numValues, dataFile);
        fclose(dataFile);
    }
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

}

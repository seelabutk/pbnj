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
    xDim(x), yDim(y), zDim(z)
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

    int numValues = this->xDim * this->yDim * this->zDim;
    FILE *dataFile = fopen(filename.c_str(), "r");

    if(dataFile == NULL) {
        std::cerr << "Could not open file!" << std::endl;
    }
    else {
        this->data = (float *)malloc(numValues * sizeof(float));
        fread(this->data, sizeof(float), numValues, dataFile);
        fclose(dataFile);
    }

    //calculate min, max, avg, stdev
    this->minVal = data[0];
    this->maxVal = data[0];
    double total = 0, totalSquares = 0;
    for(int i = 1; i < numValues; i++) {
        if(data[i] < this->minVal)
            this->minVal = data[i];
        if(data[i] > this->maxVal)
            this->maxVal = data[i];
        total += data[i];
        totalSquares += data[i]*data[i];
    }
    this->avgVal = total / numValues;
    this->stdDev = std::sqrt(totalSquares/numValues-this->avgVal*this->avgVal);
}

FILETYPE DataFile::getFiletype()
{
    std::stringstream ss;
    ss.str(this->filename);
    std::string token;
    char delim = '.';
    while(std::getline(ss, token, delim)) {
    }

    std::cerr << "DEBUG: extension is "  << token << std::endl;

    if(token.compare("bin") == 0 ||
       token.compare("dat") == 0 ||
       token.compare("raw") == 0) {
        return BINARY;
    }
    else {
        return UNKNOWN;
    }
}

void DataFile::printStatistics()
{
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

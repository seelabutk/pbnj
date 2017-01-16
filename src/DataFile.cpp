#include "DataFile.h"

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

}

#include "TimeSeries.h"
#include "Volume.h"

#include <iostream>

namespace pbnj {

TimeSeries::TimeSeries(std::vector<std::string> filenames,
        int x, int y, int z) :
    dataFilenames(filenames), length(filenames.size()), xDim(x), yDim(y),
    zDim(z)
{
    this->volumes = new Volume*[this->length];
    for(int i = 0; i < this->length; i++)
        this->volumes[i] = NULL;
}

TimeSeries::TimeSeries(std::vector<std::string> filenames,
        std::string varname, int x, int y, int z) :
    dataFilenames(filenames), length(filenames.size()), dataVariable(varname),
    xDim(x), yDim(y), zDim(z)
{
    this->volumes = new Volume*[this->length];
    for(int i = 0; i < this->length; i++)
        this->volumes[i] = NULL;
}

TimeSeries::~TimeSeries()
{
    for(int i = 0; i < this->length; i++) {
        if(this->volumes[i] != NULL) {
            delete this->volumes[i];
            this->volumes[i] = NULL;
        }
    }
}

Volume *TimeSeries::getVolume(unsigned int index)
{
    if(index >= this->length) {
        std::cerr << "WARNING: Asked for volume " << index;
        std::cerr << " in a time series of length " << length << std::endl;
        return NULL;
    }

    if(this->volumes[index] == NULL) {
        if(this->dataVariable.empty())
            this->volumes[index] = new Volume(this->dataFilenames[index],
                    this->xDim, this->yDim, this->zDim);
        else
            this->volumes[index] = new Volume(this->dataFilenames[index],
                    this->dataVariable, this->xDim, this->yDim, this->zDim);
    }
    
    return this->volumes[index];
}

unsigned int TimeSeries::getLength()
{
    return this->length;
}

}

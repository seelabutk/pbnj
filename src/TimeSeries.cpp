#include "TimeSeries.h"
#include "Volume.h"

#include <iostream>
#include <sys/sysinfo.h>

namespace pbnj {

TimeSeries::TimeSeries(std::vector<std::string> filenames,
        int x, int y, int z) :
    dataFilenames(filenames), length(filenames.size()), xDim(x), yDim(y),
    zDim(z), dataSize(x*y*z*4)
{
    this->volumes = new Volume*[this->length];
    for(int i = 0; i < this->length; i++)
        this->volumes[i] = NULL;
    this->initSystemInfo();
}

TimeSeries::TimeSeries(std::vector<std::string> filenames,
        std::string varname, int x, int y, int z) :
    dataFilenames(filenames), length(filenames.size()), dataVariable(varname),
    xDim(x), yDim(y), zDim(z), dataSize(x*y*z*4)
{
    this->volumes = new Volume*[this->length];
    for(int i = 0; i < this->length; i++)
        this->volumes[i] = NULL;
    this->initSystemInfo();
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

void TimeSeries::initSystemInfo()
{
    //system data
    //struct sysinfo system_info;
    sysinfo(&(this->systemInfo));
    unsigned long totalBytes, freeBytes, maxUsage;
    totalBytes = this->systemInfo.mem_unit * this->systemInfo.totalram /
        1073741824; //GB
    freeBytes = this->systemInfo.mem_unit * this->systemInfo.freeram /
        1073741824; // GB
    maxUsage = this->systemInfo.mem_unit * this->systemInfo.freeram *
        0.5; // bytes
    this->maxVolumes = maxUsage / this->dataSize;
}

void TimeSeries::setMaxMemory(unsigned int gigabytes)
{
    unsigned long freeBytes = this->systemInfo.mem_unit * this->systemInfo.freeram / 1073741824; // GB
    if(gigabytes > freeBytes) {
        std::cerr << "WARNING: Asking to use more memory than is currently ";
        std::cerr << "available. Keeping limit at previous value" << std::endl;
        return;
    }
    unsigned long bytes = 1073741824 * gigabytes;
    if(bytes < this->dataSize) {
        std::cerr << "WARNING: Asking to use less memory than a single volume ";
        std::cerr << "requires. Keeping limit at previous value" << std::endl;
        return;
    }
    this->maxVolumes = bytes / this->dataSize;
}

void TimeSeries::encache(unsigned int index)
{
    if(this->lruCache.size() >= this->maxVolumes) {
        // check if the LRU is this index
        if(this->lruCache.front() == index) {
            this->lruCache.remove(index);
            // and put it at the end
            this->lruCache.push_back(index);
        }
        else {
            // delete the LRU volume
            delete this->volumes[this->lruCache.front()];
            this->volumes[this->lruCache.front()] = NULL;
            this->lruCache.pop_front();
        }
    }
    else {
        // remove this index if it was already here
        this->lruCache.remove(index);
        // and put it at the end
        this->lruCache.push_back(index);
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
        this->encache(index);
    }
    
    return this->volumes[index];
}

unsigned int TimeSeries::getLength()
{
    return this->length;
}

}

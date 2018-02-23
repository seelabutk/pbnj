#include "TimeSeries.h"
#include "Volume.h"

#include <iostream>
#include <algorithm>
#include <sys/sysinfo.h>

namespace pbnj {

TimeSeries::TimeSeries(std::vector<std::string> filenames,
        int x, int y, int z, unsigned int components) :
    dataFilenames(filenames), length(filenames.size()), xDim(x), yDim(y),
    zDim(z), dataSize(x*y*z*4), numComponents(components)
{
    this->volumes = new Volume*[this->length];
    for(int i = 0; i < this->length; i++)
        this->volumes[i] = NULL;
    this->initSystemInfo();
    // default values for volume attributes
    this->opacityAttenuation = 1.0;
    this->doMemoryMap = false;
}

TimeSeries::TimeSeries(std::vector<std::string> filenames,
        std::string varname, int x, int y, int z, unsigned int components) :
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
        1073741824L; //GB
    freeBytes = this->systemInfo.mem_unit * this->systemInfo.freeram /
        1073741824L; // GB
    maxUsage = this->systemInfo.mem_unit * this->systemInfo.freeram *
        0.5; // bytes
    this->maxVolumes = maxUsage / this->dataSize;
}

void TimeSeries::setMaxMemory(unsigned int gigabytes)
{
    unsigned long freeBytes = this->systemInfo.mem_unit * this->systemInfo.freeram / 1073741824L; // GB
    if(gigabytes > freeBytes) {
        std::cerr << "WARNING: Asking to use more memory than is currently ";
        std::cerr << "available. Keeping limit at previous value" << std::endl;
        return;
    }
    unsigned long bytes = 1073741824L * gigabytes;
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
        // load the volume
        if(this->dataVariable.empty())
            this->volumes[index] = new Volume(this->dataFilenames[index],
                    this->xDim, this->yDim, this->zDim, this->doMemoryMap,
                    this->numComponents);
        else
            this->volumes[index] = new Volume(this->dataFilenames[index],
                    this->dataVariable, this->xDim, this->yDim, this->zDim,
                    this->doMemoryMap, this->numComponents);

        // set any given attributes
        if(!this->colorMap.empty())
            this->volumes[index]->setColorMap(this->colorMap);
        if(!this->opacityMap.empty())
            this->volumes[index]->setOpacityMap(this->opacityMap);
        this->volumes[index]->attenuateOpacity(this->opacityAttenuation);

        // place this volume in cache and/or set it as the newest
        this->encache(index);
    }
    
    return this->volumes[index];
}

int TimeSeries::getVolumeIndex(std::string filename)
{
    int index = 0;
    int found = -1;
    for (auto iter = this->dataFilenames.begin(); 
            iter != this->dataFilenames.end(); iter++) {
        std::size_t temp_index = (*iter).rfind(filename);
        if (temp_index != std::string::npos)
        {
            std::cout<<temp_index<<" "<<std::string::npos<<std::endl;
            found = index;
            break;
        }
        index++;
    }
    if (found == -1)
    {
        std::cerr<<"WARNING: Asked for non-existing volume: "<<filename;
        std::cerr<<std::endl;
        return -1;
    }
    else
    {
        return found;
    }
}

unsigned int TimeSeries::getLength()
{
    return this->length;
}

void TimeSeries::setColorMap(std::vector<float> &map)
{
    if(map.empty())
        return;
    this->colorMap = map;
}

void TimeSeries::setOpacityMap(std::vector<float> &map)
{
    if(map.empty())
        return;
    this->opacityMap = map;
}

void TimeSeries::setOpacityAttenuation(float attenuation)
{
    this->opacityAttenuation = attenuation;
}

void TimeSeries::setMemoryMapping(bool toMMap)
{
    this->doMemoryMap = toMMap;
}

}

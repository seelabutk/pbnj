#include "Volume.h"
#include "DataFile.h"
#include "TransferFunction.h"

#include <vector>

namespace pbnj {

Volume::Volume(std::string filename, int x, int y, int z)
{
    //volumes contain a datafile
    //one datafile per volume, one volume per renderer/camera
    this->dataFile = new DataFile(x, y, z);
    this->loadFromFile(filename);

    this->init();
}

Volume::Volume(std::string filename, std::string var_name, int x, int y, int z)
{
    //volumes contain a datafile
    //one datafile per volume, one volume per renderer/camera
    this->dataFile = new DataFile(x, y, z);
    this->loadFromFile(filename, var_name);

    this->init();
}

void Volume::init()
{
    //set up default transfer function
    this->transferFunction = new TransferFunction();
    this->transferFunction->setRange(this->dataFile->minVal,
                                     this->dataFile->maxVal);

    //setup OSPRay objects
    this->oVolume = ospNewVolume("shared_structured_volume");
    this->oData = ospNewData(this->dataFile->numValues, OSP_FLOAT, 
            this->dataFile->data, OSP_DATA_SHARED_BUFFER);

    int dimensions[] = {this->dataFile->xDim, 
                        this->dataFile->yDim,
                        this->dataFile->zDim};
    float center[] = {-dimensions[0]/(float)2.0,
                      -dimensions[1]/(float)2.0,
                      -dimensions[2]/(float)2.0};

    float voxelRange[] = {this->dataFile->minVal, 
                          this->dataFile->maxVal};
    ospSetData(this->oVolume, "voxelData", this->oData);
    ospSet3iv(this->oVolume, "dimensions", dimensions);
    ospSetString(this->oVolume, "voxelType", "float");
    ospSet2fv(this->oVolume, "voxelRange", voxelRange);
    ospSet3fv(this->oVolume, "gridOrigin", center);
    ospSetObject(this->oVolume, "transferFunction",
            this->transferFunction->asOSPObject());
    ospCommit(this->oVolume);
}

Volume::~Volume()
{
    delete this->dataFile;
    this->dataFile = NULL;
    delete this->transferFunction;
    this->transferFunction = NULL;
}

void Volume::attenuateOpacity(float amount)
{
    this->transferFunction->attenuateOpacity(amount);
}

void Volume::setColorMap(std::vector<float> &map)
{
    this->transferFunction->setColorMap(map);
}

void Volume::setOpacityMap(std::vector<float> &map)
{
    this->transferFunction->setOpacityMap(map);
}

std::vector<int> Volume::getBounds()
{
    std::vector<int> bounds = {this->dataFile->xDim, this->dataFile->yDim,
        this->dataFile->zDim};
    return bounds;
}

OSPVolume Volume::asOSPRayObject()
{
    return this->oVolume;
}

void Volume::loadFromFile(std::string filename, std::string var_name)
{
    this->dataFile->loadFromFile(filename, var_name);
    //this is slooooow :(
    this->dataFile->calculateStatistics();
    //this->dataFile->printStatistics();
}

}

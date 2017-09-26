#include "Volume.h"
#include "DataFile.h"
#include "TransferFunction.h"

#include <vector>

#include <ospray/ospray.h>

namespace pbnj {

Volume::Volume(std::string filename, int x, int y, int z, bool memmap)
{
    this->ID = createID();
    //volumes contain a datafile
    //one datafile per volume, one volume per renderer/camera
    this->dataFile = new DataFile(x, y, z);
    this->loadFromFile(filename, "", memmap);

    this->init();
}

Volume::Volume(std::string filename, std::string var_name, int x, int y, int z,
        bool memmap)
{
    this->ID = createID();
    //volumes contain a datafile
    //one datafile per volume, one volume per renderer/camera
    this->dataFile = new DataFile(x, y, z);
    this->loadFromFile(filename, var_name, memmap);

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

    int dimensions[3] = {this->dataFile->xDim, 
                        this->dataFile->yDim,
                        this->dataFile->zDim};
    float center[3] = {-dimensions[0]/(float)2.0,
                      -dimensions[1]/(float)2.0,
                      -dimensions[2]/(float)2.0};
    float voxelRange[3] = {this->dataFile->minVal, 
                          this->dataFile->maxVal};

    // There is a memory leak here caused by OSPRay
    // more info in destructor
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
    ospRemoveParam(this->oVolume, "voxelData");
    ospRemoveParam(this->oVolume, "dimensions");
    ospRemoveParam(this->oVolume, "voxelType");
    ospRemoveParam(this->oVolume, "voxelRange");
    ospRemoveParam(this->oVolume, "gridOrigin");
    ospRemoveParam(this->oVolume, "transferFunction");
    ospRelease(this->oVolume);
    ospRelease(this->oData);
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

void Volume::loadFromFile(std::string filename, std::string var_name,
        bool memmap)
{
    this->dataFile->loadFromFile(filename, var_name, memmap);
    //this is slooooow :(
    this->dataFile->calculateStatistics();
    //this->dataFile->printStatistics();
}

}

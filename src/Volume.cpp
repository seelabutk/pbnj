#include "Volume.h"
#include "DataFile.h"
#include "TransferFunction.h"

#include <vector>

namespace pbnj {

Volume::Volume(std::string filename, int x, int y, int z)
{
    this->dataFile = new DataFile(x, y, z);
    this->loadFromFile(filename);

    this->transferFunction = new TransferFunction();
    this->transferFunction->setRange(this->dataFile->minVal,
                                     this->dataFile->maxVal);

    this->oVolume = ospNewVolume("shared_structured_volume");
    this->oData = ospNewData(this->dataFile->numValues, OSP_FLOAT, 
            this->dataFile->data, OSP_DATA_SHARED_BUFFER);

    ospSetData(this->oVolume, "voxelData", this->oData);
    ospSet3iv(this->oVolume, "dimensions", (int[]){this->dataFile->xDim,
                                                   this->dataFile->yDim,
                                                   this->dataFile->zDim});
    ospSetString(this->oVolume, "voxelType", "float");
    ospSet2fv(this->oVolume, "voxelRange", (float[]){this->dataFile->minVal,
                                                     this->dataFile->maxVal});
    ospSetObject(this->oVolume, "transferFunction",
            this->transferFunction->asOSPObject());
    ospCommit(this->oVolume);
}

void Volume::attenuateOpacity(float amount)
{
    this->transferFunction->attenuateOpacity(amount);
}

void Volume::setColorMap(std::vector<float> &map)
{
    this->transferFunction->setColorMap(map);
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

void Volume::loadFromFile(std::string filename)
{
    this->dataFile->loadFromFile(filename);
    this->dataFile->calculateStatistics();
}

}

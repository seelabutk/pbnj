#include "Volume.h"
#include "DataFile.h"
#include "TransferFunction.h"

namespace pbnj {

Volume::Volume(DataFile *df) :
    dataFile(df)
{
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

}

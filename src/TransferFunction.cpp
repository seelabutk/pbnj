#include "TransferFunction.h"

#include <iostream>

namespace pbnj {

TransferFunction::TransferFunction()
{
    for(int i = 0; i < 256; i++) {
        this->colorMap[i*3+0] = i;
        this->colorMap[i*3+1] = i;
        this->colorMap[i*3+2] = i;
        this->opacityMap[i]   = i;
    }

    this->oTF = ospNewTransferFunction("piecewise_linear");
    this->oColorData = ospNewData(256, OSP_FLOAT3, this->colorMap);
    this->oOpacityData = ospNewData(256, OSP_FLOAT, this->opacityMap);
    ospSetData(this->oTF, "colors", this->oColorData);
    ospSetData(this->oTF, "opacities", this->oOpacityData);
    ospCommit(this->oTF);
}

void TransferFunction::setRange(float minimum, float maximum)
{
    if(minimum > maximum) {
        std::cerr << "Minimum is larger than maximum!" << std::endl;
        return;
    }

    this->minVal = minimum;
    this->maxVal = maximum;

    ospSet2fv(this->oTF, "valueRange", (float[]){this->minVal, this->maxVal});
    ospCommit(this->oTF);
}

OSPTransferFunction TransferFunction::asOSPObject()
{
    return this->oTF;
}
}

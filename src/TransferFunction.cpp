#include "TransferFunction.h"

#include <iostream>
#include <vector>

namespace pbnj {

TransferFunction::TransferFunction()
{
    this->colorMap.reserve(256*3);
    this->opacityMap.reserve(256);

    for(int i = 0; i < 256; i++) {
        this->colorMap.push_back(i/255.0);
        this->colorMap.push_back(i/255.0);
        this->colorMap.push_back(i/255.0);
        this->opacityMap.push_back(i/255.0);
    }

    this->oTF = ospNewTransferFunction("piecewise_linear");
    this->oColorData = ospNewData(this->colorMap.size()/3, OSP_FLOAT3,
            this->colorMap.data());
    this->oOpacityData = ospNewData(this->colorMap.size(), OSP_FLOAT,
            this->opacityMap.data());
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

void TransferFunction::attenuateOpacity(float amount)
{
    if(amount >= 1.0)
        return;
    for(int i = 0; i < this->opacityMap.size(); i++)
        this->opacityMap[i] = this->opacityMap[i] * amount;

    this->oOpacityData = ospNewData(256, OSP_FLOAT, this->opacityMap.data());
    ospSetData(this->oTF, "opacities", this->oOpacityData);
    ospCommit(this->oTF);
}

OSPTransferFunction TransferFunction::asOSPObject()
{
    return this->oTF;
}

void TransferFunction::setColorMap(std::vector<float> &map)
{
    this->colorMap.clear();
    this->colorMap.reserve(map.size());

    for(int i = 0; i < map.size(); i++)
        this->colorMap[i] = map[i];

    this->oColorData = ospNewData(256, OSP_FLOAT3, this->colorMap.data());
    ospSetData(this->oTF, "colors", this->oColorData);
    ospCommit(this->oTF);
}

}

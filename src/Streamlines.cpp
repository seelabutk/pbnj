#include "Streamlines.h"
#include "StreamlinesDataFile.h"
#include "TransferFunction.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

#include <ospray/ospray.h>

namespace pbnj {

Streamlines::Streamlines(std::string filename, float radius)
{
    this->dataFile = new StreamlinesDataFile();
    this->dataFile->loadFromFile(filename);

    this->oStreamlines = ospNewGeometry("streamlines");
    OSPData vertexDataArray = ospNewData(this->dataFile->numVertices,
            OSP_FLOAT3A, this->dataFile->getVertexData());
    OSPData indexDataArray = ospNewData(this->dataFile->numIndices,
            OSP_INT, this->dataFile->getIndexData());
    ospCommit(vertexDataArray);
    ospCommit(indexDataArray);
    ospSetData(this->oStreamlines, "vertex", vertexDataArray);
    ospSetData(this->oStreamlines, "index", indexDataArray);

    if(this->dataFile->hasData) {
        // handle extra data
        float *colorData = (float *)malloc(this->dataFile->extraData.size()*4*sizeof(float));
        auto range = std::minmax_element(this->dataFile->extraData.begin(),
                this->dataFile->extraData.end());
        float minData = *range.first, maxData = *range.second;
        std::cerr << "DEBUG: extra data min: " << minData << " max: " << maxData << std::endl;
        for(int dIndex = 0; dIndex < this->dataFile->extraData.size(); dIndex++) {
            float value = this->dataFile->extraData[dIndex];
            float gray = (value - minData) / (maxData - minData);
            int cmapIndex = (int)(gray * colormaps["inferno"].size()/3);
            colorData[dIndex*4 + 0] = colormaps["inferno"][cmapIndex*3 + 0];
            colorData[dIndex*4 + 1] = colormaps["inferno"][cmapIndex*3 + 1];
            colorData[dIndex*4 + 2] = colormaps["inferno"][cmapIndex*3 + 2];
            colorData[dIndex*4 + 3] = 1.f;
        }
        OSPData colorDataArray = ospNewData(this->dataFile->numIndices,
            OSP_FLOAT4, colorData);
        ospCommit(colorDataArray);
        ospSetData(this->oStreamlines, "vertex.color", colorDataArray);
    }

    ospSet1i(this->oStreamlines, "smooth", 1);
    ospSet1f(this->oStreamlines, "radius", radius);
    ospCommit(this->oStreamlines);
}

Streamlines::~Streamlines()
{
    ospRemoveParam(this->oStreamlines, "vertex");
    ospRemoveParam(this->oStreamlines, "index");
    ospRelease(this->oStreamlines);
}

OSPGeometry Streamlines::asOSPRayObject()
{
    return this->oStreamlines;
}

bool Streamlines::isSurface()
{
    return true;
}

std::string Streamlines::getRenderType()
{
    return "streamlines";
}

}

#include "Streamlines.h"
#include "StreamlinesDataFile.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <ospray/ospray.h>

namespace pbnj {

Streamlines::Streamlines(std::string filename)
{
    this->dataFile = new StreamlinesDataFile();
    this->dataFile->loadFromFile(filename);

    this->oStreamlines = ospNewGeometry("streamlines");
    OSPData vertexDataArray = ospNewData(this->dataFile->numVertices,
            OSP_FLOAT3A, this->dataFile->getVertexData());
    OSPData indexDataArray = ospNewData(this->dataFile->numIndices,
            OSP_FLOAT, this->dataFile->getIndexData());

    /*
    osp::vec4f color;
    color.x = 0.5; color.y = 0.5; color.z = 0.5; color.w = 1.0;
    std::vector<osp::vec4f> colorData;
    for(int i = 0; i < this->dataFile->numVertices; i++) {
        colorData.push_back(color);
    }

    OSPData colorDataArray = ospNewData(this->dataFile->numVertices,
            OSP_FLOAT4, colorData.data());
    */

    float radius = 0.0015 * *std::max_element(this->dataFile->extents.begin(),
            this->dataFile->extents.end());

    ospSetData(this->oStreamlines, "vertex", vertexDataArray);
    ospSetData(this->oStreamlines, "index", indexDataArray);
    //ospSetData(this->oStreamlines, "vertex.color", colorDataArray);
    ospSet1i(this->oStreamlines, "smooth", 1);
    ospSet1f(this->oStreamlines, "radius", radius);
    ospCommit(this->oStreamlines);

    std::cerr << "DEBUG: created streamlines" << std::endl;
    std::cerr << "       filename: " << filename << std::endl;
    std::cerr << "       bounds: ";
    for(auto b : this->getBounds()) { std::cerr << b << " "; }
    std::cerr << std::endl;
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

std::vector<float> Streamlines::getCenter()
{
    return std::vector<float> {this->dataFile->centerX,
        this->dataFile->centerY, this->dataFile->centerZ};
}

}

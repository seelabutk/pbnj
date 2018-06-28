#include "Streamlines.h"
#include "StreamlinesDataFile.h"

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

    ospSetData(this->oStreamlines, "vertex", vertexDataArray);
    ospSetData(this->oStreamlines, "index", indexDataArray);
    ospSet1i(this->oStreamlines, "smooth", 1);
    ospSet1f(this->oStreamlines, "radius", 0.001);
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

}

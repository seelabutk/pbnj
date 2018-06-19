#include "Slices.h"
#include "Volume.h"

#include <iostream>
#include <vector>

namespace pbnj {

/* normals and planes describe 1 or more normal vectors and points on a plane,
   respectively
   they should be in x,y,z, x,y,z, ... format
   the number of normal vectors and points determines how many slices will be added
   i.e. a single Slices object can be used for multiple slice planes */
Slices::Slices(Volume *v, std::vector<float> &normals, std::vector<float> &points)
{
    this->oSlices = NULL;

    // create (a, b, c, d) tuples for each plane
    std::vector<float> planes;
    for(int i = 0; i < normals.size()/3; i++) {
        float a = normals[i*3 + 0], b = normals[i*3 + 1], c = normals[i*3 + 2];
        float d = -a*points[i*3 + 0] - b*points[i*3 + 1] - c*points[i*3 + 2];
        planes.push_back(a);
        planes.push_back(b);
        planes.push_back(c);
        planes.push_back(d);
    }

    this->oSlices = ospNewGeometry("slices");
    OSPData planeDataArray = ospNewData(planes.size()/4, OSP_FLOAT4, planes.data());
    ospSetData(this->oSlices, "planes", planeDataArray);
    ospSetObject(this->oSlices, "volume", v->asOSPRayObject());
    ospCommit(this->oSlices);
}

Slices::~Slices()
{
    ospRemoveParam(this->oSlices, "planes");
    ospRemoveParam(this->oSlices, "volume");
    ospRelease(this->oSlices);
}

OSPGeometry Slices::asOSPRayObject()
{
    return this->oSlices;
}

}

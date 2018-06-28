#include "BoundingBox.h"

#include <algorithm>
#include <string>
#include <vector>

namespace pbnj {

BoundingBox::BoundingBox(std::vector<long unsigned int> bounds)
{
    float color[] = {1.0, 1.0, 1.0};
    float radius = std::min(std::min(bounds[0], bounds[1]), bounds[2]) * 0.0025f;

    float halves[] = {0, 0, 0};
    for(int i = 0; i < bounds.size(); i++) {
        halves[i] = bounds[i] / 2.0;
    }

    float startEndVertices[] = {
        -halves[0], -halves[1], -halves[2],
        -halves[0], -halves[1], halves[2],

        -halves[0], -halves[1], halves[2],
        halves[0], -halves[1], halves[2],

        halves[0], -halves[1], halves[2],
        halves[0], -halves[1], -halves[2],

        halves[0], -halves[1], -halves[2],
        -halves[0], -halves[1], -halves[2],

        -halves[0], -halves[1], -halves[2],
        -halves[0], halves[1], -halves[2],

        -halves[0], -halves[1], halves[2],
        -halves[0], halves[1], halves[2],

        halves[0], -halves[1], halves[2],
        halves[0], halves[1], halves[2],

        halves[0], -halves[1], -halves[2],
        halves[0], halves[1], -halves[2],

        -halves[0], halves[1], -halves[2],
        -halves[0], halves[1], halves[2],

        -halves[0], halves[1], halves[2],
        halves[0], halves[1], halves[2],

        halves[0], halves[1], halves[2],
        halves[0], halves[1], -halves[2],

        halves[0], halves[1], -halves[2],
        -halves[0], halves[1], -halves[2]
    };
    float pointVertices[] = {
        halves[0], halves[1], halves[2],
        halves[0], halves[1], -halves[2],
        halves[0], -halves[1], halves[2],
        halves[0], -halves[1], -halves[2],
        -halves[0], halves[1], halves[2],
        -halves[0], halves[1], -halves[2],
        -halves[0], -halves[1], halves[2],
        -halves[0], -halves[1], -halves[2]
    };

    this->oCylinders = ospNewGeometry("cylinders");
    OSPData cylinderDataArray = ospNewData(72, OSP_FLOAT, startEndVertices);
    ospSetData(this->oCylinders, "cylinders", cylinderDataArray);
    ospSet1i(this->oCylinders, "bytes_per_cylinder", 24);
    ospSet3fv(this->oCylinders, "color", color);
    ospSet1f(this->oCylinders, "radius", radius);
    ospCommit(this->oCylinders);
}

BoundingBox::~BoundingBox()
{
    ospRemoveParam(this->oCylinders, "bytes_per_cylinder");
    ospRemoveParam(this->oCylinders, "radius");
    ospRemoveParam(this->oCylinders, "color");
    ospRemoveParam(this->oCylinders, "cylinders");
    ospRelease(this->oCylinders);
}

OSPGeometry BoundingBox::asOSPRayObject()
{
    return this->oCylinders;
}

bool BoundingBox::isSurface()
{
    return true;
}

std::string BoundingBox::getRenderType()
{
    return "boundingbox";
}

}

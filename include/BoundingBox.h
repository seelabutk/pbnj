#ifndef PBNJ_BOUNDINGBOX_H
#define PBNJ_BOUNDINGBOX_H

#include <pbnj.h>

#include <vector>

#include <ospray/ospray.h>

namespace pbnj {
    class BoundingBox {
        public:
            BoundingBox(std::vector<long unsigned int> bounds);
            ~BoundingBox();

            OSPGeometry asOSPRayObject();

        private:
            OSPGeometry oCylinders;
    };
}

#endif

#ifndef PBNJ_BOUNDINGBOX_H
#define PBNJ_BOUNDINGBOX_H

#include <pbnj.h>
#include <Subject.h>

#include <string>
#include <vector>

#include <ospray/ospray.h>

namespace pbnj {
    class BoundingBox : public Subject {
        public:
            BoundingBox(std::vector<long unsigned int> bounds);
            ~BoundingBox();

            OSPGeometry asOSPRayObject();
            bool isSurface();
            std::string getRenderType();

        private:
            OSPGeometry oCylinders;
    };
}

#endif

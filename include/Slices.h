#ifndef PBNJ_SLICE_H
#define PBNJ_SLICE_H

#include <pbnj.h>
#include <Subject.h>
#include <Volume.h>

#include <vector>

#include <ospray/ospray.h>

namespace pbnj {
    class Slices : public Subject {
        public:
            Slices(Volume *v, std::vector<float> &normals,
                    std::vector<float> &points);
            ~Slices();

            OSPGeometry asOSPRayObject();
            bool isSurface();

        private:
            OSPGeometry oSlices;
    };
}

#endif

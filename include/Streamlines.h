#ifndef PBNJ_STREAMLINES_H
#define PBNJ_STREAMLINES_H

#include <pbnj.h>
#include <Subject.h>
#include <StreamlinesDataFile.h>

#include <string>
#include <vector>

#include <ospray/ospray.h>

namespace pbnj {
    class Streamlines : public Subject {
        public:
            Streamlines(std::string filename, float radius=0.5);
            ~Streamlines();

            OSPGeometry asOSPRayObject();
            bool isSurface();
            std::string getRenderType();
            std::string ID;
            void setRadius(float radius);

            std::vector<float> getCenter();
        private:
            OSPGeometry oStreamlines;
            StreamlinesDataFile *dataFile;
    };
}

#endif

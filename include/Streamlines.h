#ifndef PBNJ_STREAMLINES_H
#define PBNJ_STREAMLINES_H

#include <pbnj.h>
#include <Subject.h>
#include <StreamlinesDataFile.h>

#include <string>

#include <ospray/ospray.h>

namespace pbnj {
    class Streamlines : public Subject {
        public:
            Streamlines(std::string filename);
            ~Streamlines();

            OSPGeometry asOSPRayObject();
            bool isSurface();
            std::string ID;
        private:
            OSPGeometry oStreamlines;
            StreamlinesDataFile *dataFile;
    };
}

#endif

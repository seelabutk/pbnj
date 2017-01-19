#ifndef PBNJ_VOLUME_H
#define PBNJ_VOLUME_H

#include <pbnj.h>
#include <DataFile.h>

#include <string>
#include <vector>

#include <ospray/ospray.h>

namespace pbnj {
    
    class Volume {

        public:
            //Volume(DataFile *df);
            Volume(std::string filename, int x, int y, int z);

            void attenuateOpacity(float amount);
            void setColorMap(std::vector<float> &map);
            OSPVolume asOSPRayObject();

        private:
            DataFile *dataFile;
            TransferFunction *transferFunction;

            OSPVolume oVolume;
            OSPData oData;

            void loadFromFile(std::string filename);
    };
}

#endif

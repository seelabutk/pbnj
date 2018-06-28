#ifndef PBNJ_VOLUME_H
#define PBNJ_VOLUME_H

#include <pbnj.h>
#include <Subject.h>
#include <DataFile.h>

#include <string>
#include <vector>

#include <ospray/ospray.h>

namespace pbnj {
    
    class Volume : public Subject {

        public:
            //Volume(DataFile *df);
            Volume(std::string filename, int x, int y, int z,
                    bool memmap=false, unsigned int components=1);
            Volume(std::string filename, std::string var_name, int x, int y,
                    int z, bool memmap=false, unsigned int components=1);
            ~Volume();

            void attenuateOpacity(float amount);
            void setColorMap(std::vector<float> &map);
            void setOpacityMap(std::vector<float> &map);
            std::vector<long unsigned int> getBounds();
            OSPVolume asOSPRayObject();
            bool isSurface();
            std::string getRenderType();

            std::string ID;

        private:
            DataFile *dataFile;
            TransferFunction *transferFunction;

            OSPVolume oVolume;
            OSPData oData;

            void init();
            void loadFromFile(std::string filename, std::string var_name="",
                    bool memmap=false);
    };
}

#endif

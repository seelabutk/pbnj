#ifndef PBNJ_VOLUME_H
#define PBNJ_VOLUME_H

#include <pbnj.h>
#include <DataFile.h>

#include <ospray/ospray.h>

namespace pbnj {
    
    class Volume {

        public:
            Volume(DataFile *df);

            OSPVolume asOSPRayObject();

        private:
            DataFile *dataFile;
            TransferFunction *transferFunction;

            OSPVolume oVolume;
            OSPData oData;
    };
}

#endif

#ifndef PBNJ_TRANSFERFUNCTION_H
#define PBNJ_TRANSFERFUNCTION_H

#include <pbnj.h>

#include <ospray/ospray.h>

namespace pbnj {

    extern float coolToWarm[256*3];
    extern float spectralReverse[256*3];

    class TransferFunction {
        public:
            //creates ramp opacity and black to white color
            TransferFunction();
            //other possibilities:
            // color only
            // opacity only
            // both
            
            // enum for known color maps

            void setRange(float minimum, float maximum);
            void attenuateOpacity(float amount);
            void setColorMap(float (&map)[256*3]);

            OSPTransferFunction asOSPObject();
            
        private:

            float colorMap[256*3];
            float opacityMap[256];
            float minVal;
            float maxVal;

            OSPTransferFunction oTF;
            OSPData oColorData;
            OSPData oOpacityData;
    };
}

#endif

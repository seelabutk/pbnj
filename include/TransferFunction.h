#include <pbnj.h>

#include <ospray/ospray.h>

namespace pbnj {

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
            
        private:

            char colorMap[256*3];
            char opacityMap[256];
            float minVal;
            float maxVal;

            OSPTransferFunction oTF;
            OSPData oColorData;
            OSPData oOpacityData;
    };
}

#ifndef PBNJ_TRANSFERFUNCTION_H
#define PBNJ_TRANSFERFUNCTION_H

#include <pbnj.h>

#include <ospray/ospray.h>

#include <vector>
#include <map>

namespace pbnj {

    // named color maps
    extern std::map<std::string, std::vector<float>> colormaps;

    // named opacity maps
    extern std::vector<float> reverseRamp;
    extern std::vector<float> teeth;
    extern std::vector<float> exponential;
    extern std::vector<float> reverseExponential;
    extern std::vector<float> flat;

    class TransferFunction {
        public:
            //creates ramp opacity and black to white color
            TransferFunction();
            ~TransferFunction();
            //other possibilities:
            // color only
            // opacity only
            // both
            
            // enum for known color maps

            void setRange(float minimum, float maximum);
            void attenuateOpacity(float amount);
            void setColorMap(std::vector<float> &map);
            void setOpacityMap(std::vector<float> &map);

            OSPTransferFunction asOSPObject();
            
        private:

            std::vector<float> colorMap;
            std::vector<float> opacityMap;
            float minVal;
            float maxVal;

            OSPTransferFunction oTF;
            OSPData oColorData;
            OSPData oOpacityData;
    };
}

#endif

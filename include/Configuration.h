#ifndef PBNJ_CONFIGURATION_H
#define PBNJ_CONFIGURATION_H

#include <ConfigReader.h>
#include "rapidjson/document.h"

#include <string>
#include <vector>

namespace pbnj {

    enum CONFSTATE {ERROR_NODATA, ERROR_MULTISET, SINGLE_NOVAR, SINGLE_VAR,
        MULTI_NOVAR, MULTI_VAR};

    enum CONFTYPE {PBNJ_VOLUME, PBNJ_PARTICLES, PBNJ_STREAMLINES,
        PBNJ_ISOSURFACE, PBNJ_SLICES};

    class Configuration {

        public:
            Configuration(rapidjson::Document& json);
            CONFSTATE getConfigState();
            CONFTYPE getConfigType(std::string filename);

            std::string dataFilename;
            std::vector<std::string> globbedFilenames;
            std::string dataVariable;
            int dataXDim;
            int dataYDim;
            int dataZDim;
            unsigned int dataNumComponents;

            int imageWidth;
            int imageHeight;
            std::string imageFilename;
            std::vector<unsigned char> bgColor;

            std::vector<float> colorMap;
            std::vector<float> opacityMap;
            float opacityAttenuation;

            unsigned int samples;

            float cameraX;
            float cameraY;
            float cameraZ;

            float cameraUpX;
            float cameraUpY;
            float cameraUpZ;

            float cameraViewX;
            float cameraViewY;
            float cameraViewZ;

            std::vector<float> isosurfaceValues;
            float specularity;
            float particleRadius;

        private:
            ConfigReader *reader;

            void selectColorMap(std::string userInput);
            void selectOpacityMap(std::string userInput);
    };

}

#endif

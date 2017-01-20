#include "Configuration.h"
#include "ConfigReader.h"
#include "TransferFunction.h"

#include "rapidjson/document.h"

#include <iostream>

namespace pbnj {

Configuration::Configuration(std::string filename) :
    configFilename(filename)
{
    this->reader = new ConfigReader();
    rapidjson::Document json;
    this->reader->parseConfigFile(filename, json);

    if(!json.HasMember("filename"))
        std::cerr << "Data filename is required!" << std::endl;
    else
        this->dataFilename = json["filename"].GetString();

    if(!json.HasMember("dimensions"))
        std::cerr << "Data dimensions are required!" << std::endl;
    else {
        const rapidjson::Value& dataDim = json["dimensions"];
        this->dataXDim = dataDim[0].GetInt();
        this->dataYDim = dataDim[1].GetInt();
        this->dataZDim = dataDim[2].GetInt();
    }

    if(!json.HasMember("imageSize"))
        std::cerr << "Image dimensions are required!" << std::endl;
    else {
        const rapidjson::Value& imageDim = json["imageSize"];
        this->imageWidth = imageDim[0].GetInt();
        this->imageHeight = imageDim[1].GetInt();
    }

    if(!json.HasMember("outputImageFilename"))
        std::cerr << "Image filename is required!" << std::endl;
    else
        this->imageFilename = json["outputImageFilename"].GetString();

    if(json.HasMember("colorMap"))
        this->selectColorMap(json["colorMap"].GetString());

    if(json.HasMember("opacityAttenuation"))
        this->opacityAttenuation = json["opacityAttenuation"].GetFloat();
    else
        this->opacityAttenuation = 1.0;
}

void Configuration::selectColorMap(std::string userInput)
{
    if(userInput.compare("coolToWarm") == 0 ||
       userInput.compare("cool to warm") == 0) {
        this->colorMap = coolToWarm;
    }
    else if(userInput.compare("spectralReverse") == 0 ||
            userInput.compare("spectral reverse") == 0 ||
            userInput.compare("reverse spectral") == 0) {
        this->colorMap = spectralReverse;
    }
    else if(userInput.compare("magma") == 0) {
        this->colorMap = magma;
    }
    else if(userInput.compare("viridis") == 0) {
        this->colorMap = viridis;
    }
    else {
        std::cerr << "Unrecognized color map " << userInput << "!" << std::endl;
    }
}

}

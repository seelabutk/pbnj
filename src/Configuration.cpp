#include "Configuration.h"
#include "ConfigReader.h"

#include "rapidjson/document.h"

namespace pbnj {

Configuration::Configuration(std::string filename) :
    configFilename(filename)
{
    this->reader = new ConfigReader();
    rapidjson::Document json;
    this->reader->parseConfigFile(filename, json);

    this->dataFilename = json["filename"].GetString();
    const rapidjson::Value& dataDim = json["dimensions"];
    this->dataXDim = dataDim[0].GetInt();
    this->dataYDim = dataDim[1].GetInt();
    this->dataZDim = dataDim[2].GetInt();

    const rapidjson::Value& imageDim = json["imageSize"];
    this->imageWidth = imageDim[0].GetInt();
    this->imageHeight = imageDim[1].GetInt();
    this->imageFilename = json["outputImageFilename"].GetString();
}

}

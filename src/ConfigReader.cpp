#include "ConfigReader.h"

#include "rapidjson/document.h"

#include <stdlib.h>
#include <stdio.h>

namespace pbnj {

ConfigReader::ConfigReader()
{
}

void ConfigReader::parseConfigFile(std::string filename, 
        rapidjson::Document& config)
{
    FILE *file = fopen(filename.c_str(), "r");
    fseek(file, 0, SEEK_END);
    long fileLength = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json = (char *)malloc(fileLength + 1);
    fread(json, fileLength, 1, file);
    fclose(file);
    json[fileLength] = 0;

    //rapidjson::Document config;
    config.ParseInsitu(json);
    //return config;
}

}

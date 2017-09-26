#include "ConfigReader.h"

#include "rapidjson/document.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

namespace pbnj {

ConfigReader::ConfigReader()
{
}

void ConfigReader::parseConfigFile(std::string filename, 
        rapidjson::Document& config)
{
    //open file and get its length
    FILE *file = fopen(filename.c_str(), "r");
    fseek(file, 0, SEEK_END);
    long fileLength = ftell(file);
    fseek(file, 0, SEEK_SET);

    //read the data into a string
    char *json = (char *)malloc(fileLength + 1);
    size_t nread = fread(json, 1, fileLength, file);
    if(nread != fileLength) {
        std::cerr << "WARNING: Unexpected number of bytes read from ";
        std::cerr << "configuration file. Read " << nread << " but";
        std::cerr << " should be " << fileLength << std::endl;
    }
    fclose(file);
    json[fileLength] = 0;

    //parse the string in place
    config.ParseInsitu(json);
}

}

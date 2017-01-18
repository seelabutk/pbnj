#ifndef PBNJ_CONFIGREADER_H
#define PBNJ_CONFIGREADER_H

#include <pbnj.h>

#include <string>

#include <rapidjson/document.h>

namespace pbnj {
    
    class ConfigReader {
        public:

            ConfigReader();

            void parseConfigFile(std::string filename,
                    rapidjson::Document& config);
    };

}

#endif

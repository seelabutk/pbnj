#include "StreamlinesDataFile.h"

#include "pugixml/pugixml.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include <string.h>
#include <stdlib.h>

namespace pbnj {

StreamlinesDataFile::StreamlinesDataFile()
    : hasData(false)
{
}

StreamlinesDataFile::~StreamlinesDataFile()
{
}

void StreamlinesDataFile::loadFromFile(std::string filename)
{
    this->filename = filename;
    this->filetype = getFiletype();

    if(this->filetype == UNKNOWN) {
        std::cerr << "Unknown filetype!" << std::endl;
    }
    else if(this->filetype == XML) {
        pugi::xml_document document;
        pugi::xml_parse_result result = document.load_file(filename.c_str());
        if(!result) {
            std::cerr << "Error reading XML file: " << result.description();
            std::cerr << std::endl;
            return;
        }

        // dfs for the first Streamlines node
        //pugi::xml_node sl = searchForStreamlineNode(document);
        pugi::xml_node sl = document.find_node(streamlineNodePredicate());
        if(strcmp(sl.name(), "") == 0) {
            std::cerr << "No Streamlines tag found!" << std::endl;
            return;
        }

        pugi::xml_node vnode = sl.child("vertex");
        pugi::xml_node inode = sl.child("index");
        if(strcmp(vnode.name(), "") == 0) {
            std::cerr << "No vertex tag found!" << std::endl;
            return;
        }
        if(strcmp(inode.name(), "") == 0) {
            std::cerr << "No index tag found!" << std::endl;
            return;
        }

        pugi::xml_node datanode = sl.child("data");
        if(strcmp(datanode.name(), "") != 0) {
            this->hasData = true;
            std::cerr << "DEBUG: found a data tag!" << std::endl;
        }

        const char *cvstring = vnode.child_value();
        char *vstring = (char *)malloc(strlen(cvstring)+1);
        strcpy(vstring, cvstring); // to un-const
        // split vertices
        const char *delims = " \n";
        char *token = strtok(vstring, delims);
        float xMin = std::numeric_limits<float>::max(),
              xMax = std::numeric_limits<float>::min(),
              yMin = std::numeric_limits<float>::max(),
              yMax = std::numeric_limits<float>::min(),
              zMin = std::numeric_limits<float>::max(),
              zMax = std::numeric_limits<float>::min();
        while(token != NULL) {
            osp::vec3fa vert;
            vert.x = atof(token);
            this->checkBounds(xMin, xMax, vert.x);

            token = strtok(NULL, delims);
            vert.y = atof(token);
            this->checkBounds(yMin, yMax, vert.y);

            token = strtok(NULL, delims);
            vert.z = atof(token);
            this->checkBounds(zMin, zMax, vert.z);

            token = strtok(NULL, delims);
            this->vertexData.push_back(vert);
        }

        this->numVertices = this->vertexData.size();
        this->bounds = std::vector<float> {xMin, xMax, yMin, yMax, zMin, zMax};
        this->extents = std::vector<float> {xMax-xMin, yMax-yMin, zMax-zMin};
        this->dataXDim = this->extents[0];
        this->dataYDim = this->extents[1];
        this->dataZDim = this->extents[2];
        this->centerX = this->extents[0]/2 + xMin;
        this->centerY = this->extents[1]/2 + yMin;
        this->centerZ = this->extents[2]/2 + zMin;

        const char *cistring = inode.child_value();
        char *istring = (char *)malloc(strlen(cistring)+1);
        strcpy(istring, cistring); // to un-const
        token = strtok(istring, delims);
        while(token != NULL) {
            this->indexData.push_back(atoi(token));
            token = strtok(NULL, delims);
        }

        this->numIndices = this->indexData.size();

        if(this->hasData) {
            const char *dataName = datanode.first_attribute().value();
            std::cerr << "DEBUG: data node name: " << dataName << std::endl;

            const char *cdatastring = datanode.child_value();
            char *datastring = (char *)malloc(strlen(cdatastring)+1);
            strcpy(datastring, cdatastring); // to un-const
            token = strtok(datastring, delims);
            while(token != NULL) {
                this->extraData.push_back(atof(token));
                token = strtok(NULL, delims);
            }

            std::cerr << "DEBUG: read " << this->extraData.size() << " data values" << std::endl;
        }
    }
    else {
        std::cerr << "Not a PBNJ streamlines capable file!" << std::endl;
    }
}

void StreamlinesDataFile::checkBounds(float &minVal, float &maxVal, float &value)
{
    minVal = std::min(minVal, value);
    maxVal = std::max(minVal, value);
}

pugi::xml_node StreamlinesDataFile::searchForStreamlineNode(pugi::xml_node node)
{
    std::cout << node.name() << std::endl;
    for(pugi::xml_node child : node.children()) {
        if(strcmp(child.name(), "Streamlines") == 0 ||
           strcmp(child.name(), "StreamLines") == 0 ||
           strcmp(child.name(), "streamlines") == 0) {
            return child;
        }
        else {
            return searchForStreamlineNode(child);
        }
    }
}

FILETYPE StreamlinesDataFile::getFiletype()
{
    std::stringstream ss;
    ss.str(this->filename);
    std::string token;
    char delim = '.';
    while(std::getline(ss, token, delim)) {
    }
    
    if(token.compare("xml") == 0 ||
       token.compare("osx") == 0) {
        return XML;
    }
    else {
        return UNKNOWN;
    }
}

osp::vec3fa *StreamlinesDataFile::getVertexData()
{
    return this->vertexData.data();
}

unsigned int *StreamlinesDataFile::getIndexData()
{
    return this->indexData.data();
}

}

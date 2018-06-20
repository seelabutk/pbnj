#include "StreamlinesDataFile.h"

#include "pugixml/pugixml.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <string.h>
#include <stdlib.h>

namespace pbnj {

StreamlinesDataFile::StreamlinesDataFile()
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

        const char *cvstring = vnode.child_value();
        char *vstring = (char *)malloc(strlen(cvstring)+1);
        strcpy(vstring, cvstring); // to un-const
        // split vertices
        const char *delims = " \n";
        char *token = strtok(vstring, delims);
        while(token != NULL) {
            osp::vec3fa vert;
            vert.x = atof(token);
            token = strtok(NULL, delims);
            vert.y = atof(token);
            token = strtok(NULL, delims);
            vert.z = atof(token);
            token = strtok(NULL, delims);
            this->vertexData.push_back(vert);
        }

        this->numVertices = this->vertexData.size();

        const char *cistring = inode.child_value();
        char *istring = (char *)malloc(strlen(cistring)+1);
        strcpy(istring, cistring); // to un-const
        token = strtok(istring, delims);
        while(token != NULL) {
            this->indexData.push_back(atoi(token));
            token = strtok(NULL, delims);
        }

        this->numIndices = this->indexData.size();
    }
    else {
        std::cerr << "Not a PBNJ streamlines capable file!" << std::endl;
    }
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

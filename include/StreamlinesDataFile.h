#ifndef PBNJ_STREAMLINESDATAFILE_H
#define PBNJ_STREAMLINESDATAFILE_H

#include "DataFile.h" // for filetype enum

#include "pugixml/pugixml.hpp"

#include <string>
#include <vector>

#include <string.h>

namespace pbnj {

    class StreamlinesDataFile {
        public:
            StreamlinesDataFile();
            ~StreamlinesDataFile();

            void loadFromFile(std::string filename);

            osp::vec3fa *getVertexData();
            unsigned int *getIndexData();

            unsigned int numVertices;
            unsigned int numIndices;
        private:
            std::vector<osp::vec3fa> vertexData;
            std::vector<unsigned int> indexData;

            FILETYPE getFiletype();
            FILETYPE filetype;
            std::string filename;

            pugi::xml_node searchForStreamlineNode(pugi::xml_node node);

            struct streamlineNodePredicate {
                bool operator()(pugi::xml_node node) const {
                    return (strcmp(node.name(), "Streamlines") == 0 ||
                            strcmp(node.name(), "StreamLines") == 0 ||
                            strcmp(node.name(), "streamlines") == 0);
                }
            };
    };

}

#endif

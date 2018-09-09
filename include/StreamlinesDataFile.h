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

            std::vector<float> bounds;
            std::vector<float> extents;
            float dataXDim;
            float dataYDim;
            float dataZDim;
            float centerX;
            float centerY;
            float centerZ;

        private:
            std::vector<osp::vec3fa> vertexData;
            std::vector<unsigned int> indexData;

            FILETYPE getFiletype();
            FILETYPE filetype;
            std::string filename;

            void checkBounds(float &minVal, float &maxVal, float &value);

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

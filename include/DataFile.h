#include <string>

#include <pbnj.h>

namespace pbnj {

    enum FILETYPE {UNKNOWN, BINARY};

    class DataFile {

        public:
            DataFile(int x, int y, int z);
            ~DataFile();

            void loadFromFile(std::string filename);

            std::string filename;
            FILETYPE filetype;

            int xDim;
            int yDim;
            int zDim;

            float minVal; // these should 
            float maxVal; // eventually be 
            float *data;  // template types

        private:
            FILETYPE getFiletype();
    };

}

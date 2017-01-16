#include <string>

#include <pbnj.h>

namespace pbnj {

    enum FILETYPE {UNKNOWN, BINARY};

    class DataFile {

        public:
            DataFile(int x, int y, int z);
            ~DataFile();

            void loadFromFile(std::string filename);
            void calculateStatistics();
            void printStatistics();

            std::string filename;
            FILETYPE filetype;

            int xDim;
            int yDim;
            int zDim;

            float minVal; // these should 
            float maxVal; //
            float avgVal; // eventually be 
            float stdDev; //
            float *data;  // template types

            bool statsCalculated;

        private:
            FILETYPE getFiletype();
    };

}

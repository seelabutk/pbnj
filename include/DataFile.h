#ifndef PBNJ_DATAFILE_H
#define PBNJ_DATAFILE_H

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

            // experimental
            void bin(unsigned int num_bins);

            std::string filename;
            FILETYPE filetype;

            int xDim;
            int yDim;
            int zDim;
            int numValues;

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

#endif

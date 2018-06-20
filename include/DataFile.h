#ifndef PBNJ_DATAFILE_H
#define PBNJ_DATAFILE_H

#include <string>

#include <pbnj.h>

namespace pbnj {

    // TODO this should be moved out of DataFile
    enum FILETYPE {UNKNOWN, BINARY, NETCDF, PARTICLE, XML};

    class DataFile {

        public:
            DataFile(int x, int y, int z, unsigned int components=1);
            ~DataFile();

            void loadFromFile(std::string filename, std::string variable="",
                    bool memmap=false);
            void calculateStatistics();
            void printStatistics();

            // experimental
            void bin(unsigned int num_bins);

            std::string filename;
            FILETYPE filetype;

            unsigned long int xDim;
            unsigned long int yDim;
            unsigned long int zDim;
            unsigned long int numValues;
            unsigned int numComponents;

            float minVal; // these should 
            float maxVal; //
            float avgVal; // eventually be 
            float stdDev; //
            float *data;  // template types

            bool statsCalculated;

        private:
            FILETYPE getFiletype();
            bool wasMemoryMapped;
    };

}

#endif

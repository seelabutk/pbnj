#ifndef PBNJ_TIMESERIES_H
#define PBNJ_TIMESERIES_H

#include "Volume.h"

#include <string>
#include <vector>

namespace pbnj {

    class TimeSeries {

        public:
            TimeSeries(std::vector<std::string> filenames, int x, int y, int z);
            TimeSeries(std::vector<std::string> filenames, std::string varname,
                    int x, int y, int z);
            ~TimeSeries();

            Volume *getVolume(unsigned int index);
            unsigned int getLength();

        private:
            int xDim;
            int yDim;
            int zDim;
            unsigned int length;
            std::vector<std::string> dataFilenames;
            std::string dataVariable;
            Volume **volumes;
    };

}

#endif

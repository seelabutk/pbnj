#ifndef PBNJ_TIMESERIES_H
#define PBNJ_TIMESERIES_H

#include "Volume.h"

#include <list>
#include <string>
#include <sys/sysinfo.h>
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
            void setMaxMemory(unsigned int gigabytes);

            // attributes for volumes to receive when loaded
            std::vector<float> colorMap;
            std::vector<float> opacityMap;
            float opacityAttenuation;
            bool doMemoryMap;

            void setColorMap(std::vector<float> &map);
            void setOpacityMap(std::vector<float> &map);
            void setOpacityAttenuation(float attenuation);
            void setMemoryMapping(bool toMMap);

        private:
            int xDim;
            int yDim;
            int zDim;
            unsigned int dataSize;
            unsigned int maxVolumes;
            unsigned int currentVolumes;
            std::list<int> lruCache;
            void encache(unsigned int index);

            unsigned int length;
            std::vector<std::string> dataFilenames;
            std::string dataVariable;
            Volume **volumes;

            struct sysinfo systemInfo;
            void initSystemInfo();
    };

}

#endif

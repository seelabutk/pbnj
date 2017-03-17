#include "pbnj.h"
#include "Camera.h"
#include "Configuration.h"
#include "Renderer.h"
#include "TimeSeries.h"
#include "TransferFunction.h"
#include "Volume.h"

#include <iostream>
#include <sys/sysinfo.h>
#include <queue>
#include <string>
#include <cmath>

float PI = 3.141592;

int main(int argc, const char **argv)
{
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.json>" << std::endl;
        return 1;
    }

    pbnj::Configuration *config = new pbnj::Configuration(argv[1]);
    pbnj::pbnjInit(&argc, argv);

    pbnj::TimeSeries *timeSeries;
    pbnj::Volume *volume;
    pbnj::Camera *camera;

    int state = config->getConfigState();
    if(state == pbnj::MULTI_NOVAR) {
        timeSeries = new pbnj::TimeSeries(config->globbedFilenames,
                config->dataXDim, config->dataYDim, config->dataZDim);
    }
    else if(state == pbnj::MULTI_VAR) {
        timeSeries = new pbnj::TimeSeries(config->globbedFilenames,
                config->dataVariable, config->dataXDim, config->dataYDim,
                config->dataZDim);
    }
    else {
        std::cerr << "This example tests time series data, not single volumes";
        std::cerr << std::endl;
        return 1;
    }

    timeSeries->setMaxMemory(2);

    for(int i = 0; i < 15; i++) {
        volume = timeSeries->getVolume(i);
    }

    delete config;
    delete timeSeries;

    return 0;
}

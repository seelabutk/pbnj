#include "pbnj.h"
#include "Camera.h"
#include "Configuration.h"
#include "Renderer.h"
#include "TimeSeries.h"
#include "TransferFunction.h"
#include "Volume.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

std::string imageFamily(std::string filename, unsigned int count)
{
    std::string::size_type index;
    index = filename.rfind('.');
    std::string base = filename.substr(0, index);
    std::string family = std::to_string(count);
    family.insert(0, 4 - family.length(), '0');
    std::string ext = filename.substr(index);
    return base + family + ext;
}

int main(int argc, const char **argv)
{
    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.json>" << std::endl;
        return 1;
    }

    pbnj::ConfigReader *reader = new pbnj::ConfigReader();
    rapidjson::Document json;
    reader->parseConfigFile(argv[1], json);
    pbnj::Configuration *config = new pbnj::Configuration(json);

    pbnj::pbnjInit(&argc, argv);

    // there are 4 possible valid cases for data in the config file:
    //  - single volume without variable
    //  - single volume with variable
    //  - multiple volumes without variable
    //  - multiple volumes with variable
    pbnj::Volume *volume;
    pbnj::TimeSeries *timeSeries;
    bool single = true;

    // there are 2 invalid cases:
    //  - no data filename(s) provided
    //  - both a single and multiple filenames provided
    pbnj::CONFSTATE confState = config->getConfigState();
    switch(confState) {
        case pbnj::ERROR_NODATA:
            std::cerr << "ERROR: No data filename(s) provided";
            std::cerr << std::endl;
            break;
        case pbnj::ERROR_MULTISET:
            std::cerr << "ERROR: Multiple filename types requested";
            std::cerr << "requested" << std::endl;
            break;
        case pbnj::SINGLE_NOVAR:
            std::cout << "Single volume, no variable" << std::endl;
            volume = new pbnj::Volume(config->dataFilename, config->dataXDim,
                    config->dataYDim, config->dataZDim, true);
            break;
        case pbnj::SINGLE_VAR:
            std::cout << "Single volume, variable" << std::endl;
            volume = new pbnj::Volume(config->dataFilename,
                    config->dataVariable, config->dataXDim, config->dataYDim,
                    config->dataZDim);
            break;
        case pbnj::MULTI_NOVAR:
            std::cout << "Multiple volumes, no variable" << std::endl;
            timeSeries = new pbnj::TimeSeries(config->globbedFilenames,
                    config->dataXDim, config->dataYDim, config->dataZDim);
            timeSeries->setColorMap(config->colorMap);
            timeSeries->setOpacityMap(config->opacityMap);
            timeSeries->setOpacityAttenuation(config->opacityAttenuation);
            timeSeries->setMemoryMapping(true);
            single = false;
            break;
        case pbnj::MULTI_VAR:
            std::cout << "Multiple volumes, variable" << std::endl;
            timeSeries = new pbnj::TimeSeries(config->globbedFilenames,
                    config->dataVariable, config->dataXDim, config->dataYDim,
                    config->dataZDim);
            timeSeries->setColorMap(config->colorMap);
            timeSeries->setOpacityMap(config->opacityMap);
            timeSeries->setOpacityAttenuation(config->opacityAttenuation);
            timeSeries->setMemoryMapping(true);
            single = false;
    }

    // set up the camera
    pbnj::Camera *camera = new pbnj::Camera(config->imageWidth, 
            config->imageHeight);
    camera->setPosition(config->cameraX, config->cameraY, config->cameraZ);
    camera->setUpVector(config->cameraUpX, config->cameraUpY,
            config->cameraUpZ);
    camera->centerView();

    // set up the renderer
    // the samples and camera are the same regardless of whether we are
    // rendering a single volume or a time series
    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setSamples(config->samples);
    renderer->setBackgroundColor(config->bgColor);
    renderer->setCamera(camera);

    if(single) {
        // we have a single volume
        // let's take a single picture of it

        // set up any remaining config options for the volume
        volume->setColorMap(config->colorMap);
        volume->setOpacityMap(config->opacityMap);
        volume->attenuateOpacity(config->opacityAttenuation);

        // set up the renderer and get an image
        if(config->isosurfaceValues.size() == 0) {
            renderer->setVolume(volume);
        }
        else {
            renderer->setIsosurface(volume, config->isosurfaceValues,
                    config->specularity);
        }
        auto begin = std::chrono::high_resolution_clock::now();
        renderer->renderImage(config->imageFilename);
        auto end = std::chrono::high_resolution_clock::now();
        unsigned long int duration = std::chrono::duration_cast
            <std::chrono::nanoseconds>(end - begin).count();

        //std::cout << "Rendered image to " << config->imageFilename << std::endl;
        std::cout << config->imageWidth << " ";
        std::cout << config->opacityAttenuation << " ";
        std::cout << config->samples << " ";
        std::cout << std::setprecision(6)  << duration/1e9 << " s";
        std::cout << std::endl;
    }
    else {
        // we have a series of volumes
        // render an image of each one sequentially
        for(int v = 0; v < timeSeries->getLength(); v++) {
            // get the "current" volume
            volume = timeSeries->getVolume(v);
            //volume->setColorMap(config->colorMap);
            //volume->setOpacityMap(config->opacityMap);
            //volume->attenuateOpacity(config->opacityAttenuation);

            // modify the config filename so we have a family of images
            std::string imageFilename = imageFamily(config->imageFilename, v);

            // set the current volume as the one to render
            // this erases the last volume in the renderer
            renderer->setVolume(volume);
            renderer->renderImage(imageFilename);

            std::cout << "Rendered image to " << imageFilename << std::endl;
        }
    }

    return 0;
}

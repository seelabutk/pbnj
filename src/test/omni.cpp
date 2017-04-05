#include "pbnj.h"
#include "Camera.h"
#include "Configuration.h"
#include "Renderer.h"
#include "TimeSeries.h"
#include "TransferFunction.h"
#include "Volume.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdlib.h>
#include "lodepng/lodepng.h"
#include <string>
#include <vector>

#define PI 3.14158

int main(int argc, const char **argv)
{
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.json>" << std::endl;
        return 1;
    }

    std::string confFile(argv[1]);
    std::string confName = confFile.substr(confFile.rfind('/')+1);
    confName = confName.substr(0, confName.rfind('.'));

    pbnj::Configuration *config = new pbnj::Configuration(argv[1]);

    pbnj::pbnjInit(&argc, argv);

    pbnj::CONFSTATE confState = config->getConfigState();
    if(confState != pbnj::SINGLE_NOVAR && confState != pbnj::SINGLE_VAR) {
        std::cerr << "Multiple time steps not supported" << std::endl;
        return 1;
    }

    float degreeStep = 0.25;
    unsigned int renderWidth = 64, renderHeight = 64;
    unsigned int outputWidth = (unsigned int) 360/degreeStep;
    unsigned int outputHeight = (unsigned int) 180/degreeStep;
    unsigned char *output = (unsigned char *)calloc(4*outputWidth*outputHeight, 1);
    float currentPhi = -90, currentTheta = 0;
    float radius = (float) sqrt(config->dataXDim * config->dataXDim +
                                config->dataYDim * config->dataYDim +
                                config->dataZDim * config->dataZDim);
    float camx = 0, camy = radius, camz = 0;

    pbnj::Volume *volume = new pbnj::Volume(config->dataFilename,
            config->dataVariable, config->dataXDim, config->dataYDim,
            config->dataZDim);
    volume->setColorMap(config->colorMap);
    volume->setOpacityMap(config->opacityMap);
    volume->attenuateOpacity(config->opacityAttenuation);

    pbnj::Camera *camera = new pbnj::Camera(renderWidth, renderHeight);
    camera->setUpVector(0, 1, 0);

    pbnj::Renderer *renderer = new pbnj::Renderer();
    renderer->setBackgroundColor(config->bgColor);
    renderer->setVolume(volume);

    unsigned long int numPixels = outputWidth * outputHeight;
    std::cout << outputWidth << "x" << outputHeight << "=" << numPixels;
    std::cout << std::endl;

    for(int i = 0; i < outputWidth; i++) {
        std::cout << '\r' << /*std::setprecision(2) <<*/ std::setw(5);
        std::cout << 100 * (i/(float)outputWidth) << "%";
        std::cout << " " << (360.0 * i / outputWidth) << "     ";
        std::cout << std::flush;
        for(int j = 0; j < outputHeight; j++) {
            // my conventions for these match neither math nor physics >_>
            float currentPhi = (180.0 * j / outputHeight) - 90; // x-axis angle
            float currentTheta = (360.0 * i / outputWidth); // y-axis angle
            camx = radius*sin(currentTheta*PI/180)*cos(currentPhi*PI/180);
            camy = radius*sin(currentPhi*PI/180);
            camz = radius*cos(currentTheta*PI/180)*cos(currentPhi*PI/180);

            camera->setPosition(camx, camy, camz);
            renderer->setCamera(camera);

            unsigned char *buffer;
            renderer->renderToBuffer(&buffer);

            output[i*4 + j*outputWidth*4 + 0] = buffer[4*renderWidth/2 + 4*renderWidth*renderHeight/2 + 0];
            output[i*4 + j*outputWidth*4 + 1] = buffer[4*renderWidth/2 + 4*renderWidth*renderHeight/2 + 1];
            output[i*4 + j*outputWidth*4 + 2] = buffer[4*renderWidth/2 + 4*renderWidth*renderHeight/2 + 2];
            output[i*4 + j*outputWidth*4 + 3] = 255;
        }
    }
    std::cout << std::endl;

    std::vector<unsigned char> png;
    unsigned int error = lodepng::encode(png, output, outputWidth, outputHeight);
    if(error) {
        std::cerr << "LodePNG had an error" << std::endl;
        return 1;
    }
    lodepng::save_file(png, "omni_"+confName+".png");

    // save to lodepng

    return 0;

}

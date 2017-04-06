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

void createOmni(pbnj::Volume *volume, pbnj::Renderer *renderer, 
        pbnj::Camera *camera, pbnj::Configuration *config, std::string name, 
        unsigned int renderWidth, unsigned int renderHeight)
{
    float radius = (float) sqrt(config->dataXDim * config->dataXDim +
                                config->dataYDim * config->dataYDim +
                                config->dataZDim * config->dataZDim);
    float camera_distance = 0.0;
    float fov = 1.0;
    float near = 1e-6;
    float r = near / std::sin(fov * PI / 360.0);
    float r_prime = radius + camera_distance - near;
    float t = fov / r; // in radians
    float c_w = 2 * r * std::sin(t/2.0) * renderWidth;
    float px_w = 1 / c_w * r; // pixel size in world unit on the screen space
    // px_w should be equal to the pixel size in images space at this point

    float px_obj = px_w / r_prime; // in radians
    float angle_of_rotation = px_obj * 180 / PI; // in degrees
    std::cout<<"Angle of rotation is "<<angle_of_rotation<<std::endl;

    unsigned int outputWidth = (unsigned int) 360/angle_of_rotation;
    unsigned int outputHeight = (unsigned int) 180/angle_of_rotation;
    unsigned char *output = (unsigned char *) calloc(4*outputWidth*outputHeight, 1);
    float currentPhi = -90, currentTheta = 0;
    float camx = 0, camy = radius, camz = 0;

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

            output[i*4 + j*outputWidth*4 + 0] = buffer[4*(renderWidth/2) + 4*renderWidth*(renderHeight/2) + 0];
            output[i*4 + j*outputWidth*4 + 1] = buffer[4*(renderWidth/2) + 4*renderWidth*(renderHeight/2) + 1];
            output[i*4 + j*outputWidth*4 + 2] = buffer[4*(renderWidth/2) + 4*renderWidth*(renderHeight/2) + 2];
            output[i*4 + j*outputWidth*4 + 3] = 255;
        }
    }
    std::cout << std::endl;

    std::vector<unsigned char> png;
    unsigned int error = lodepng::encode(png, output, outputWidth, outputHeight);
    if (error) 
    {
        std::cerr << "LodePNG had an error" << std::endl;
        return;
    }
    lodepng::save_file(png, "omni_" + name + ".png");

}

int main(int argc, const char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.json>" << std::endl;
        return 1;
    }
    unsigned int renderWidth = 1, renderHeight = 1;

    std::string confFile(argv[1]);
    std::string confName = confFile.substr(confFile.rfind('/')+1);
    confName = confName.substr(0, confName.rfind('.'));
    pbnj::Configuration *config = new pbnj::Configuration(argv[1]);
    pbnj::pbnjInit(&argc, argv);

    pbnj::CONFSTATE confState = config->getConfigState();

    if (confState != pbnj::SINGLE_NOVAR && confState != pbnj::SINGLE_VAR) 
    {
        pbnj::Volume *volume;
        pbnj::TimeSeries *timeSeries;
        timeSeries = new pbnj::TimeSeries(config->globbedFilenames, 
                config->dataVariable, config->dataXDim, config->dataYDim, 
                config->dataZDim);
        timeSeries->setMaxMemory(50);
        timeSeries->setColorMap(config->colorMap);
        timeSeries->setOpacityMap(config->opacityMap);
        timeSeries->setOpacityAttenuation(config->opacityAttenuation);

        pbnj::Camera *camera = new pbnj::Camera(renderWidth, renderHeight);
        camera->setUpVector(0, 1, 0);

        pbnj::Renderer *renderer = new pbnj::Renderer();
        renderer->setBackgroundColor(config->bgColor);

        for (int i = 0; i < timeSeries->getLength(); i++)
        {
            volume = timeSeries->getVolume(i);
            renderer->setVolume(volume);
            createOmni(volume, renderer, camera, config, confName + std::to_string(i), renderWidth, renderHeight);
        }

    }
    else
    {
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

        createOmni(volume, renderer, camera, config, confName, renderWidth, renderHeight);
    }

    return 0;
}

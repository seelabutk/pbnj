#include "pbnj.h"
#include "Camera.h"
#include "Configuration.h"
#include "Renderer.h"
#include "TransferFunction.h"
#include "Volume.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

void print_current_vals(int *imsize, float att, int samp)
{
    std::cout << std::setw(4) << imsize[0] << " x ";
    std::cout << std::setw(4) << imsize[1] << "  ";
    std::cout << std::setw(4) << att << "  ";
    std::cout << samp << "  ";
}

int main(int argc, const char **argv)
{
    // we only need the config file for the dataset
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.json>" << std::endl;
        return 1;
    }

    // pbnj and volume initialization
    pbnj::Configuration *config = new pbnj::Configuration(argv[1]);
    pbnj::pbnjInit(&argc, argv);
    pbnj::Volume *volume = new pbnj::Volume(config->dataFilename,
            config->dataXDim, config->dataYDim, config->dataZDim);

    // benchmark parameters
    int image_sizes[6][2] = {
        {  64,   64},
        { 128,  128},
        { 256,  256},
        { 512,  512},
        {1024, 1024},
        {2048, 2048}
    };
    float attenuations[4] = {1.0, 0.5, 0.1, 0.01};
    int samples[4] = {1, 2, 4, 8};
    int iterations = 10;
    // pseudo-random generators for the camera
    std::random_device rand_device;
    std::mt19937 generator(rand_device());
    std::uniform_int_distribution<> cam_x(-config->dataXDim, config->dataXDim);
    std::uniform_int_distribution<> cam_y(-config->dataYDim, config->dataYDim);
    std::uniform_int_distribution<> cam_z(-config->dataZDim, config->dataZDim);
    // default ramp opacity map to reset the volume's opacity
    std::vector<float> ramp;
    for(int i = 0; i < 256; i++)
        ramp.push_back(i/255.0);
    // open CSV file and write headers to it
    std::ofstream csv;
    csv.open("benchmark_results.csv");
    csv << "width,height,attenuation,samples per pixel,average time for ";
    csv << iterations << " iterations (s)\n";

    // iterate over all benchmarking parameters
    for(int image_index = 0; image_index < 6; image_index++) {
        int *current_image_size = image_sizes[image_index];
        for(int att_index = 0; att_index < 4; att_index++) {
            float current_attenuation = attenuations[att_index];
            for(int samp_index = 0; samp_index < 4; samp_index++) {
                int current_samples = samples[samp_index];
                // output current test to screen and to CSV file
                print_current_vals(current_image_size, current_attenuation,
                        current_samples);
                csv << current_image_size[0] << ",";
                csv << current_image_size[1] << ",";
                csv << current_attenuation << ",";
                csv << current_samples << ",";

                // time the total iterations and get an average
                auto begin = std::chrono::high_resolution_clock::now();
                for(int iter_index = 0; iter_index < iterations; iter_index++) {
                    // set current attenuation and reset after rendering
                    volume->attenuateOpacity(current_attenuation);
                    // setup a random camera
                    pbnj::Camera *camera = new pbnj::Camera(
                            current_image_size[0], current_image_size[1]);
                    camera->setPosition(cam_x(generator), cam_y(generator),
                            cam_z(generator));
                    camera->setUpVector(0, 1, 0);
                    // setup a renderer
                    pbnj::Renderer *renderer = new pbnj::Renderer();
                    renderer->setVolume(volume);
                    renderer->setCamera(camera);
                    renderer->setSamples(current_samples);
                    renderer->render(); // throw away the buffer
                    // reset opacity map
                    volume->setOpacityMap(ramp);
                }
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast
                    <std::chrono::nanoseconds>(end - begin).count();
                // output average time taken
                float seconds = duration / iterations / 1e9;
                std::cout << std::setprecision(6)  << seconds << " s";
                std::cout << std::endl;
                csv << seconds << "\n";
            }
        }
    }

    return 0;
}

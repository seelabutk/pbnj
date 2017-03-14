#include "pbnj.h"
#include "Camera.h"
#include "Configuration.h"
#include "Renderer.h"
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
    //supernova data
    int dim[3] = {432, 432, 432};
    unsigned long data_size = dim[0] * dim[1] * dim[2] * 4; // bytes
    std::string data_prefix = "/mnt/seenas1/data/supernova/E_";
    std::string data_extension = ".dat";
    int time_range[2] = {1295, 1354};
    int num_volumes = time_range[1] - time_range[0] + 1;

    //render info
    std::string output_prefix = "/home/ahota/projects/pbnj/renders/supernova/";
    std::string output_extension = ".ppm";
    int image_size[2] = {512, 512};
    std::vector<float> colormap = pbnj::viridis;
    float attenuation = 0.01;
    int samples = 4;

    //system data
    struct sysinfo system_info;
    sysinfo(&system_info);
    unsigned long total_bytes, free_bytes, max_usage, max_queue_size;
    total_bytes = system_info.mem_unit * system_info.totalram / 1073741824; //GB
    free_bytes = system_info.mem_unit * system_info.freeram / 1073741824; // GB
    max_usage = system_info.mem_unit * system_info.freeram * 0.5; // bytes
    max_queue_size = 10; //max_usage / data_size;
    std::cout << "Total memory: " << total_bytes << "GB" << std::endl;
    std::cout << "Free memory:  " << free_bytes << "GB" << std::endl;
    std::cout << "Max to use:   " << max_usage/1073741824 << "GB" << std::endl;
    std::cout << "Concurrent vols: " << max_queue_size << std::endl;

    //queue
    std::queue<pbnj::Volume*> volume_queue;

    pbnj::pbnjInit(&argc, argv);

    //set up camera
    pbnj::Camera *camera = new pbnj::Camera(image_size[0], image_size[1]);
    float orig_position[3] = {0.0, 0.0, (float)dim[2]};
    //camera->setPosition(orig_position[0], orig_position[1], orig_position[2]);
    float rot_rad = (float)dim[2];
    camera->setUpVector(0.0, 1.0, 0.0);

    int rindex = time_range[0];
    int rotation = 0;
    // load volumes for the whole time range
    // if we start hitting the memory limit, volumes are popped out
    // and an image is rendered. At the end, any remaining volumes are
    // rendered
    for(int dindex = time_range[0]; dindex <= time_range[1]; dindex++) {
        std::string current_filename = data_prefix + std::to_string(dindex) +
            data_extension;
        std::cout << "Current file: " << current_filename << std::endl;
        //if the queue has hit max capacity, start popping
        if(volume_queue.size() >= max_queue_size) {
            std::cout << "Rendering " << rindex << std::endl;
            std::string output_filename = output_prefix +
                std::to_string(rindex++) + output_extension;
            float xPos = rot_rad * std::sin(rotation * PI/180.);
            float zPos = rot_rad * std::cos(rotation * PI/180.);
            camera->setPosition(xPos, 0.0, zPos);
            rotation += 3;
            pbnj::Renderer *renderer = new pbnj::Renderer();
            renderer->setVolume(volume_queue.front());
            renderer->setCamera(camera);
            renderer->setSamples(samples);
            renderer->renderImage(output_filename);
            volume_queue.pop();
        }
        volume_queue.push(new pbnj::Volume(current_filename, dim[0],
                    dim[1], dim[2]));
        volume_queue.back()->setColorMap(colormap);
        volume_queue.back()->attenuateOpacity(attenuation);
    }

    int remaining = volume_queue.size();
    for(int i = remaining; i > 0; i--) {
        std::cout << "Rendering " << rindex << std::endl;
        std::string output_filename = output_prefix + std::to_string(rindex++) +
            output_extension;
        float xPos = rot_rad * std::sin(rotation * PI/180.);
        float zPos = rot_rad * std::cos(rotation * PI/180.);
        camera->setPosition(xPos, 0.0, zPos);
        rotation += 3;
        pbnj::Renderer *renderer = new pbnj::Renderer();
        renderer->setVolume(volume_queue.front());
        renderer->setCamera(camera);
        renderer->setSamples(samples);
        renderer->renderImage(output_filename);
        volume_queue.pop();
    }

    return 0;
}

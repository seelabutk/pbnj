#include "DataFile.h"

#include <iostream>

int main(int argc, char **argv) {
    std::cout << "Hello world!" << std::endl;

    std::string filename(
    "/export/share/data/magnetic/magnetic-512-volume/magnetic-512-volume.raw");

    pbnj::DataFile *datafile = new pbnj::DataFile(512, 512, 512);
    datafile->loadFromFile(filename);

    datafile->printStatistics();
    datafile->calculateStatistics();
    datafile->printStatistics();

    return 0;
}

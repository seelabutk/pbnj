#include "pbnj.h"

#include <ospray/ospray.h>

#include <chrono>
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>

namespace pbnj {

void pbnjInit(int *argc, const char **argv)
{
    // eventually this can parse argv for pbnj flags, eg for config files
    // just call OSPRay initialization for now though
    // NOTE: this MUST be called after reading a config file!
    ospInit(argc, argv);
}

std::string createID()
{
    // IDs are composed of a MAC address followed by a time
    FILE *arp = fopen("/proc/net/arp", "r");
    if(arp == NULL) {
        std::cerr << "WARNING: Could not open /proc/net/arp. ";
        std::cerr << "Object IDs won't work." << std::endl;
        return "";
    }

    char buffer[1024];
    // read the column headers
    char *header = fgets(buffer, 1024, arp);
    if(header == NULL) {
        std::cerr << "WARNING: Unexpected format in /proc/net/arp. ";
        std::cerr << "Object IDs won't work." << std::endl;
        return "";
    }

    char *res = fgets(buffer, 1024, arp);
    if(res == NULL) {
        std::cerr << "WARNING: Unexpected error reading /proc/net/arp. ";
        std::cerr << "Object IDs won't work." << std::endl;
        return "";
    }

    char *macaddr;
    macaddr = strtok(buffer, " ");
    macaddr = strtok(NULL, " ");
    macaddr = strtok(NULL, " ");
    macaddr = strtok(NULL, " ");

    // current time in milliseconds
    unsigned long int ms = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    std::string ID(macaddr);
    
    return ID + ":" + std::to_string(ms);
}

}

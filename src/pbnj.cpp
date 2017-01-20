#include "pbnj.h"

#include <ospray/ospray.h>

namespace pbnj {

void pbnjInit(int *argc, const char **argv)
{
    // eventually this can parse argv for pbnj flags, eg for config files
    // just call OSPRay initialization for now though
    // NOTE: this MUST be called after reading a config file!
    ospInit(argc, argv);
}

}

#include "pbnj.h"
#include "ParticleDataFile.h"

int main(int argc, const char **argv)
{
    pbnj::ParticleDataFile *pdf = new pbnj::ParticleDataFile();
    pdf->loadFromFile("/home/ahota/projects/pbnj/configs/test.xyz");

    return 0;
}

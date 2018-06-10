#ifndef PBNJ_H
#define PBNJ_H

#include <ospray/ospray.h>

#include <string>

namespace pbnj {
    /* contains metadata about the data being requested
     * This includes things like:
     *  - filename
     *  - file type
     *  - data type
     *  - dimensions
     *  - min/max
     *  etc.
     */
    class DataFile;

    /* abstraction wrapper around OSPRay volumes */
    class Volume;

    class Streamlines;

    /* abstraction around Volume to hold a series of data volumes */
    class TimeSeries;

    /* abstraction wrapper around OSPRay transfer functions
     * combines color and opacity tfs into a single object
     */
    class TransferFunction;

    /* abstraction wrapper around OSPRay renderer */
    class Renderer;

    /* abstraction wrapper around OSPRay camera
     * Provides simplified camera movement
     */
    class Camera;

    /* configuration class, uses rapidjson to parse JSON
     * config files
     */
    class ConfigReader;

    class Configuration;

    void pbnjInit(int *argc, const char **argv);

    std::string createID();
}

#endif
